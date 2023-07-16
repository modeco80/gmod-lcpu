- cpu entities
	- controllable paramaters (ram size, ...)
		- admin controlled per-user max RAM size (default 32mb)
			- possibly override for "respectful" users and admins (admins probably wouldn't even count)?
		- admin controlled per-user max lcpu chip count (default 8)
			- admins can override/do not count?

	- admin controled global (affects all placed lcpus) scheduler timeslice.
		- couldn't be faster than tickrate though or we might block source (and.. well, i dont think i have to explain)

	- upload a raw binary (or elf? we could probably just imitate a "boot loader" and throw it into memory and move the given register to it?) to execute
		- or write assembly/maybe c code for tiny projects

	- text editor for lcpu projects

	- some example projects?
	
	- riscv rv32-ima cpu core
		- llvm assembler used in this case
		- maybe clang integration? (this might make the addon unwieldly though)

- wiremod interopability (to obsolete ZCPU at least for my purposes)
	- gpio (which uses normal wire stuff)
	- console screen
	- gpu (if it's not painful to do so)?
