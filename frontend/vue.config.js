// vue.config.js

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
    }
}