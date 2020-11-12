<template>
    <app-home v-if="inited"/>
</template>

<script>
import AppHome from '/@pages/home.vue';
import store from '/@store/index';
import services from '/@utils/services';

export default {
    name: 'App',
    provide: {
        store,
    },
    components: {
        AppHome,
    },
    data() {
        return {
            inited: false
        }
    },
    mounted() {
        services
            .getMatrixParameters()
            .then(() => {
                this.inited = true;
            });
    }
}
</script>

<style>
* {
    box-sizing: border-box;
    margin: 0;
    padding: 0;
    font-family: sans-serif;
}

.picture__row {
    display: flex;
}

.picture__column {
    width: 2px;
    height: 2px;
}

.drawer {
    max-width: 600px;
    margin: 10px auto;
}

.drawer__row {
    display: flex;
}

.drawer__col {
    border: 1px solid #f5f5f5;
}

.picker_wrapper {
    left: 0 !important;
    right: 0 !important;
    margin: auto !important;
}

.picker_arrow {
    display: none !important;
}

.upload__button {
    text-align: center;
}

.upload__drawer {
    /* display: none; */
    margin: 10px auto;
}

.ui.header:first-child {
    margin-top: 1.14285714em;
}

.mb-10 {
    margin-bottom: 10px !important;
}

.center {
    text-align: center;
}
</style>
