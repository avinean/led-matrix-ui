<template>
    <div>
        <h4 class="ui horizontal divider header">
            <i class="chess icon"></i>
            Games
        </h4>
        <div class="ui card" style="width: auto;">
            <div class="content">
                <div class="mb-10">

                    <div class="ui floating labeled icon dropdown button" id="dropdown">
                        <i class="icon" :class="selectedGame.icon || 'chess'"></i>
                        <span class="text">Game</span>
                        <div class="menu">
                            <div
                                v-for="game in gamesList"
                                :key="game.label"
                                :data-value="game.value"
                                :data-text="game.name"
                                selected
                                class="item"
                            >
                                <i class="icon" :class="game.icon"></i>
                                {{ game.name }}
                            </div>
                        </div>
                    </div>
                    
                    <button
                        class="ui left labeled icon button green"
                        :disabled="!selectedGame.value"
                        @click="play"
                    >
                        <i class="play icon"></i>
                        Play
                    </button >

                </div>
            </div>
        </div>
        <h4 class="ui horizontal divider header">
            <i class="arrows alternate icon"></i>
            Controls
        </h4>
        <div class="ui card" style="width: auto;">
            <div class="content">
                <div class="mb-10 game__controls">
                    
                    <button
                        class="ui icon button"
                        :disabled="!gameStarted"
                        @click="up"
                    >
                        <i class="caret square up outline icon"></i>
                    </button>
                    
                    <div>
                        <button
                            class="ui icon button left"
                            :disabled="!gameStarted"
                            @click="left"
                        >
                            <i class="caret square left outline icon"></i>
                        </button>
                        
                        <button
                            class="ui icon button right"
                            :disabled="!gameStarted"
                            @click="right"
                        >
                            <i class="caret square right outline icon"></i>
                        </button>
                    </div>
                    
                    <button
                        class="ui icon button"
                        :disabled="!gameStarted"
                        @click="down"
                    >
                        <i class="caret square down outline icon"></i>
                    </button>

                </div>
            </div>
        </div>
    </div>
</template>

<script>
import services from '../utils/services';

export default {
    name: 'app-games',
    data() {
        return {
            gamesList: [
                {
                    icon: 'python',
                    name: 'Snake',
                    value: 'snake',
                    selected: true,
                }
            ],
            currentGame: '',
            gameStarted: false
        };
    },
    computed: {
        selectedGame() {
            return this.gamesList.find(game => game.value === this.currentGame) || {};
        }
    },
    methods: {
        play() {
            if (!this.selectedGame.value) return;

            services.playGame({
                game: this.selectedGame.value,
            }).then(() => this.gameStarted = true);
        },
        up() {
            services.control('up')
        },
        right() {
            services.control('right')
        },
        down() {
            services.control('down')
        },
        left() {
            services.control('left')
        },
        watchKeys(e) {
            const keys = {
                37: 'left',
                38: 'up',
                39: 'right',
                40: 'down',
            }
            services.control(keys[e.keyCode]);
        }
    },
    mounted() {
        $('#dropdown').dropdown({
            value: this.currentGame,
            onChange: (value) => {
                this.currentGame = value;
            }
        });

        document.addEventListener('keydown', this.watchKeys);
    },
    unmounted() {
        document.removeEventListener('keydown', this.watchKeys);
    }
}
</script>