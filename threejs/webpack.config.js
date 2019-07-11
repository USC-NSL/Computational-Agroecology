const path = require('path');

module.exports = {
  entry: './js/maincontrol.js',
  output: {
    filename: 'bundle.js',
    path: path.resolve(__dirname, 'public')
  }
};