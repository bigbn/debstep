/**
 * apt-cache representation
 * @module aptCache
 * @flow
 **/
import gypBuild from 'node-gyp-build'
const bindings = gypBuild(__dirname)

/**
 * List of tags used in getTags method defaults
 * @const
 * @memberof aptCache
 * @type {string[]}
 */
export const defaultTags = ['Package', 'Version', 'Installed-Size', 'Maintainer', 'Architecture']

/**
 * Extract package meta-information from apt-cache index.
 * Detailed package info
 * @function
 * @memberof aptCache
 * @example
 * import { aptCache } from 'debstep'
 * const meta = aptCache.getTags('vim', ['Installed-Size'], '2:8.1.0875-5')
 * console.log('vim installed size', meta['Installed-Size'])
 * // #_ vim installed size 2800
 */
export function getTagsSync (packageName: string, tags?:string[] = defaultTags, version?:string) : ?{} {
  return bindings.getPackageTags(packageName, tags, version)
}

/**
 * Get current package version
 * @function
 * @memberof aptCache
 * @example
 * import { aptCache } from 'debstep'
 * const currentVersion = aptCache.getCurrentVersionSync('vim')
 * console.log('current version', currentVersion)
 * # #_ current version 2:8.1.0875-4
 */
export function getCurrentVersionSync (packageName: string) : ?string {
  return bindings.getPackageCurrentVersion(packageName)
}

/**
 * Get latest package version to be upgraded
 * @function
 * @memberof aptCache
 * @example
 * import { aptCache } from 'debstep'
 * const availableVersion = aptCache.getCandidateVersionSync('vim')
 * console.log('new version', availableVersion)
 * // #_ new version 2:8.1.0875-5
 */
export function getCandidateVersionSync (packageName: string) : ?string {
  return bindings.getPackageCandidateVersion(packageName)
}

/**
 * Check if package has upgrades
 * @function
 * @memberof aptCache
 * @example
 * import { aptCache } from 'debstep'
 * const updateAvailable = aptCache.hasUpgradesSync('vim')
 * console.log('vim has a new version to upgrade:', updateAvailable)
 * // #_ vim has a new version to upgrade: false
 */
export function hasUpgradesSync (packageName: string) : boolean {
  const candidateVersion = getCandidateVersionSync(packageName)
  if (!candidateVersion) return false

  const currentVersion = getCurrentVersionSync(packageName)
  if (candidateVersion !== currentVersion) return true
  else return false
}
