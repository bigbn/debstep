const Package = require('./Package')
const PackageManager = require('./PackageManager')

let pm = new PackageManager()
const package = new Package('vim')
console.log(package.getTagsSync())

module.exports = {Package, PackageManager}
