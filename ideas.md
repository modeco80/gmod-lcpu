# LCPU Idea/Working Model Sheet

This is basically the working ideas for the LCPU project.

## Core features

- CPU SENTs
	- RISC-V rv32ima core
		- Would supporting more than one CPU core type be worthwhile? If so, the project is relatively setup for such workflow...
	- Controllable paramaters (RAM size, ...)

## Code upload/project workflow

- Upload a raw binary to execute, generated from any user tooling
	- Yes, this means you can run Linux in GMod. No, I'm not sorry.
	- Or even an ELF? Would require less linker hell?

### Integrated simple project workflow

- Write assembly/maybe C code using a tiny project system (data for them would go in server data folder ?)
	- LLVM assembler is used
		- Integrated clang for C code? (if it's not prohibitively expensive)
		- In all cases (as/clang), the tools will all be firewalled to the given project data directory.
			In the case of clang, an addional path will be allowed (clang freestanding headers), but that's the only exception. 
	
- Text editor used to edit project source files

- Some example projects?
	- I joke about it, but an RTOS would be really nice and a good stress test of the project system (for usage in "real" projects.)

## Moderation/administration tools
- Admin controlled per-user max RAM size (default 64mb)
	- possibly override for "respectful" users and admins (admins probably wouldn't even count)?

- Admin controlled per-user max LCPU entity count (default 8)
	- admins can override/do not count?

- Admin controled global (affects all placed LCPUs) scheduler timeslice.
	- Couldn't be faster than tickrate though or we might block source (and.. well, i dont think i have to explain)
		- I decided not to go with the cpu thread stuff just because its annoying, and would require more state tracking. just ticking in lua using `ENTITY:Think` should be more than good enough (even if theres a risk of hitching source, but I don't think it's that big of a problem...)
		- Project compilations however will definitely end up in a different thread though. Running them in the engine thread would undoubtably cause issues.

## Addon interopability

- Wiremod interopability
	- Wiremod GPIO (which uses normal wire stuff)
	- Console Screen
	- Wire GPU (if it's not painful to do so)?
