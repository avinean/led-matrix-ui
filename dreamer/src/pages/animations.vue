<template>
    <div>
        <app-animation-upload />
        <app-picture-drawer :disabled="true"/>
        <app-animation-gallery />
    </div>
</template>

<script>
import AppAnimationUpload from '/@components/animationUpload.vue'
import AppPictureDrawer from '/@components/pictureDrawer.vue'
import AppAnimationGallery from '/@components/animationGallery.vue'

export default {
    name: 'app-animations',
    components: {
        AppAnimationUpload,
        AppPictureDrawer,
        AppAnimationGallery
    }
}
</script>
