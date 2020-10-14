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
                    key: 'Effects for picture',
                    label: 'Effects for picture',
                    icon: 'film'
                },
                {
                    key: 'Animations',
                    label: 'Animations',
                    icon: 'play'
                }
            ],
            currentTab: 'Picture',
        };
    },
});