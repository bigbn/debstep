# debstep

Control apt, dpkg tasks via node js
Currently supports only reading packages info

# Example 

    let package = new debstep.Package('vim')
    await package.load()
    console.log(package)

    Package {
      loaded: true,
      name: 'vim',
      avaialableVersion: '2:7.4.1689-3ubuntu1.2',
      hasUpgrades: false,
      package: 'vim',
      status: 'install ok installed',
      priority: 'optional',
      section: 'editors',
      'installed-size': '2400',
      maintainer: 'Ubuntu Developers <ubuntu-devel-discuss@lists.ubuntu.com>',
      architecture: 'amd64',
      version: '2:7.4.1689-3ubuntu1.2',
      provides: 'editor',
      depends: 'vim-common (= 2:7.4.1689-3ubuntu1.2), vim-runtime (= 2:7.4.1689-3ubuntu1.2), libacl1 (>= 2.2.51-8), libc6 (>= 2.15), libgpm2 (>= 1.20.4), libpython3.5 (>= 3.5.0~b1), libselinux1 (>= 1.32), libtinfo5 (>= 6)',
      suggests: 'ctags, vim-doc, vim-scripts',
      description: 'Vi IMproved - enhanced vi editorVim is an almost compatible version of the UNIX editor Vi.\n\nMany new features have been added: multi level undo, syntaxhighlighting, command line history, on-line help, filenamecompletion, block operations, folding, Unicode support, etc.\n\nThis package contains a version of vim compiled with a ratherstandard set of features.  This package does not provide a GUIversion of Vim.  See the other vim-* packages if you need more(or less).',
      homepage: 'http://www.vim.org/',
      'original-maintainer': 'Debian Vim Maintainers <pkg-vim-maintainers@lists.alioth.debian.org>' }