#pragma once
#include <riscv/Types.hpp>

namespace riscv {

	// Note that
	enum class TrapCode : u32 {
		InstructionAddressMisaligned,
		InstructionAddressFault,
		IllegalInstruction,
		Breakpoint, // I see what you did there ;)
		LoadAddressMisaligned,
		LoadAccessFault,
		StoreAddressMisaligned,
		StoreAccessFault,

		EnvCallUMode,
		EnvCallSMode,
		EnvCallMMode,

		// Not used but documented
		InstructionPageFault,
		LoadPageFault,
		StorePageFault
	};

	enum class Gpr : u8 {
		X0, // zero
		X1,
		X2,
		X3,
		X4,
		X5,
		X6,
		X7,
		X8,
		X9,
		X10,
		X11,
		X12,
		X13,
		X14,
		X15,
		X16,
		X17,
		X18,
		X19,
		X20,
		X21,
		X22,
		X23,
		X24,
		X25,
		X26,
		X27,
		X28,
		X29,
		X30,
		X31,
		PC
	};

	/// Container for GPRs which can be Statically Typed or not depending on use case.
	/// Pretty cool, huh?
	struct GeneralPurposeRegisters {
		constexpr u32& operator[](Gpr gpr) { return operator[](static_cast<usize>(gpr)); }
		constexpr u32& operator[](usize index) { return gprs_[index]; }
		u32 gprs_[32];
	};

} // namespace riscv
