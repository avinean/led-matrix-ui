const app = Vue.createApp({
    data() {
        return {
            tabs: [
                {
                    key: 'Picture',
                    label: 'Picture',
                },
                {
                    key: 'Effects for picture',
                    label: 'Effects for picture',
                },
                {
                    key: 'Animations',
                    label: 'Animations',
                }
            ],
            currentTab: 'Picture',
        };
    },
});