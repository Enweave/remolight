# frontend

npm version 6.14.13
node version v14.17.0

## Project setup
```
npm install
```

### get [mkcert](https://github.com/FiloSottile/mkcert)

install it and generate certificate for localhost

```
mkcert -install
mkcert localhost
```
put *.pem and *key.pem files in project root. 
they should be named `localhost.pem` and `localhost.key.pem`.

### Compiles and hot-reloads for development
```
npm run serve
```

### Compiles and minifies for production
```
npm run build
```

### Lints and fixes files
```
npm run lint
```

### Customize configuration
See [Configuration Reference](https://cli.vuejs.org/config/).


### dev server
```
node dev_server.js
```
