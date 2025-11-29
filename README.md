# items.dat decoder by Yuhkil

A small command-line tool and library for decoding Growtopia `items.dat` to JSON. Made in C99.
You can open a ticket in `Issues` if any problems occur. 
(bugs may occur since this is a completely refactored old project / syntax might be different)

**REQUIRED**
- Place the compatible `items.dat‘ that you want to decode within the root folder.

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

**Version changes (might be wrong but still alligned)**
### `version 11`
- **New Field:** `punch_option`
  - **Description:** A new string field named `punch_option` was introduced.

### `version 12`
- **Data Skipping:** 
  - **Description:** An additional 13 bytes of data are skipped in this version.

### `version 13`
- **Data Skipping:** 
  - **Description:** An additional 4 bytes of data are skipped in this version.

### `version 14`
- **Data Skipping:** 
  - **Description:** An additional 4 bytes of data are skipped in this version.

### `version 15`
- **Data Handling:**
  - **Description:** An additional 25 bytes of data are skipped, followed by reading a new string.

### `version 16`
- **Data Handling:**
  - **Description:** A new string is read in this version.

### `version 17`
- **Data Skipping:**
  - **Description:** An additional 4 bytes of data are skipped in this version.

### `version 18`
- **Data Skipping:**
  - **Description:** An additional 4 bytes of data are skipped in this version.

### `version 19`
- **Data Handling:**
  - **Description:** An additional 9 bytes of unknown data are read in this version.

### `version 21`
- **Data Handling:**
  - **Description:** An additional 2 bytes of unknown data are read in this version.

### `version 22`
- **Data Handling:**
  - **Description:** A new string field for item info is read in this version.

### `version 23`
- **Data Handling:**
  - **Description:** Two additional 2-byte recipe values are read in this version.

**License**
- See `LICENSE` for license details.
