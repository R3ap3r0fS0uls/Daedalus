# What The KeepLoadout Mod Does
Keep in mind, the mod only works in offline mode, due to how the mod edits your local save file. 
* This mod allows you to bring any item up from prospect/outpost and bring it into orbit.

# Installation Instructions
* At the moment you'll have to compile the mod and mod loader yourself.
* Install Visual Studio and open the DaedalusLoader folder and then open Daedalus.sln in visual studio.
* Click the green arrow at the top of the visual studio window, then repeat the process for the rest of the projects in the Soultion explorer on the right.  Also, disregard the error related to not being able to open dll.
* Now open DaedalusLoader/x64/Release in file explorer, and copy the contents into (Icarus-Root)/Icarus/Binaries/Win64.
* Then, in that same folder, create a folder and call it "mods", then copy KeepLoadout.dll from the 64x folder into it.
* Now you should be abe to start the game and see "Core Mod Created: KeepLoadout", in the DaedalusLoader console window.

# Daedalus
A modloader for [Icarus](https://store.steampowered.com/app/1149460/ICARUS/) for blueprint and
C++ based mods. Still heavily under construction, see below for proper accredidation and info.

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
