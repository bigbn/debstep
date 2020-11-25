import { basename, resolve } from 'path'
import { copy, writeFile } from 'fs-extra'

async function copyFile (file) {
  const fileName = basename(file)
  const filePath = resolve(__dirname, '../lib/', fileName)

  await copy(file, filePath)

  console.log(`Copied ${file} to ${filePath}`)
}

async function createPackageFile () {
  const oldPckgPath = resolve(__dirname, '../package.json')
  const oldPckgData = require(oldPckgPath)

  delete oldPckgData.private
  delete oldPckgData.scripts
  delete oldPckgData.devDependencies

  const newPckgPath = resolve(__dirname, '../lib/package.json')
  const newPckgData = Object.assign(oldPckgData, { main: './index.js' })
  await writeFile(newPckgPath, JSON.stringify(newPckgData), 'utf8')

  console.log(`Created package.json in ${newPckgPath}`)
}

async function run () {
  await ['README.md', 'LICENSE', 'binding.gyp', 'bindings.cpp'].map(file => copyFile(file))
  await createPackageFile()
}

run()
