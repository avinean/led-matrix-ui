<template>
    <div>
        <h4 class="ui horizontal divider header">
            <i class="folder open icon"></i>
            Find picture in gallery
        </h4>
        <div class="ui card" style="width: auto;">
            <div class="content">
                <div
                    v-if="images.length"
                    class="ui small images mb-10"
                >
                    <span
                        v-for="image in images"
                        :key="image"
                    >
                        <img
                            :src="'https://dreamer-led.000webhostapp.com/image.php?image=' + image"
                            @click="drawImage"
                        />
                    </span>
                </div>
                <div
                    class="upload__button mb-10"
                >
                    <button
                        class="ui left labeled icon button primary"
                        :class="{
                            loading: loading
                        }"
                        @click="loadGallery"
                    >
                        <i class="image icon"></i>
                        {{ images.length ? 'Load more' : 'Load gallery' }}
                    </button >
                </div>
            </div>
        </div>
    </div>
</template>

<script>
import services from '../../utils/services';
import { observer, IMG_CHOSEN_FROM_GALLERY } from '../../utils/observer';

export default {
    name: 'app-picture-gallery',
    data() {
        return {
            images: [],
            loading: false,
        }
    },
    methods: {
        loadGallery() {
            this.loading = true;

            services.getImagesFromGallery().then((response) => {
              if (response.imageList.length) {
                this.images = response.imageList;
              }
            }).finally(() => {
              this.loading = false;
            });
        },
        drawImage({ target }) {
            const image = target;
            image.crossOrigin = "Anonymous";
            observer.emit(IMG_CHOSEN_FROM_GALLERY, image);
        }
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
</style>