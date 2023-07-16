# riscv

This is the RISC-V emulation core that LCPU uses in its native emulation module.

This is based off [cnlohr/mini-rv32ima](https://github.com/cnlohr/mini-rv32ima), but:

- Rewritten in C++20 (because I like sanity)
- Cleaned up somewhat
- Moved *ALL* device and MMIO code to seperate interfaces
	- Re-implemented the timer device and the UART as said oop interface
		- Lua devices use a wrapper which can contain lua callbacks
