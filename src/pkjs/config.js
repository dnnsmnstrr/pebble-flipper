var Clay = require('@rebble/clay');

var clayConfig = {
  "enabled": true,
  "content": [
    {
      "type": "heading",
      "defaultValue": "Flipper Settings"
    },
    {
      "type": "section",
      "items": [
        {
          "type": "radiogroup",
          "messageKey": "IMAGE_COLOR",
          "label": "Flipper color",
          "defaultValue": "0",
          "options": [
            { "label": "White", "value": "0" },
            { "label": "Orange", "value": "1" },
            { "label": "Black", "value": "2" }
          ]
        }
      ]
    }
  ]
};

var clay = new Clay(clayConfig, null, null);

module.exports = clay;
