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
        label: "Flipper color",
        defaultValue: "0",
        options: [
          { label: "White", value: "0" },
          { label: "Orange", value: "1" },
          { label: "Black", value: "2" },
        ],
      },
    ],
  },
];
