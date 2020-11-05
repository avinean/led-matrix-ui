<template>
    <div>
        <div class="ui container main">
            <div
                v-if="currentTab === 'Picture'"
                class="ui segment"
            >
                <app-picture></app-picture>
            </div>
            <div
                v-if="currentTab === 'Text/Clock'"
                class="ui segment"
            >
                <app-text></app-text>
            </div>
            <div
                v-if="currentTab === 'Effects for picture'"
                class="ui segment"
            >
                <app-effects></app-effects>
            </div>
            <div
                v-if="currentTab === 'Animations'"
                class="ui segment"
            >
                <app-animations></app-animations>
            </div>
            <div
                v-if="currentTab === 'Games'"
                class="ui segment"
            >
                <app-games></app-games>
            </div>
            <div
                v-if="currentTab === 'Settings'"
                class="ui segment"
            >
                <app-settings></app-settings>
            </div>

            <div
                class="main__menu"
                :class="isMenuOpened ? 'main__menu--opened' : ''"
                @click="isMenuOpened = !isMenuOpened"
            >
                <button
                    v-for="(tab, i) in tabs"
                    class="ui circular icon button massive main__menu-item"
                    :class="[
                    tab.key === currentTab ? 'main__menu-item--active' : '',
                    colors[i]
                ].join(' ')"
                    :key="tab.key"
                    @click="goTo(tab.key)"
                >
                    <i
                        class="icon"
                        :class="tab.icon"
                    ></i>
                    <span>{{ tab.label }}</span>
                </button>
            </div>
        </div>

        <div v-show="store.state.loading" class="ui segment global-loader">
            <div class="ui active dimmer inverted">
                <div class="ui medium text loader">Loading</div>
            </div>
        </div>
    </div>

</template>

<script>
import AppGames from '/@pages/games.vue';
import AppAnimations from '/@pages/animations.vue';
import AppEffects from '/@pages/effects.vue';
import AppText from '/@pages/text.vue';
import AppPicture from '/@pages/picture.vue';
import AppSettings from '/@pages/settings.vue';

export default {
    name: 'app-home',
    inject: ['store'],
    components: {
        AppGames,
        AppAnimations,
        AppEffects,
        AppText,
        AppPicture,
        AppSettings,
    },
    data() {
        return {
            colors: ['red', 'green', 'teal', 'blue', 'violet', 'brown', 'grey'],
            tabs: [
                {
                    key: 'Picture',
                    label: 'Picture',
                    icon: 'image'
                },
                {
                    key: 'Text/Clock',
                    label: 'Text/Clock',
                    icon: 'keyboard outline',
                },
                {
                    key: 'Animations',
                    label: 'Animations',
                    icon: 'play'
                },
                {
                    key: 'Effects for picture',
                    label: 'Effects for picture',
                    icon: 'film'
                },
                {
                    key: 'Games',
                    label: 'Games',
                    icon: 'chess'
                },
                {
                    key: 'Settings',
                    label: 'Settings',
                    icon: 'cogs'
                },
            ],
            currentTab: '',
            isMenuOpened: false,
        };
    },
    watch: {
        currentTab() {
            localStorage.currentTab = this.currentTab;
        }
    },
    methods: {
      goTo(tab) {
        if (this.isMenuOpened) {
          this.currentTab = tab;
        }
      }
    },
    mounted() {
        this.currentTab = localStorage.currentTab || this.tabs[0].key;
    }
}
</script>

<style scoped>

.main {
    position: relative;
    padding: 20px 0 50px;
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
</style>
