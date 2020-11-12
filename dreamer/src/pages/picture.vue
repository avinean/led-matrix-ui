<template>
    <div>
        <app-picture-drawer />
        <app-picture-upload />
        <app-picture-gallery />
    </div>
</template>

<script>
import { defineComponent } from '/@utils/modifiers';
const AppPictureDrawer = defineComponent(() => import('/@components/pictureDrawer.vue'));
const AppPictureUpload = defineComponent(() => import('/@components/pictureUpload.vue'));
const AppPictureGallery = defineComponent(() => import('/@components/pictureGallery.vue'));

export default {
    name: 'app-picture',
    components: {
        AppPictureDrawer,
        AppPictureUpload,
        AppPictureGallery
    }
}
</script>
