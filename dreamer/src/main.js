import { createApp } from 'vue'
import { defineComponent } from '/@utils/modifiers';

createApp(defineComponent(() => import('./App.vue'))).mount('#app')
