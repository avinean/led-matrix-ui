<template>
    <div class="toolbar">
        <div class="ui card center" style="width: auto;">
            <div class="content">

                <div class="ui small basic icon buttons">
                    <button
                        class="ui circular facebook icon button"
                        @click="rotate"
                    >
                        <i class="retweet icon"></i>
                        <span>Rotate</span>
                    </button>
                    <button
                        class="ui circular facebook icon button"
                        @click="rotate"
                    >
                        <i class="sync icon"></i>
                        <span>Refresh</span>
                    </button>
                    <button
                        class="ui circular facebook icon button"
                        @click="toggleSpeed"
                    >
                        <i class="angle double right icon"></i>
                        <span>Speed</span>
                    </button>
                </div>

                <div v-show="isSpeedShown" class="speed-range">
                    <app-range
                        :min="1"
                        :max="6"
                        v-model="speed"
                    ></app-range>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
import AppRange from '/@components/range.vue';
import services from "/@utils/services";

export default {
    name: 'app-toolbar',
    inject: ['store'],
    components: {
        AppRange,
    },
    data() {
        return {
            isSpeedShown: false,
            speed: 5,
            second: 1000
        };
    },
    watch:{
        speed() {
            this.setSpeed()
        }
    },
    methods: {
        toggleSpeed() {
          this.isSpeedShown = !this.isSpeedShown;
        },
        setSpeed() {
            services.setPictureEffects({
                speed: this.speed,
            }).then(() => {
                this.isSpeedShown = false;
            });
        },
        rotate() {
            services.rotate();
        },
        refresh() {
            services.getMatrixState();
        },
    },
}
</script>

<style scoped>
    .toolbar .buttons {
        display: flex;
        justify-content: flex-end;
    }

    .speed-range {
        padding: 20px 0;
    }

    .buttons .button span {
        display: block;
        font-size: 10px;
    }
</style>
