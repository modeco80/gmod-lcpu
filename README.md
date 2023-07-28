# LCPU

LCPU is an alternative CPU core addon for Garry's Mod and Wiremod.

It provides:

- A standard RISC-V architechure (rv32ima) CPU core, implemented in native code
	- I know, but it wouldn't particularly be very good if I managed to implement it in Lua
		(that and the more reasons to not use Lua for a part of this codebase, the better...)
- Interoperation with the Wiremod addon (and addons which implement Wiremod integration)

This addon (for now) works with both the non-beta branch and the x86-64 beta branches of GMod.

Note that the GitHub mirror is provided only for convinence, and is not used for active development. 
See [this link](https://git.crustywindo.ws/modeco80/gmod-lcpu) for the actual development repository.

# Installation

You will need Wiremod installed, either from the Workshop or cloned as a filesystem addon.

This repository is set up to be a Filesystem Addon; therefore, workflows which clone repositories from Git and put them in addons/ should be able to work with the LCPU addon just fine.

Preliminary installation steps (by hand):

```
garrysmod/addons$ git clone --recursive https://git.crustywindo.ws/modeco80/gmod-lcpu.git lcpu
garrysmod/addons$ cd lcpu

# Build the LCPU native module. These steps build the linux64 version of the module
# on linux; you'll need to alter it if you want to build for 32-bit.
garrysmod/addons/lcpu$ cmake -Wno-dev -GNinja -S native -B build \
	-DCMAKE_BUILD_TYPE=Release
garrysmod/addons/lcpu$ ninja -C build

# Install the native module (Linux)
# For Windows you can do the same thing, just replace this
# with how you'd do it in batch (or use Explorer, I'm not your dad)
garrysmod/addons/lcpu$ [[ ! -d '../../lua/bin']] && mkdir -p ../../lua/bin && cp build/projects/lcpu/*.dll ../../lua/bin

# Tada!
```

On Linux you can alternatively use the `./build_module.sh` script that will do all the build and installation steps automatically, once cloning the repository in the garrysmod/addons folder.

Windows building is currently untested; I see no reason why it wouldn't work.


# Special Thanks

[CNLohr](https://github.com/CNLohr) - I may not like the mini-rv32ima code that much, but it did still help a lot (and my emulation core library is based off it, with tons of refactoring and a C++ rewrite to boot.), so thanks for creating the project.
