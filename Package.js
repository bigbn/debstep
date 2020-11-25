const BaseWrapper = require('./BaseWrapper')
const bindings = require('node-gyp-build')(__dirname)


console.log(bindings.getPackageTags("npm", ["Package", "XB-Nginx-Template", "Version", "Description"]))
console.log(bindings.getPackageCandidateVersion("libkdb5-9"))
console.log(bindings.getPackageCurrentVersion("libkdb5-9"))

/**
 * DEB-package representation
 * Must be initialized with asynchronous "load" method before
 * accessing to any properties
 *
 * @class Package
 * @property {boolean} loaded load state
 * @property {String} name name of the package
 * @property {boolean} hasUpgrades can package be upgraded
 * @property {String} avaialableVersion newer version available for upgrade
 * @extends {BaseWrapper}
 */
class Package extends BaseWrapper {
  constructor (packageName) {
    super()
    this.loaded = false
    this.name = packageName
    this.avaialableVersion = null
    this.hasUpgrades = false
  }

  getTagsSync (tags=['Package', 'Version', 'Installed-Size', 'Maintainer', 'Architecture']/*:string[]*/, version=null /*:string*/) /*:Object|undefined*/{     
    return bindings.getPackageTags(this.name, tags, version)
  }

  /**
   * Read package information from OS
   *
   * @returns Promise
   * @memberOf Package
   */
  async load () {
    try {
      let result = await this.exec(`dpkg -s ${this.name}`)
      let data = this._parseMetadata(result)
      Object.assign(this, data)
      this.avaialableVersion = this.version
    } catch (e) {
      this.loaded = false
      return this
    }

    try {
      let result = await this.exec(`apt list --upgradable 2>/dev/null | grep ${this.name}`)
      this.avaialableVersion = this._parseUpgradeVersion(result)
      this.hasUpgrades = true
    } catch (e) {
      this.hasUpgrades = false
    }
    this.loaded = true
    return this
  }

  _parseMetadata (output = '') {
    if (!output.trim()) return {}

    let parsed = {}
    let currKey = null
    let currValue = ''

    output = output.split('\n')
    output.forEach((line) => {
      if (line[0] !== ' ') {
        if (currKey) parsed[currKey] = currValue
        line = line.split(':')
        currKey = line.shift().toLowerCase()
        currValue = line.join(':').trim()
      } else if (line === ' .') {
        // The literal " ." indicates a new paragraph
        currValue += '\n\n'
      } else {
        currValue += line.trim()
      }
    })

    if (currKey) parsed[currKey] = currValue
    return parsed
  }

  _parseUpgradeVersion (output = '') {
    output = output.split('\n')
    let firstLine = output[0]
    let search = new RegExp(/\d*\.\d*.\d*-\w*/)
    if (firstLine) return search.exec(output)[0]
    else return this.version
  }
}

module.exports = Package
