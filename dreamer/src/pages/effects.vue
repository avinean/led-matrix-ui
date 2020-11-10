<template>
    <div>
        <h4 class="ui horizontal divider header">
            <i class="film icon"></i>
            Effects
        </h4>
        <div class="ui card center" style="width: auto;">
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

                <div class="mb-10">
                    <button
                        class="ui right labeled icon button green"
                        @click="rotate"
                    >
                        <i class="sync icon"></i>
                        Rotate
                    </button>
                </div>

                <div class="mb-10">
                    <div class="ui segment" style="display: flex;">
                        {{ speedConfig.min }} <div class="ui range" id="tmp"></div> {{ speedConfig.max }}
                    </div>
                </div>
            </div>
        </div>
        <div class="ui card" style="width: auto;">
            <div class="content">
                <div class="center">
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
    inject: ['store'],
    data() {
        return {
            selectedEffect: 0,
            speed: 5,
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
    computed: {
        effectsList() {
            return this.store.state.matrixParams.effects;
        }
    },
    methods: {
        apply() {
            services.setPictureEffects({
                effect: this.selectedEffect,
                speed: this.speed
            })
        },
        rotate() {
            services.rotate();
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
