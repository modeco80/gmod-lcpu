- cpu cores
	- controllable paramaters (ram size, ...)
		- admin controlled per-user max RAM size (default 16mb)
			- possibly override for "respectful" users and admins?
		- admin controlled per-user max lcpu chip count (default 4)
			- admins can override?
	- admin controled global (affects all placed lcpus) scheduler timeslice (? cpus wouldn't block source anyways, the only way they could is if they start posting too many cpu thread -> lua thread callbacks ?)
	- upload a raw binary (or elf? we could probably just imitate a "boot loader" and throw it into memory and move the given register to it?) to execute
		- or write code as a "lcpu project." in theory exporting out to a Makefile/etc should be possible
	- text editor for lcpu projects
	
	- supported architechures (iniitally. more could be added and a "common" abstraction framework for devices could be agreed on)
		- riscv rv32-ima cpu core
			- llvm assembler/maybe clang integration? (this might make the addon unwieldly though)

- wiremod interopability (to obsolete ZCPU at least for my purposes)
	- gpio (which uses normal wire stuff)
	- console screen
	- gpu (if it's not painful to do so)?
