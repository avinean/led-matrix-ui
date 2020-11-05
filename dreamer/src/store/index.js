import { reactive, readonly } from 'vue';

const state = reactive({
    loading: 0,
    isImageLoaded: null,
    imageList: [],
    gallery: {
        links: [],
    },
    matrixParams: {
        height: 0,
        width: 0,
    },
});

const setLoading = loading => loading ? state.loading++ : state.loading--;
const setImageLoaded = loaded => state.isImageLoaded = loaded;
const setImageList = imageList => state.imageList = imageList;
const setGalleryLinks = links => state.gallery.links = links;
const setMatrixParams = matrixParams => state.matrixParams = matrixParams;

export default {
    state: readonly(state),

    setLoading,
    setImageList,
    setGalleryLinks,
    setImageLoaded,
    setMatrixParams
};


