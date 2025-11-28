# Archive of decoded items.dat

## Information

The `items.dat` file format has evolved through various versions, with changes in data structure and additional fields being introduced over time. Below is a summary of the modifications made to the data structure for each version:

# items.dat-decoder

A small command-line tool and library for decoding Growtopia `items.dat` files into JSON and for working with the items schema.

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

**Repository layout**
- `include/` — public headers
- `src/` — implementation files
- `Makefile`, `build.bat`, `build_and_run.bat` — build helpers

**Notes**
- The code supports many optional fields and version-gated branches; removing support for older versions will reduce code size but will break compatibility with older `items.dat` files.
- Do not check compiled artifacts (binaries, object files) into the repository.

**Contributing**
- PRs welcome. Run the build and include tests where appropriate.

**License**
- See `LICENSE` for license details.
