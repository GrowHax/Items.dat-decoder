# items.dat decoder by Yuhkil

A small command-line tool and library for decoding Growtopia `items.dat` to JSON. Made in C99.
You can open a ticket in `Issues` if any problems occur. 
(bugs may occur since this is a completely refactored old project / syntax might be different)

You can either snatch the decoded items.dat from here [Releases](https://github.com/GrowHax/Items.dat-decoder/releases), or continue reading.

**REQUIRED**
- Place the compatible `items.dat` that you want to decode within the root folder.

**Prerequisites**
- Windows: PowerShell and a supported toolchain (MSYS2/MinGW or Visual Studio) to build from source.
- Linux/macOS: `gcc` and `make`.

**Build**
Windows (PowerShell):
```
.\build.bat
```

Unix (Make):
```
make
```

**Usage**
- Decode `items.dat` to JSON (writes to stdout):
```
.\itemsdat_decoder.exe --json items.dat > items.json
```

- Print help:
```
.\itemsdat_decoder.exe --help
```

**Version changes**
### `version 11`
- **New Field:** `punchOptions` (string)

### `version 12`
- **New Fields:**
  - `flags3` (int32_t)
  - `bodyPart[9]` (uint8_t array)

### `version 13`
- **New Field:** `lightRange` (int32_t)

### `version 14`
- **New Field:** `unknownInt3` (int32_t)

### `version 15`
- **New Fields:** Chair/player functionality?
  - `canSit` (uint8_t)
  - `playerOffsetX` (int32_t)
  - `playerOffsetY` (int32_t)
  - `chairTextureX` (int32_t)
  - `chairTextureY` (int32_t)
  - `chairLegOffsetX` (int32_t)
  - `chairLegOffsetY` (int32_t)
  - `chairTexture` (string)

### `version 16`
- **New Field:** `itemRenderer` (string)

### `version 17`
- **New Field:** `extraFlags1` (int32_t)

### `version 18`
- **New Field:** `itemRendererHash` (int32_t)

### `version 19`
- **New Field:** `unknownBytes2[9]` (uint8_t array)

### `version 21`
- **New Field:** `unknownShort1` (int16_t)

### `version 22`
- **New Field:** `info` (string)

### `version 23`
- **New Fields:** Recipe system
  - `recipe[0]` (uint16_t)
  - `recipe[1]` (uint16_t)

### `version 24`
- **New Field:** `unknownBytes3` (uint8_t)

**License**
- See `LICENSE` for license details.
