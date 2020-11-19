<template>
    <div class="gallery">
        <h4 class="ui horizontal divider header">
            <i class="folder open icon"></i>
            Gallery
        </h4>
        <div class="ui card" style="width: auto;">
            <div class="content">
                <div class="gallery-handlers mb-10">
                    <select
                        class="ui dropdown fluid mb-10"
                        v-model="mode"
                        @change="loadGallery"
                    >
                        <option value="0">Images from gallery</option>
                        <option value="1">Images from memory</option>
                        <option value="2">Animations from gallery</option>
                        <option value="3">Animations from memory</option>
                    </select>
                    <button
                        v-if="mode && store.state.gallery.links.length"
                        class="ui green compact icon button"
                        @click="playAll"
                    >
                        <i class="play icon"></i>
                    </button>
                </div>
                <div
                    v-if="store.state.gallery.links.length"
                    class="ui small images mb-10"
                >
                    <span
                        v-for="(gif, i) in store.state.gallery.links"
                        :key="gif"
                        class="images__item"
                    >
                        <img
                            crossorigin="Anonymous"
                            :ref="'gif' + i"
                            :src="prefix + gif"
                            @click="selectGif(prefix + gif)"
                        />
                    </span>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
import services from '/@utils/services';
import store from '/@store/index';

export default {
    name: 'app-animation-gallery',
    inject: ['store'],
    data() {
      return {
          mode: null,
      }
    },
    computed: {
          prefix() {
              switch(this.mode) {
                  case '0': return 'https://dreamer-led.000webhostapp.com/image.php?image=';
                  case '2': return 'https://dreamer-led.000webhostapp.com/gif.php?gif=';
                  default: return '';
              }
          }
    },
    methods: {
        async loadGallery() {
            this.store.clearGalleryLinks();
            switch(this.mode) {
                case '0': return await services.getImagesFromGallery();
                case '1': return await services.getImagesFromController();
                case '2': return await services.getAnimationsFromGallery();
                case '3': return await services.getAnimationsFromController();
            }
        },
        selectGif(url) {
            const [ fileName ] = url.match(/[\d\w]+\.gif$/gim) || [];
            return fetch(url)
                .then(e => e.blob())
                .then(blob => {
                    const file = new File([blob], fileName);

                    const formData = new FormData();
                    formData.append('update', file);

                    return services.sendFile(formData);
                })
        },
        async playAll() {
            switch(this.mode) {
                case '0': return this.sendAllRecursively();
                case '1': return await services.playAll();
                case '2': return this.sendAllRecursively();
                case '3': return await services.playAll();
            }
        },
        sendAllRecursively() {
            const send = async (i) => {
                const fileName = this.store.state.gallery.links[i];
                if (!fileName) {
                    await services.playAll();
                    return;
                }
                await this.selectGif(this.prefix + fileName);
                await send(i + 1);
            }

            send(0);
        }
    },
    mounted() {
        $('.ui.dropdown').dropdown();
    },
    unmounted () {
        this.store.clearGalleryLinks();
    }
}
</script>

<style>
.images {
  display: flex;
  justify-content: space-around;
  flex-wrap: wrap;
}

.images > span {
  margin-bottom: 10px;
}

.gallery .ui.images img {
  width: 50px;
  height: 50px;
}

.images__item {
    border: 2px solid transparent;
    border-radius: 4px;
}

.images__item--active {
    border: 2px solid grey;
}

.gallery-handlers {
    display: flex;
    align-items: center;
}

.gallery-handlers > div {
    margin: 0 !important;
}
.gallery-handlers > button {
    margin-left: 10px !important;
}

</style>
