let trueThis;

app.component('app-effects', {
    template: `
        <h4 class="ui horizontal divider header">
            <i class="film icon"></i>
            Effect
        </h4>
        <div class="ui card" style="width: auto;">
            <div class="content">
                <div class="mb-10">
                    <select ref="dropdown" class="ui search dropdown">
                        <option value="">Select effect</option>
                        <option
                            v-for="(label, value) in effectsList"
                            :key="label"
                            :value="value"
                        >{{ label }}</option>
                    </select>
                </div>
            </div>
        </div>
        <h4 class="ui horizontal divider header">
            <i class="fast forward icon"></i>
            Effect spped
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
            selectedEffect: 0,
            speed: 0,
            effectsList: {
                0: 'effect 1',
                1: 'effect 2',
                2: 'effect 3',
                3: 'effect 4',
                4: 'effect 5',
                5: 'effect 6',
                6: 'effect 7',
            },
            speedConfig: {
                min: 0,
                max: 10,
                start: 5,
                // onChange: trueThis.setSpeed.bind(trueThis),
            }
        };
    },
    methods: {
        setSpeed(speed) {
            this.speed = speed;
        }
    },
    mounted() {
        $('.ui.dropdown').dropdown();
        $('#tmp').range(this.speedConfig);
    },
    created() {
        trueThis = this;
    }
});