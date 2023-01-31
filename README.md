# Installation Instructions
1. Extract V1.2.zip and copy the contents into (Icarus-Root)/Icarus/Binaries/Win64;
2. Then, in that same folder, create a folder and call it "mods", this is where you'll put .dll mods (Not xinput.dll or daedalus.dll).
3. This isn't releated to this mod loader, however some of my mods rely on this.  Create a folder and call it "mods" in (Icarus-Root)/Icarus/Content/Paks.  This is where all mods that have a .pak file extention will go.

# Notes
* The mod loader is not currently supported in dx12, so when starting Icarus, select dx11.

# Credit
This project was originally a fork of an old version of 
[SatisfactoryModLoader](https://github.com/satisfactorymodding/SatisfactoryModLoader) but has since
transitioned to using a fork of [UnrealModLoader](https://github.com/RussellJerome/UnrealModLoader).

Most credit should go to [RusselJerome](https://github.com/RussellJerome) for his UnrealModLoader project
(seriously awesome software), and the amazing reverse engineers behind [UnrealDumper](https://github.com/guttir14/UnrealDumper-4.25)
who have made profiles for which I've pulled offsets.

# 3rd Party Software
  * [feather-ini-parser](https://github.com/Turbine1991/cpp-feather-ini-parser)
  * [Minhook](https://github.com/TsudaKageyu/minhook)
  * [ImGui](https://github.com/ocornut/imgui)
  * [JsonCPP](https://jsoncpp.docsforge.com)
