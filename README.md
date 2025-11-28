# Archive of decoded items.dat

## Information

The `items.dat` file format has evolved through various versions, with changes in data structure and additional fields being introduced over time. Below is a summary of the modifications made to the data structure for each version (till recent update as of now):

# items.dat-decoder

A small command-line tool and library for decoding Growtopia `items.dat` to `items.json`.

**Features**
- Parse `items.dat` across multiple historical versions.
- Read and write (encode/decode) item definitions.
- Defensive parsing with length checks to avoid crashes on malformed input.

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

**License**
- See `LICENSE` for license details.
