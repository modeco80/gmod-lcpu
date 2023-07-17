# LCPU

LCPU is an alternative CPU core addon for GMod/Wiremod.

It provides:

- A standard RISC-V architechure (rv32ima) CPU core
- Extensive interoperation with the Wiremod addon
- A text editor and assembler suite based on LLVM (for writing bare-metal LCPU programs in assembly)

# Building Native Module

TODO: Steps to build the LCPU native module on Windows and Linux


LLVM is built using a podman container (see Containerfile). The steps for replicating this build follow:

```
mkdir llvm_build
podman build -t llvm-build -f Containerfile.llvm
podman run --cpus 16 -m 8G --mount type=bind,source=$PWD/llvm_build,destination=/build/llvm-bin localhost/llvm-build
```


# Installation

This repository is set up in the exact directory structure to be a Legacy Addon; therefore once you have built the native module (TODO), adding it to a server should work fine.
