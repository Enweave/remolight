// vue.config.js

const fs = require('fs');
const path = require("path");
const ServiceWorkerWebpackPlugin = require("serviceworker-webpack-plugin");

/**
 * @type {import('@vue/cli-service').ProjectOptions}
 */
module.exports = {
    // options...
    integrity: true,
    css: {
        sourceMap: true
    },
    chainWebpack: (config) => {
        config
            .plugin('html')
            .tap(args => {
                args[0].title = 'Remolight';
                args[0].meta = {
                    viewport: 'width=device-width,initial-scale=1,user-scalable=no'
                };

                return args;
            })
    },
    configureWebpack: {
        plugins: [
            new ServiceWorkerWebpackPlugin({
                entry: path.join(__dirname, "./src/sw.js")
            })
        ]
    },
    devServer: {
        https: {
            key: fs.readFileSync('./../localhost.key.pem'),
            cert: fs.readFileSync('./../localhost.pem'),
        },
        public: 'https://localhost:8080/',
    },
    pwa: {
        name: 'Remolight appa',
        themeColor: '#ba4d4d',
        msTileColor: '#000000',
        appleMobileWebAppCapable: 'yes',
        appleMobileWebAppStatusBarStyle: 'black',

        // configure the workbox plugin
        workboxPluginMode: 'InjectManifest',
        workboxOptions: {
            // swSrc is required in InjectManifest mode.
            swSrc: 'src/sw.js',
            swDest: 'sw.js'
            // ...other Workbox options...
        }
    }
}