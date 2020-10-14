const express = require('express')
const path = require('path')
const bodyParser = require('body-parser')
const app = express()
const port = 2832

const store = {};

app.use(express.static(path.join(__dirname, '../data')))
app.use(bodyParser.raw({limit: '50mb'}))
app.use(bodyParser.json())

app.get('/', (req, res) => {
  res.sendFile(path.resolve(__dirname, 'index.html'))
})

app.get('/matrix-parameters', (req, res) => {
  res.send({
    width: 16,
    height: 16,
  })
})

app.post('/pixel', (req, res) => {
  res.sendStatus(200)
})

app.post('/draw', (req, res) => {
  store.img = req.body
  res.sendStatus(200)
})

app.post('/fill-matrix', (req, res) => {
  console.log(req.body);
  res.sendStatus(200)
})

app.get('/matrix-state', (req, res) => {
  res.send(store.img)
})

app.listen(port, () => {
  console.log(`Example app listening at http://localhost:${port}`)
})
