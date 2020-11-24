const Package = require('./Package')
const PackageManager = require('./PackageManager')
const bindings = require('./build/Release/bindings');

console.log(bindings.get("welcome-page", ["Package", "XB-Nginx-Template", "Version", "Description"], '0.11.7-xenial'))

let pm = new PackageManager()

module.exports = {Package, PackageManager}
