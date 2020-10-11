app.component('app-animations', {
    template: `
        <h4 class="ui horizontal divider header">
            <i class="play icon"></i>
            Animations
        </h4>
        <div class="ui card" style="width: auto;">
            <div class="content">
                <div class="mb-10">
                    <div class="ui input">
                        <input type="text" v-model="text">
                    </div>
                    <button @click="sendData">send</button>
                </div>
            </div>
        </div>
        <h4 class="ui horizontal divider header">
            <i class="fast forward icon"></i>
            Animation speed
        </h4>
        <div class="ui card" style="width: auto;">
            <div class="content">
                <div class="mb-10">
                    <div class="ui segment">
                        <div class="ui range" id="tmp"></div>
                    </div>
                </div>
            </div>
        </div>
    `,
    data() {
        return {
            speed: 0,
            speedConfig: {
                min: 0,
                max: 10,
                onChange: (value) => {
                    this.speed = value;
                },
            }
        };
    },
    methods: {
        sendData() {
            fetch(`/animated-text?speed=${this.speed}&text=${encodeURIComponent(this.text)}`)
        }
    },
    mounted() {
        $('.ui.range').range(this.speedConfig);
    }
});