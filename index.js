const Package = require('./Package')

let skatServer = new Package('skat-server-cloud')
let skatAdmin = new Package('skat-maintenance')

let start = async () => {
  await skatServer.loadMeta()
  await skatAdmin.loadMeta()
  if (skatAdmin.metaLoaded) {
    console.log(skatAdmin.version, skatAdmin.avaialableVersion)
  }
}

start()

