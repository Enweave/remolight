import App from './App.vue'
// import './registerServiceWorker'
import router from './router'
import store from './store'
import './assets/style/app.sass'
import {createApp} from "vue";

import runtime from "serviceworker-webpack-plugin/lib/runtime";

if ("serviceWorker" in navigator) {
  runtime.register();
}

createApp(App).use(store).use(router).mount('#app')