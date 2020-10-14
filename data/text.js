app.component('app-text', {
    template: `
        <h4 class="ui horizontal divider header">
            <i class="keyboard outline icon"></i>
            Running text
        </h4>
        <div class="ui card" style="width: auto;">
            <div class="content">
                <div class="mb-10">
                    <select ref="dropdown" class="ui search dropdown">
                        <option value="">Select effect</option>
                        <option
                            v-for="(value, label) in effectsList"
                            :key="label"
                            :value="value"
                        >{{ label }}</option>
                    </select>
                </div>
            </div>
        </div>
        <h4 class="ui horizontal divider header">
            <i class="cogs icon"></i>
            Settings
        </h4>
        <div class="ui card" style="width: auto;">
            <div class="content">
                <template v-if="selectedEffect === effectsList.text">
                    <div class="mb-10">
                        <div class="ui segment" style="display: flex;">
                            {{ speedConfig.min }} <div class="ui range" id="tmp"></div> {{ speedConfig.max }}
                        </div>
                    </div>
                </template>
                <template v-if="selectedEffect === effectsList.clock">
                    <div class="grouped fields">
                        <label for="fruit">Sync clock:</label>
                        <div class="field">
                            <div class="ui radio checkbox">
                                <input type="radio" name="fruit" checked="" tabindex="0" class="hidden">
                                <label>With internet</label>
                            </div>
                        </div>
                        <div class="field">
                            <div class="ui radio checkbox">
                                <input type="radio" name="fruit" tabindex="0" class="hidden">
                                <label>With phone</label>
                            </div>
                        </div>
                    </div>
                </template>
            </div>
        </div>
    `,
    data() {
        return {
            selectedEffect: 0,
            speed: 5,
            effectsList: {
                text: 0,
                clock: 1,
            },
            speedConfig: {
                min: 0,
                max: 6,
                start: 3,
                onChange: (value) => {
                    this.speed = value;
                },
            }
        };
    },
    mounted() {
        $('.ui.dropdown').dropdown();
        $('.ui.range').range(this.speedConfig);
    }
});