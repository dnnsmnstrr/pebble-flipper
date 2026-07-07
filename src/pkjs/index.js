Pebble.addEventListener('ready', function() {
  console.log('Flipper watchface JS ready!');
});

Pebble.addEventListener('showConfiguration', function(e) {
  var current = localStorage.getItem('image_color') || '0';
  var url = 'data:text/html;charset=utf-8,' + encodeURIComponent(
    '<!DOCTYPE html>' +
    '<html>' +
    '<head>' +
    '<meta charset="utf-8">' +
    '<meta name="viewport" content="width=device-width, initial-scale=1">' +
    '<title>Flipper Settings</title>' +
    '<style>' +
    'body{font-family:-apple-system,Helvetica,sans-serif;padding:20px;background:#1a1a2e;color:#eee;margin:0}' +
    'h2{color:#e94560;text-align:center;margin-top:0}' +
    'label{display:block;margin:12px 0;padding:16px;background:#16213e;border-radius:10px;cursor:pointer}' +
    'input{margin-right:10px}' +
    '.save{width:100%;padding:14px;background:#e94560;color:#fff;border:none;border-radius:10px;font-size:18px;font-weight:bold;cursor:pointer;margin-top:10px}' +
    '.desc{font-size:12px;color:#888;margin:4px 0 0 26px}' +
    '</style>' +
    '</head>' +
    '<body>' +
    '<h2>Flipper Color</h2>' +
    '<form id="f">' +
    '<label><input type="radio" name="color" value="0"' + (current === '0' ? ' checked' : '') + '> White <div class="desc">White flipper on black background</div></label>' +
    '<label><input type="radio" name="color" value="1"' + (current === '1' ? ' checked' : '') + '> Orange <div class="desc">Orange flipper on black background</div></label>' +
    '<label><input type="radio" name="color" value="2"' + (current === '2' ? ' checked' : '') + '> Black <div class="desc">Black flipper on white background</div></label>' +
    '</form>' +
    '<button class="save" onclick="save()">Save</button>' +
    '<script>' +
    'function save(){var f=document.getElementById("f");var val=f.querySelector("input[name=color]:checked").value;document.location="pebblejs://close#"+encodeURIComponent(JSON.stringify({"0":parseInt(val,10)}))}' +
    '<\/script>' +
    '</body>' +
    '</html>'
  );
  Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
  if (e.response) {
    var config = JSON.parse(decodeURIComponent(e.response));
    localStorage.setItem('image_color', String(config["0"]));
    Pebble.sendAppMessage({IMAGE_COLOR: parseInt(config["0"], 10)},
      function() { console.log('Config sent to watchface'); },
      function() { console.log('Failed to send config to watchface'); }
    );
  }
});
