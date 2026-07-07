module.exports = [
  {
    type: "heading",
    defaultValue: "Flipper Settings",
  },
  {
    type: "section",
    items: [
      {
        type: "radiogroup",
        messageKey: "IMAGE_COLOR",
        label: "Theme",
        defaultValue: 0,
        options: [
            { label: "Light", value: 0 },
            { label: "Dark", value: 1 },
            { label: "Orange Light", value: 2 },
            { label: "Orange Dark", value: 3 },
        ],
      },
    ],
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];
