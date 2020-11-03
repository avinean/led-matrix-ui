<template>
  <app-home />
</template>

<script>
import AppHome from './pages/home.vue'

export default {
  name: 'App',
  components: {
    AppHome
  }
}
</script>

<style>
* {
  box-sizing: border-box;
  margin: 0;
  padding: 0;
  font-family: sans-serif;
}

.picture__row {
  display: flex;
}

.picture__column {
  width: 2px;
  height: 2px;
}

.drawer {
  max-width: 600px;
  margin: 10px auto;
}

.drawer__row {
  display: flex;
}

.drawer__col {
  border: 1px solid #f5f5f5;
}

.picker_wrapper {
  left: 0 !important;
  right: 0 !important;
  margin: auto !important;
}

.picker_arrow {
  display: none !important;
}

.upload__button {
  text-align: center;
}

.upload__drawer {
  /* display: none; */
  margin: 10px auto;
}

.game__controls {
  text-align: center;
}

.game__controls > div {
  display: flex;
  justify-content: center;
}

.game__controls button.left {
  margin-right: 45px !important;
}

.game__controls button.right {
  margin-left: 45px !important;
}

.game__controls button i {
  font-size: 90px;
  height: 90px;
  width: 90px;
  display: flex;
  align-items: center;
  justify-content: center;
}

.ui.header:first-child {
  margin-top: 1.14285714em;
}

.mb-10 { margin-bottom: 10px !important;}
</style>
