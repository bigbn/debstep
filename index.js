const Package = require('./Package')
const PackageManager = require('./PackageManager')
const bindings = require('./build/Release/bindings');

console.log(bindings.hello())

let pm = new PackageManager()

module.exports = {Package, PackageManager}
