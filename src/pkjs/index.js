var Clay = require('@rebble/clay');
var clayConfig = require('./config');
var clay = new Clay(clayConfig);

Pebble.addEventListener('ready', function() {
  console.log('Flipper watchface JS ready!');
});

Pebble.addEventListener('showConfiguration', function() {
  Pebble.openURL(clay.generateUrl());
});

Pebble.addEventListener('webviewclosed', function(e) {
  if (!e || !e.response) {
    return;
  }

  var settings = clay.getSettings(e.response);
  Pebble.sendAppMessage(settings, function() {
    console.log('Settings sent to Pebble');
  }, function(error) {
    console.log('Settings send failed: ' + JSON.stringify(error));
  });
});
