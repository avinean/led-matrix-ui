<template>
    <div>
        <app-animation-upload />
        <app-picture-drawer :disabled="true"/>
    </div>
</template>

<script>
import AppAnimationUpload from './components/animationUpload.vue'
import AppPictureDrawer from './components/pictureDrawer.vue'

export default {
    name: 'app-animations',
    components: {
        AppAnimationUpload,
        AppPictureDrawer
    }
}
</script>