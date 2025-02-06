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
- ninja-build

Under a Debian based distribution, you can install these with the following commands:
```
sudo apt update
sudo apt install git build-essential binutils-mips-linux-gnu gcc-mips-linux-gnu python3 ninja-build
```

### Clone the repository
Clone `https://github.com/Drahsid/mischief-makers.git` in whatever directory you wish.
```
git clone https://github.com/Drahsid/mischief-makers.git --recursive
```

### Install Python3 requirements
In the root directory, run `pip3 install -U -r requirements.txt`

Subsequently, Navigate to `tools/splat/` and run `pip3 install -U -r requirements.txt`

### Prepare the base rom
Copy over your copy of Mischief Makers 1.1 US into the root folder of this repository. Rename the rom to baserom.us1.z64.

For first-time setup, run `./configure.py --setup` to download the compiler.
To extract and disassemble the rom, run `./configure.py -s`

## Build the rom
Just run `ninja`, or alternatively, `./configure.py -sb`. A folder named 'build' will be produced, inside this will be the output rom `mischiefmakers.z64`. If the rom matches, you should see the following output in the console:
```
source: 811709c6473f74cbf283aa61f056e1de3046ec73; target: 811709c6473f74cbf283aa61f056e1de3046ec73
OK!
```

## Other configure optons
You can use `-c` (clean) and `-fc` (full clean) to clean the build, extraction, and dissasemmbly output.

## Contributing
Contributions are welcome. If you would like to reserve a function, open a PR with the function or file name(s).
If you plan to contribute, be sure to take a look at the [style guide](https://github.com/Drahsid/mischief-makers/wiki/Style-Guide).

