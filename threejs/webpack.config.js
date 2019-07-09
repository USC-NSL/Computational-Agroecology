const path = require('path');

module.exports = {
  entry: './js/maincontrol.js',
  output: {
    filename: 'main.js',
    path: path.resolve(__dirname, 'public')
  }
};