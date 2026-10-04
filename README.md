# Smart Training SKSE

Unused training sessions carry over between levels. The mod uses native game data, works with existing saves, and keeps your configured per-level allowance as the base.

To change the number of training sessions per level, edit the vanilla game setting `iTrainingNumAllowedPerLevel` using SSEEdit or a mod such as Game Settings Override. Changing this setting requires a full game restart.

One DLL for SE, AE, and VR, built with [alandtse's CommonLibSSE-NG](https://github.com/alandtse/CommonLibSSE-NG). No ESP or Papyrus scripts.

## Requirements

[SKSE](https://www.nexusmods.com/skyrimspecialedition/mods/30379) and [Address Library](https://www.nexusmods.com/skyrimspecialedition/mods/32444) for your game runtime. Install the release archive through your mod manager.

## Build

Requires Visual Studio 2022 C++ tools, CMake, Ninja, and vcpkg. From this directory, in an x64 Native Tools Command Prompt with `VCPKG_ROOT` pointing to vcpkg:

```sh
cmake --preset release
cmake --build build/release
```

The DLL is copied to `contrib/Distribution/PluginRelease/`.

## Credits

- [SkyrimScripting](https://github.com/SkyrimScripting/SKSE_Template_HelloWorld) — project template.
- [alandtse and the CommonLibSSE-NG contributors](https://github.com/alandtse/CommonLibSSE-NG) — CommonLibSSE-NG.
- [SKSE Team](https://www.nexusmods.com/skyrimspecialedition/mods/30379) — SKSE.
- [meh321](https://www.nexusmods.com/skyrimspecialedition/mods/32444) — Address Library.

## License

[GPL-3.0-or-later](LICENSE) with the exceptions in [EXCEPTIONS.md](EXCEPTIONS.md).
