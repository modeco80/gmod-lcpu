# LCPU

LCPU is an alternative CPU core addon for Garry's Mod and Wiremod.

It provides:

- A standard RISC-V architechure (rv32ima) CPU core, implemented in native code
	- I know, but it wouldn't particularly be very good if I managed to implement it in Lua
		(that and the more reasons to not use Lua for a part of this codebase, the better...)
- Interoperation with the Wiremod addon (and addons which implement Wiremod integration)

This addon works with both the non-beta branch and the x86-64 beta branches of GMod, however rigorous testing is usually only done with the x86-64 beta branch.

Note that the GitHub mirror is provided only for convinence, and is not used for active development. 
See [this link](https://git.crustywindo.ws/modeco80/gmod-lcpu) for the actual development repository.

# Installation

You will need Wiremod installed, either from the Workshop or cloned as a filesystem addon.

This repository is set up to be a Filesystem Addon; therefore, workflows which clone repositories from Git and put them in addons/ should be able to work with the LCPU addon just fine.

Preliminary installation steps (for Linux):

```
garrysmod/addons$ git clone --recursive https://git.crustywindo.ws/modeco80/gmod-lcpu.git lcpu
garrysmod/addons$ cd lcpu
garrysmod/addons/lcpu$ ./build_module.sh
# Tada!
```

Windows building is currently untested; I see no reason why it wouldn't work.


# Special Thanks

[CNLohr](https://github.com/CNLohr) - I may not like the mini-rv32ima code that much, but it did still help a lot (and my emulation core library is based off it, with tons of refactoring and a C++ rewrite to boot.), so thanks for creating the project.
