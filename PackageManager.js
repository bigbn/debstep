const BaseWrapper = require('./BaseWrapper');

class PackageManager extends BaseWrapper {
  constructor() {
    super()
  }

  async update() {
    return await this.exec('apt-get update')
  }

  async upgrade() {
    return await this.exec('apt-get upgrade -y')
  }

  async install(packageName) {
    return await this.exec(`apt-get install -y ${packageName}`)
  }

  async uninstall(packageName) {
    return await this.exec(`apt-get remove -y ${packageName}`)
  }

}

module.exports = PackageManager
