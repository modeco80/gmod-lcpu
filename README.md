# LCPU

LCPU is an alternative CPU core addon for GMod/Wiremod.

It provides:

- A standard RISC-V architechure (rv32ima) CPU core
- Extensive interoperation with the Wiremod addon
- A text editor and assembler suite based on LLVM (for writing bare-metal LCPU programs in assembly)

# Building Native Module

TODO: Steps to build the LCPU native module on Windows and Linux


LLVM is built using a podman container (see Containerfile). The steps for building follow (this should only need to be done once):

```
mkdir llvm_build
podman build -t llvm-build -f Containerfile.llvm
podman run --rm --mount type=bind,source=$PWD/llvm_build,destination=/build/llvm-build localhost/llvm-build
cp -rv llvm_build/install lua/bin/llvm
```


# Installation

This repository is set up in the exact directory structure to be a Legacy Addon; therefore once you have built the native module (TODO), adding it to a server should work fine.
