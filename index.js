const Package = require('./Package')
const PackageManager = require('./PackageManager')
const bindings = require('./build/Release/bindings');

console.log(bindings.getPackageTags("vim", ["Package", "XB-Nginx-Template", "Version", "Description"]))

let pm = new PackageManager()

module.exports = {Package, PackageManager}
