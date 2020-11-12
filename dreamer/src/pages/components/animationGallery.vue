<template>
    <div class="gallery">
        <h4 class="ui horizontal divider header">
            <i class="folder open icon"></i>
            Find gif in gallery
        </h4>
        <div class="ui card" style="width: auto;">
            <div class="content">
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
                            :src="'https://dreamer-led.000webhostapp.com/gif.php?gif=' + gif"
                            @click="selectGif('https://dreamer-led.000webhostapp.com/gif.php?gif=' + gif)"
                        />
                    </span>
                </div>
                <div
                    class="upload__button mb-10"
                >
                    <button
                        class="ui left labeled icon button primary mb-10"
                        :class="{
                            loading: store.state.loading
                        }"
                        @click="loadGallery"
                    >
                        <i class="image icon"></i>
                        Load gallery
<!--                        {{ images.length ? 'Load more' : 'Load gallery' }}-->
                    </button >
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
    methods: {
        async loadGallery() {
            await services.getAnimationsFromGallery();
        },
        selectGif(url) {
            const [ fileName ] = url.match(/[\d\w]+\.gif$/gim) || [];
            fetch(url)
                .then(e => e.blob())
                .then(blob => {
                    const file = new File([blob], fileName);

                    const formData = new FormData();
                    formData.append('update', file);

                    services.sendFile(formData);
                })
        },
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
</style>
