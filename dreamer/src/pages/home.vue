<template>
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
        
        <div class="ui five item menu main__menu">
            <a
                v-for="tab in tabs"
                class="item green"
                :class="tab.key === currentTab ? 'active' : ''"
                :key="tab.key"
                @click="currentTab = tab.key"
            >
                <i class="icon" :class="tab.icon" style="font-size: 30px;"></i>
            </a>
        </div>
    </div>
</template>

<script>
import AppGames from './games.vue';
import AppAnimations from './animations.vue';
import AppEffects from './effects.vue';
import AppText from './text.vue';
import AppPicture from './picture.vue';

export default {
    name: 'app-home',
    components: {
        AppGames,
        AppAnimations,
        AppEffects,
        AppText,
        AppPicture
    },
    data() {
        return {
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
                    label: 'games',
                    icon: 'chess'
                }
            ],
            currentTab: '',
        };
    },
    watch: {
        currentTab() {
            localStorage.currentTab = this.currentTab;
        }
    },
    mounted() {
        if (localStorage.currentTab) {
            this.currentTab = localStorage.currentTab || 'Picture';
        } 
    }
}
</script>