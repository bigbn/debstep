const BaseWrapper = require('./BaseWrapper')

class Package extends BaseWrapper {
  constructor(packageName) {
    super()
    this.name = packageName
    this.avaialableVersion = null
    this.hasUpgrades = false
  }

  async loadMeta() {
    try {
      let result = await this.exec(`dpkg -s ${this.name}`)
      let data = this._parseMetadata(result)
      Object.assign(this, data)
      this.avaialableVersion = this.version
    } catch(e) {
      return this.metaLoaded = false
    }

    try {
      let result = await this.exec(`apt list --upgradable 2>/dev/null | grep ${this.name}`)
      this.avaialableVersion = this._parseUpgradeVersion(result)
      this.hasUpgrades = true
    } catch(e) {
      this.hasUpgrades = false
    }
    return this.metaLoaded = true
  }

  _parseMetadata(output='') {
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

    if (currKey)  parsed[currKey] = currValue  
    return parsed
  }

  _parseUpgradeVersion(output='') {
    output = output.split('\n')
    let firstLine = output[0]
    let search = new RegExp(/\d*\.\d*.\d*-\w*/)
    if (firstLine) return search.exec(output)[0]
    else return this.version;
  }
}

module.exports = Package
