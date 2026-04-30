# Mischief Makers
[![Match Status](https://img.shields.io/badge/matched-3.37-brightgreen.svg)]()
[![Decomp Status](https://img.shields.io/badge/decompiled-6.53-yellow.svg)]()

A in-progress decompilation of Mischief Makers (or Yuke-Yuke!! Trouble Makers, ゆけゆけ!!トラブルメーカーズ, Yuke Yuke!! Toraburu Mēkāzu in Japanese.)

This project currently supports the 1.1 release of the US version, with very minor support for the other versions of the game.

## Building (Linux)

### Install build dependencies
The build process has the following package requirements:
- git
- build-essential
- binutils-mips-linux-gnu
- gcc-mips-linux-gnu
- python3
- python3-venv
- ninja-build
- cargo

Under a Debian based distribution, you can install these with the following commands:
```
sudo apt update
sudo apt install git build-essential binutils-mips-linux-gnu gcc-mips-linux-gnu python3 python3-venv ninja-build cargo
```

### Clone the repository
Clone `https://github.com/Drahsid/mischief-makers.git` in whatever directory you wish.
```
git clone https://github.com/Drahsid/mischief-makers.git --recursive
```

For an existing checkout, synchronize and initialize the submodules before building:
```
git submodule sync --recursive
git submodule update --init --recursive
```

### Install Python requirements
Create a project venv and install the pinned Python requirements into it:
```
python3 -m venv .venv
.venv/bin/python -m pip install --upgrade pip
.venv/bin/python -m pip install -r requirements.txt
```

Run `configure.py` through this venv Python.

### Install Slinky
The build uses Slinky to generate the final linker script from the split linker entries. Install `slinky-cli` and ensure it is on `PATH`:
```
cargo install --git https://github.com/decompals/slinky --locked slinky-cli
slinky-cli --version
```

If `slinky-cli` is not on `PATH`, set `SLINKY` when configuring:
```
SLINKY=/path/to/slinky-cli .venv/bin/python configure.py --split
```

### Prepare the base rom
Copy over your copy of Mischief Makers 1.1 US into the root folder of this repository. Rename the rom to baserom.us1.z64.

For first-time setup, run the following command to download and extract the compiler:
```
.venv/bin/python configure.py --setup
```

To extract, disassemble, generate the build script, and build the ROM:
```
.venv/bin/python configure.py --split --build
```
The split step writes `build/mischiefmakers.slinky.yaml` and uses Slinky to generate `versions/us1/mischiefmakers.ld`.

## Build the rom
Just run `ninja`, or alternatively, `./configure.py -sb`. A folder named 'build' will be produced, inside this will be the output rom `mischiefmakers.z64`. If the rom matches, you should see the following output in the console:
```
source: 811709c6473f74cbf283aa61f056e1de3046ec73; target: 811709c6473f74cbf283aa61f056e1de3046ec73
OK!
```

## Other configure options
Use the venv Python for configure commands:
```
.venv/bin/python configure.py --clean
.venv/bin/python configure.py --fullclean
.venv/bin/python configure.py --split
.venv/bin/python configure.py --symbols
```

## Contributing
Contributions are welcome. If you would like to reserve a function, open a PR with the function or file name(s).
If you plan to contribute, be sure to take a look at the [style guide](https://github.com/Drahsid/mischief-makers/wiki/Style-Guide).
