<template>
    <div>
        <app-animation-upload />
        <app-picture-drawer :disabled="true"/>
        <app-animation-gallery />
    </div>
</template>

<script>
import { defineComponent } from '/@utils/modifiers';
const AppAnimationUpload = defineComponent(() => import('/@components/animationUpload.vue'));
const AppPictureDrawer = defineComponent(() => import('/@components/pictureDrawer.vue'));
const AppAnimationGallery = defineComponent(() => import('/@components/animationGallery.vue'));

export default {
    name: 'app-animations',
    components: {
        AppAnimationUpload,
        AppPictureDrawer,
        AppAnimationGallery
    }
}
</script>
