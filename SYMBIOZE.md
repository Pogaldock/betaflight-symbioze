# Betaflight — SYMBIOZE OSD Edition

A small, OSD-only patch set on top of upstream Betaflight master, built for
maximum OSD customization within the MAX7456's hard limits (256 glyphs,
12×18 px, white/black/transparent, 30×16 grid). Companion to the Symbioze
OSD Font Editor web app.

Branch: `symbioze-osd` (base: upstream `6aeffc3`). All changes are marked
`SYMBIOZE:` in the source. Nothing outside the OSD/CLI/MSP surface is touched —
flight code is unchanged.

## Features

### 1. OSD Art elements (×6)

Each art element draws a **cols × rows block of consecutive font glyphs** at
any position — custom frames, HUD decorations, images, or the boot logo *in
flight*. Author the block in the font editor, point an art element at it.

```
# element 1: the boot-logo block (0xA0, 24 wide x 4 tall) centered-ish
set osd_art1_glyph = 160
set osd_art1_cols = 24
set osd_art1_rows = 4
set osd_art1_pos = 2243        # x=3, y=6, profile 1  (x | y<<5 | 0x0800)

# element 2: a 4x2 custom image you drew into glyphs 0x77..0x7E
set osd_art2_glyph = 119
set osd_art2_cols = 4
set osd_art2_rows = 2
set osd_art2_pos = 2076        # x=28... clamp to fit
save
```

- Position/profile encoding is the standard `osd_*_pos` scheme (P1 0x0800,
  P2 0x1000, P3 0x2000, variant bits unused).
- Glyph indices wrap mod 256 by design; 0x00 draws as transparent.
- Defaults: every art element starts as the 24×4 logo block at 0xA0.

### 2. Custom messages — 8 slots, fully CLI-controllable

Upstream has 4 message slots, settable only via MSP and positionable only via
the Configurator. This fork:

- **8 slots** (`pilotConfig` v3).
- **CLI text**: `set custom_msg_1 = HELLO WORLD` … `custom_msg_8`.
- **CLI positions**: `set osd_custom_msg1_pos = …` … `osd_custom_msg8_pos`.
- **Raw glyphs from the CLI** via the new command:

  ```
  osdmsg                     # list all 8 slots (raw bytes shown as \xNN)
  osdmsg 3 ARMED AND READY   # plain text (spaces fine)
  osdmsg 4 \xA0\xA1\xA2\xA3  # arbitrary glyph codes - art via message line
  osdmsg 4 -                 # clear slot 4
  save
  ```

- **MSP**: slots 0-3 keep text types 7-10; slots 4-7 use types **12-15**
  (`MSP2TEXT_CUSTOM_MSG_4`); `MSP2_GET_TEXT` can now read messages back
  (upstream is write-only).

### 3. Radio-switched visibility (AUX gating)

14 new modes appear in the Configurator Modes tab: **OSD MSG 1-8** and
**OSD ART 1-6** (permanent IDs 58-71).

- No range assigned → the element behaves exactly as stock (always shown when
  its profile bit is set).
- Range assigned → the element only draws while the switch is active:
  flip-a-switch messages ("LANDING", pit-crew notes, RTL checklists) and
  toggleable HUD art.

## Building

Standard Betaflight build system:

```
make TARGET_LIST                    # see targets
make STM32F405                      # or your target / config
```

Or push the branch to a GitHub fork with Actions enabled — upstream's CI
workflows build target hexes automatically. Flash the hex with Betaflight
Configurator → Firmware Flasher → *Load Firmware [Local]*.

## Compatibility notes

- `pilotConfig` (v2→v3) and `osdConfig` (v13→v14) parameter-group versions are
  bumped: flashing this fork **resets those two groups** (names, messages, OSD
  alarm/art settings — element positions are a separate group and survive).
  Standard Betaflight behavior for config-layout changes; take a `diff all`
  first.
- The added enum entries sit after `OSD_BATTERY_PROFILE_NAME` and before the
  build-conditional tail, so stock Configurators list them as extra/unknown
  OSD elements past the ones they know — positioning them from the CLI (or the
  Symbioze web editor) is the intended path.
- Everything is `#ifdef USE_OSD`-scoped; non-OSD targets are unaffected.
