import App from './App.vue'
import './registerServiceWorker'
import router from './router'
import store from './store'
import './assets/style/app.sass'
import {createApp} from "vue";

createApp(App).use(store).use(router).mount('#app')