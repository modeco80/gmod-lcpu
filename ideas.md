# LCPU Idea/Working Model Sheet

This is basically the working ideas for the LCPU project.

## Core features

- CPU SENTs
	- RISC-V rv32ima core
		- Would supporting more than one CPU core type be worthwhile? If so, the project is relatively setup for such workflow...
	- Controllable paramaters (RAM size, ...)

## Code upload

- Upload a raw binary to execute, generated from any user tooling (goes into server data folder)
	- Yes, this means you can run Linux in GMod. No, I'm not sorry.

## Integrated simple project workflow

- Uses official RISC-V GCC toolchain
	- In a podman container for jailing reasons?

- Write assembly/C/C++ code using a tiny project system (source for them would go in server data folder ?)
	- At the root of a project, a `project.json` file is expected to exist, with contents like:
	```json
		{
			"project": {
				// All configurations for a project.
				"configurations": {
					"debug": {
						"CCompileFlags": "-O0 -g ${BaseCCompileFlags}",
						"CppCompileFlags": "-O0 -g ${BaseCppCompileFlags}",
						"LinkerScript": "binary.ld",
					},
					"release": {
						"CCompileFlags": "-O2 ${BaseCCompileFlags}",
						"CppCompileFlags": "-O2 ${BaseCppCompileFlags}",
						"LinkerScript": "binary.ld",
						"LinkerFlags": "-Wl,--gc-sections"
					},
				},

				// Obviously you can use separate subdirectories;
				// this is just a very very simple baremetal program.
				"Sources": [
					"startup.S",
					"main.cpp"
				]
			}
		}
	```

	- `BaseCCompileFlags` and `BaseCppCompileFlags` are defaulted to sane values for each language.

	- This will be transpiled into a `Makefile` by the addon.
		- A standalone tool will be provided and used for transpiling `project.json` to a `Makefile` (and maybe even passed into the container and transpiled there, to reduce the actions on the host to just the podman run?)
		- which is then run with `make` in a temporary podman container which only has access to the source code for the project (and nothing else, besides riscv tools).
			- Command line is probably something like `make CONFIG=${config}`
		- the output binary will be stored alongside the source code on the server side, with a name like `${name}-${config}.bin`
			- This file can then be selected for loading (without uploading from the client).


- There is no conditional compilation in the `project.json` system
	- All files in a project are always built by that project.

- Text editor used to edit project source files
	- Use the Wire editor? (we need wiremod anyways, and the text editor is.. OK I suppose.)

- Some example projects?
	- I joke about it, but an RTOS would be really nice and a good stress test of the project system (for usage in "real" projects.)

## Moderation/administration tools
- Admin controlled per-user max RAM size (default 64mb)

- Admin controlled per-user max LCPU entity count (default 8)

- Admin controled global (affects all placed LCPUs) scheduler cycle rate/instruction count.
		- default is 0.1/1024 instructions

## Addon interopability

- Wiremod interopability
	- Wiremod GPIO peripheral (to interface with wire world)
	- special Console Screen peripheral (interfacing specifically with it)
	- frambuffer peripheral which uses the Digital Screen
