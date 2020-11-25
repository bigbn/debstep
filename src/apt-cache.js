/**
 * apt-cache representation
 **/

const bindings = require('node-gyp-build')(__dirname)

function getTagsSync (packageName, tags=['Package', 'Version', 'Installed-Size', 'Maintainer', 'Architecture']/*:string[]*/, version=null /*:string*/) /*:Object|undefined*/{
  return bindings.getPackageTags(packageName, tags, version)
}

function getCandidateVersionSync(packageName) {
  return bindings.getPackageCandidateVersion(packageName)
}

function getCurrentVersionSync(packageName) {
  return bindings.getPackageCurrentVersion(packageName)
}

function hasUpgradesSync(packageName) {
  const candidateVersion = getCandidateVersionSync(packageName)
  if (!candidateVersion) return false
  
  const currentVersion = getCurrentVersion(packageName)
  if (candidateVersion !== currentVersion) return true
  else return false
}

module.exports = Package
