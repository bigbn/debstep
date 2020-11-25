const Package = require('./Package')
const PackageManager = require('./PackageManager')
const bindings = require('./build/Release/bindings');

console.log(bindings.getPackageTags("npm", ["Package", "XB-Nginx-Template", "Version", "Description"]))
console.log(bindings.getPackageCandidateVersion("npm"))
console.log(bindings.getPackageCurrentVersion("npm"))
let pm = new PackageManager()
const package = new Package('vim')
console.log(package.getTagsSync())

module.exports = {Package, PackageManager}
