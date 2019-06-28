const path = require('path');

module.exports = {
  entry: './js/client.js',
  output: {
    filename: 'main.js',
    path: path.resolve(__dirname, 'public')
  }
};