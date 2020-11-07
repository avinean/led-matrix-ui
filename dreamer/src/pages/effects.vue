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
                            v-for="effect in effectsList"
                            :key="effect"
                            :value="effect"
                        >{{ effect }}</option>
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
        <div class="ui card" style="width: auto;">
            <div class="content">
                <div class="upload__button">
                    <button
                        class="ui right labeled icon button green"
                        @click="apply"
                    >
                        <i class="check icon"></i>
                        Apply
                    </button>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
import services from "/@utils/services";

export default {
    name: 'app-effects',
    data() {
        return {
            selectedEffect: 0,
            speed: 5,
            effectsList: [
                'effect 1',
                'effect 2',
                'effect 3',
                'effect 4',
                'effect 5',
                'effect 6',
                'effect 7',
            ],
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
        apply() {
            services.setPictureEffects({
                effect: this.selectedEffect,
                speed: this.speed
            })
        }
    },
    mounted() {
        $('.ui.dropdown').dropdown();
        $('.ui.range').range(this.speedConfig);

        services.getEffectsList().then(effects => {
            this.effectsList = effects;
        })
    }
}
</script>
