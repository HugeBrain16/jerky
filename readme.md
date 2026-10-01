# Silent Hill 3 - jerky.asi

Makes beef jerky usable as heal item.

*Requires [**Ultimate ASI Loader**](https://github.com/ThirteenAG/Ultimate-ASI-Loader)

Building:
1. Install mingw-w64 toolchain and make sure i686-w64-mingw32-gcc is in PATH
2. run `./build`, it will generate jerky.asi file if build succeeds

Installation:

(You can get a pre-built jerky.asi file from [Releases](../../releases) if you are unable to build it yourself)

1. Create scripts folder in `Silent Hill 3\scripts`
2. Move jerky.asi into scripts
  
Config is loaded from file "Silent Hill 3\scripts\jerky.cfg".

- `heal = <number>`
amount of hp to add

- `preserve_behavior = <bool>`
keep original behavior of placing down the item while also adding hp