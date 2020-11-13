<template>
    <div>
        <app-picture-drawer />
        <app-picture-upload />
        <app-picture-gallery />
    </div>
</template>

<script>
import { defineComponent } from '/@utils/modifiers';
import AppPictureDrawer from '/@components/pictureDrawer.vue';
import AppPictureUpload from '/@components/pictureUpload.vue';
import AppPictureGallery from '/@components/pictureGallery.vue';

export default {
    name: 'app-picture',
    components: {
        AppPictureDrawer,
        AppPictureUpload,
        AppPictureGallery
    }
}
</script>
