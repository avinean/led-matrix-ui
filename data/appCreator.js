const app = Vue.createApp({
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
                }
            ],
            currentTab: 'Picture',
        };
    },
    watch: {
        currentTab() {
            localStorage.currentTab = this.currentTab;
        }
    },
    mounted() {
        if (localStorage.currentTab) {
            this.currentTab = localStorage.currentTab;
        } 
    }
});