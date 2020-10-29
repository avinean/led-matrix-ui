<template>
    <div>
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
            Effect speed
        </h4>
        <div class="ui card" style="width: auto;">
            <div class="content">
                <div class="mb-10">
                    <div class="ui segment" style="display: flex;">
                        {{ speedConfig.min }} <div class="ui range" id="tmp"></div> {{ speedConfig.max }}
                    </div>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
import { EFFECT_SPEED, store } from "../utils/store";

export default {
    name: 'app-effects',
    data() {
        return {
            selectedEffect: 0,
            speed: 5,
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
                min: 1,
                max: 6,
                start: 3,
                onChange: (value) => {
                    this.speed = value;
                },
            },
            second: 1000
        };
    },
    watch: {
        speed() {
            store.set(EFFECT_SPEED, this.speed * this.second);
        }
    },
    mounted() {
        this.speedConfig.start = store.get(EFFECT_SPEED) / this.second;
        $('.ui.dropdown').dropdown();
        $('.ui.range').range(this.speedConfig);
    }
}
</script>
