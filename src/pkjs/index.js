var Clay = require('./config');

Pebble.addEventListener('ready', function() {
  console.log('Flipper watchface JS ready!');
});

Pebble.addEventListener('showConfiguration', function(e) {
  Pebble.openURL(Clay.configUrl());
});

Pebble.addEventListener('webviewclosed', function(e) {
  if (!e || !e.response) {
    return;
  }

  var config = JSON.parse(decodeURIComponent(e.response));
  console.log('Configuration received: ' + JSON.stringify(config));

  Pebble.sendAppMessage(config,
    function() {
      console.log('Config sent to watchface');
    },
    function() {
      console.log('Failed to send config to watchface');
    }
  );
});
