# AGENTS.md — Pebble Flipper Zero Watchface

Single Pebble SDK 3 native watchface. Only one source file.

## Commands

```sh
pebble build            # build the watchface
pebble install --phone <IP>   # sideload to watch
pebble kill             # disconnect phone session
```

## Project layout

| Path | Purpose |
|------|---------|
| `src/c/main.c` | Only C source — app entrypoint and all watchface logic |
| `src/pkjs/index.js` | JS companion entrypoint (PebbleKit JS) |
| `resources/` | Fonts (Jersey10, Haxrcorp‑4089), image assets |
| `package.json` | `pebble.*` fields drive resource bundling and SDK config |
| `wscript` | waf build script — no customization needed |

## Architecture

- Pure C Pebble SDK app — no workers, no extra libraries.
- Tick timer fires every minute; time + date rendered in custom fonts over a bitmap.
- No tests, no CI, no lint/typecheck pipeline.
- JS lint (`jshint src/**/*.js`) exists in build script but is **disabled** (`if False`).
- `pebble.projectType: "native"` — this is C, not Simply.js.
