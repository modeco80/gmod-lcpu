# LCPU Idea/Working Model Sheet

This is basically the working ideas for the LCPU project.

## Core features

- CPU SENTs
	- RISC-V rv32ima core
		- Would supporting more than one CPU core type be worthwhile? If so, the project is relatively setup for such workflow...
	- Controllable paramaters (RAM size, ...)
- Our own frambuffer screen SENT (since wiremod decided to go stupid mode and saw off the gpu)

## Code upload/project workflow

- Upload a raw binary to execute, generated from any user tooling
	- Yes, this means you can run Linux in GMod. No, I'm not sorry.
	- Or even an ELF? Would require less linker hell?

### Integrated simple project workflow

### LLVM integration

- Use LLVM tools (clang as both assembler driver and C/C++ driver) for compilation.
	- Prebuilt toolchain with compiler-rt can be built. (only needed since compiler-rt isn't usually built with most system clang installations.)

- Write assembly/maybe C/C++ code using a tiny project system (data for them would go in server data folder ?)

- No conditional compilation
	- All files in a project are built by that project

- Diagnostic integration (by either using libclang)

- Text editor used to edit project source files

- Some example projects?
	- I joke about it, but an RTOS would be really nice and a good stress test of the project system (for usage in "real" projects.)

## Moderation/administration tools
- Admin controlled per-user max RAM size (default 64mb)
	- possibly override for "respectful" users and admins (admins probably wouldn't even count)?

- Admin controlled per-user max LCPU entity count (default 8)
	- Admins don't count to limits

- Admin controled global (affects all placed LCPUs) scheduler cycle rate.
	- Couldn't be faster than tickrate though or we might block source (and.. well, i dont think i have to explain)
		- I decided not to go with the cpu thread stuff just because its annoying, and would require more state tracking. just ticking in lua using `ENTITY:Think` should be more than good enough (even if theres a risk of hitching source, but I don't think it's that big of a problem...)
		- Project compilations however will definitely end up in a different thread though. Running them in the engine thread would undoubtably cause issues.

## Addon interopability

- Wiremod interopability
	- Wiremod GPIO peripheral (to interface with wire world)
	- special Console Screen peripheral (interfacing specifically with it)
