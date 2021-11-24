const express = require('express')
const cors = require('cors')

const app = express()
const host = '127.0.0.1'
const port = 3000

app.use(cors({
    origin: '*'
}));
app.use(express.json());

app.get('/', (req, res) => {
  res.json({
      c0: 10,
      c1: 20,
      c3: 30,
      c4: 40,
      c5: 50,
      c6: 60,
      c7: 70,
      c8: 80,
      c9: 90,
      c10: 100,
      c11: 110,
      c12: 120,
      c13: 130,
      c14: 140,
      c15: 150,
    })
})

app.post('/', (req, res) => {
    console.log(req.body);
  res.json(
      req.body
  )
})

app.listen(port, () => {
  console.log(`Example app listening at http://${host}:${port}`)
})