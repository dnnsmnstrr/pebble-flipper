# Flipper Zero Watchface

A Pebble watchface themed after the Flipper Zero device.

## Features

- Time display (12/24h auto) with custom bitmap font
- Date display (day, month abbreviation, day number)
- Flipper Zero silhouette bitmap

## Build

Requires the [Pebble SDK](https://developer.rebble.io/).

```sh
pebble build
pebble install --emulator emery 
# or
pebble install --phone <IP>

```

## Project

Single-file C app using Pebble SDK 3. Custom fonts (Haxrcorp-4089, Jersey10) and a Flipper Zero image are bundled as resources.

## License

MIT
