# Hogwarts Legacy Fix
[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/W7W01UAI9)</br>
[![Github All Releases](https://img.shields.io/github/downloads/Lyall/HogwartsLegacyFix/total.svg)](https://github.com/Lyall/HogwartsLegacyFix/releases)

This is a fix for pillarboxing in cutscenes with ultrawide/wider displays in Hogwarts Legacy.

## Features
- Removes pillarboxing in cutscenes.

## Installation
- Grab the latest release of HogwartsLegacyFix from [here.](https://github.com/Lyall/HogwartsLegacyFix/releases)
- Extract the contents of the release zip in to the game directory.<br />(e.g. "**steamapps\common\Hogwarts Legacy**" for Steam).

## Configuration
- See **HogwartsLegacyFix.ini** to adjust settings for the fix.

## Linux/Steam Deck
- Make sure you set the Steam launch options to `WINEDLLOVERRIDES="version.dll=n,b" %command%`.
![image](https://user-images.githubusercontent.com/695941/218338901-b65546d0-316d-4b46-a6b4-aa7ef9a1ed98.png)


## Known Issues
Please report any issues you see.

- **Game doesn't launch at all with the fix installed**<br />
Try renaming `version.dll` to one of the options from [this list.](https://github.com/ThirteenAG/Ultimate-ASI-Loader#description) For example, `d3d12.dll`. ([#3](https://github.com/Lyall/HogwartsLegacyFix/issues/3#issuecomment-1427009944))

## Screenshots

| ![ezgif-5-aa9dbbde3b](https://user-images.githubusercontent.com/695941/217569024-242b3e90-0c66-46de-9460-6e31eb476f5d.gif) |
|:--:|
| Disabled pillarboxing in cutscenes. |

## Credits
[Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader) for ASI loading. <br />
[inih](https://github.com/jtilly/inih) for ini reading. <br />
[Loguru](https://github.com/emilk/loguru) for logging. <br />
[length-disassembler](https://github.com/Nomade040/length-disassembler) for length disassembly.
