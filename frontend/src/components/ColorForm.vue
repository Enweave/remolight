<template>
  <div class="card">
    <div class="card-body row">
      <div class="mt-2 mb-2 col-sm-12 col-md-6 ">
        <label class="form-label">Api path:</label>
        <input class="form-control" v-model="api_path">
      </div>
      <div class="mt-2 mb-2 col-sm-12 col-md-6 ">
        <label class="form-label">Actions:</label>
        <div class=" d-flex align-items-center">
          <button class="flex-grow-0 btn btn-success" @click="getStuff">GET</button>
          <button class="flex-grow-0 btn btn-warning" @click="postStuff">POST</button>
        </div>
      </div>
    </div>
    <div class="card-body row">
      <div class="col-sm-6 col-md-3 mb-2" v-for="(i, key) in form_data" :key="key">
        <label class="form-label">{{ key }}:</label>
        <input type="number" min="0" max="4095" class="form-control" v-model="form_data[key]">
      </div>
    </div>
    <div class="card-footer"></div>
  </div>
</template>

<script>
export default {
  name: "ColorForm",
  data: () => ({
    api_path: 'https://176.226.134.125:8001/api',
    form_data: {
      c0: 0,
      c1: 0,
      c2: 0,
      c3: 0,
      c4: 0,
      c5: 0,
      c6: 0,
      c7: 0,
      c8: 0,
      c9: 0,
      c10: 0,
      c11: 0,
      c12: 0,
      c13: 0,
      c14: 0,
      c15: 0,
    }
  }),
  methods: {
    getStuff() {
      fetch(this.api_path)
          .then(res => res.json())
          .then(data => this.form_data = data)
    },
    postStuff() {
      fetch(this.api_path, {
            method: 'POST', // *GET, POST, PUT, DELETE, etc.
            mode: 'cors', // no-cors, *cors, same-origin
            cache: 'no-cache',
            headers: {
              'Content-Type': 'application/json'
              // 'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: JSON.stringify(this.form_data)
          }
      )
          .then(res => console.log(res))
    }
  }
}
</script>

<style scoped>

</style>