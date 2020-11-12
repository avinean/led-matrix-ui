<template>
    <div v-if="inited">
        <div class="ui container main">
            <component :is="currentPage.component" />

            <div
                class="main__menu"
                :class="isMenuOpened ? 'main__menu--opened' : ''"
                @click="isMenuOpened = !isMenuOpened"
            >
                <button
                    v-for="(tab, key) in tabs"
                    class="ui circular icon button massive main__menu-item"
                    :class="[
                    key === currentTab ? 'main__menu-item--active' : '',
                    tab.color
                ].join(' ')"
                    :key="key"
                    @click="currentTab = key"
                >
                    <i
                        class="icon"
                        :class="tab.icon"
                    ></i>
                    <span>{{ tab.label }}</span>
                </button>
            </div>
        </div>
    </div>
</template>

<script>
import { defineComponent } from '/@utils/modifiers';
import store from '/@store/index';
import services from '/@utils/services';

export default {
    name: 'App',
    provide: {
        store,
    },
    data: () => {
        return {
            inited: false,
            tabs: {
                'Picture': {
                    label: 'Picture',
                    icon: 'image',
                    component: defineComponent(() => import('/@pages/picture.vue')),
                    color: 'red'
                },
                'Text/Clock': {
                    label: 'Text/Clock',
                    icon: 'keyboard outline',
                    component: defineComponent(() => import('/@pages/text.vue')),
                    color: 'green'
                },
                'Animations': {
                    label: 'Animations',
                    icon: 'play',
                    component: defineComponent(() => import('/@pages/animations.vue')),
                    color: 'teal'
                },
                'Effects for picture': {
                    label: 'Effects for picture',
                    icon: 'film',
                    component: defineComponent(() => import('/@pages/effects.vue')),
                    color: 'blue'
                },
                'Games': {
                    label: 'Games',
                    icon: 'chess',
                    component: defineComponent(() => import('/@pages/games.vue')),
                    color: 'yellow'
                },
                'Settings': {
                    label: 'Settings',
                    icon: 'cogs',
                    component: defineComponent(() => import('/@pages/settings.vue')),
                    color: 'orange'
                },
            },
            isMenuOpened: false,
            openedPage: ''
        };
    },
    computed: {
        currentPage () {
            return this.tabs[this.currentTab];
        },
        currentTab: {
            get () {
                return this.openedPage || localStorage.currentTab || Object.keys(this.tabs)[0];
            },
            set (tab) {
                if (this.isMenuOpened) {
                    this.openedPage = tab;
                    localStorage.currentTab = tab;
                }
            }
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

.main {
    position: relative;
    min-height: 100vh;
    padding: 20px 0 50px;
}

.main > .segment {
    min-height: 95vh;
}

.main__menu {
    position: fixed;
    bottom: 20px;
    right: 20px;
    background: rgba(0, 0, 0, 0);
    transition: all ease 0.5s;
}

.main__menu--opened {
    width: 100vw;
    height: 100vh;
    top: 0px;
    left: 0px;
    z-index: 9;
    background: rgba(0, 0, 0, 0.8);
}

.main__menu--opened .main__menu-item:nth-child(2) {bottom: 90px;}
.main__menu--opened .main__menu-item:nth-child(3) {bottom: 160px;}
.main__menu--opened .main__menu-item:nth-child(4) {bottom: 230px;}
.main__menu--opened .main__menu-item:nth-child(5) {bottom: 300px;}
.main__menu--opened .main__menu-item:nth-child(6) {bottom: 370px;}
.main__menu--opened .main__menu-item:nth-child(7) {bottom: 440px;}
.main__menu--opened .main__menu-item:nth-child(8) {bottom: 610px;}
.main__menu--opened .main__menu-item:nth-child(9) {bottom: 680px;}

.main__menu-item {
    position: fixed;
    bottom: 20px;
    right: 20px;
    transition: all ease .5s;
    box-shadow: rgba(0, 0, 0, 0.5) -5px -4px 13px -3px inset !important;
}

.main__menu-item--active {
    z-index: 999999;
}

.main__menu-item span {
    position: fixed;
    opacity: 0;
    right: 0px;
    transition: all 0.5s ease 0s;
}

.main__menu--opened .main__menu-item span {
    right: 100px;
    opacity: 1;
}

.global-loader {
    position: fixed;
    top: 0;
    bottom: 0;
    left: 0;
    right: 0;
    margin: 0;
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
