<template>
    <div class="gallery">
        <h4 class="ui horizontal divider header">
            <i class="folder open icon"></i>
            Find picture in gallery
        </h4>
        <div class="ui card" style="width: auto;">
            <div class="content">
                <div v-if="images.length" class="ui checkbox mb-10">
                    <input
                        v-model="isMultiSelect"
                        type="checkbox"
                        tabindex="0"
                        class="hidden"
                        id="isMultiSelect"
                    >
                    <label for="isMultiSelect">Multiple mode</label>
                </div>
                <div
                    v-if="images.length"
                    class="ui small images mb-10"
                >
                    <span
                        v-for="(image, i) in images"
                        :key="image"
                        class="images__item"
                        :class="selectedImages.includes($refs['image' + i]) ? 'images__item--active' : ''"
                    >
                        <img
                            crossorigin="Anonymous"
                            :ref="'image' + i"
                            :src="'https://dreamer-led.000webhostapp.com/image.php?image=' + image"
                            @click="selectImage"
                        />
                    </span>
                </div>
                <div
                    class="upload__button mb-10"
                >
                    <button
                        class="ui left labeled icon button primary mb-10"
                        :class="{
                            loading: loading
                        }"
                        @click="loadGallery"
                    >
                        <i class="image icon"></i>
                        Load gallery
<!--                        {{ images.length ? 'Load more' : 'Load gallery' }}-->
                    </button >
                    <button
                        v-if="selectedImages.length"
                        class="ui left labeled icon button green"
                        @click="drawImages"
                    >
                        <i class="play icon"></i>
                        Run slide show
                        <!--                        {{ images.length ? 'Load more' : 'Load gallery' }}-->
                    </button >
                </div>
            </div>
        </div>
    </div>
</template>

<script>
import services from '../../utils/services';
import {store, IMG_CHOSEN_FROM_GALLERY, IMGS_CHOSEN_FROM_GALLERY} from '../../utils/store';

export default {
    name: 'app-picture-gallery',
    data() {
        return {
            images: [],
            selectedImages: [],
            loading: false,
            isMultiSelect: false
        }
    },
    watch: {
        isMultiSelect() {
            if (!this.isMultiSelect) {
                this.selectedImages = [];
            }
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
        drawImage(image) {
            store.emit(IMG_CHOSEN_FROM_GALLERY, image);
        },
        drawImages() {
            store.emit(IMGS_CHOSEN_FROM_GALLERY, [...this.selectedImages]);
        },
        collectImages(image) {
            const index = this.selectedImages.indexOf(image);
            if (!~index) {
                this.selectedImages.push(image);
            } else {
                this.selectedImages.splice(index, 1);
            }
        },
        selectImage({ target }) {
            if (this.isMultiSelect) {
                this.collectImages(target);
            } else {
                this.drawImage(target);
            }
        },
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