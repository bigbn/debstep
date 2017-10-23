const exec = require('child_process').exec;

class BaseWrapper {
  exec(cmd) {
    return new Promise((resolve, reject) => {
      exec(cmd, (error, stdout, stderr) => {
        if (error) return reject(error)
        if (stderr) return resolve(stderr)
        resolve(stdout)
      })
    })
  }
}

module.exports = BaseWrapper
