#pragma once
#include <riscv/Types.hpp>
#include <string_view>

namespace riscv {

	/// Most possible trap codes.
	enum class TrapCode : u32 {
		InstructionAddressMisaligned,
		InstructionAccessFault,
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
		Zero,
		Ra,
		Sp,
		Gp,
		Tp,
		T0,
		T1,
		T2,
		S0, // also `fp`
		S1,
		A0,
		A1,
		A2,
		A3,
		A4,
		A5,
		A6,
		A7,
		S2,
		S3,
		S4,
		S5,
		S6,
		S7,
		S8,
		S9,
		S10,
		S11,
		T3,
		T4,
		T5,
		T6
	};

	constexpr std::string_view RegName(Gpr gpr) {
		std::string_view table[] = {
			"zero",
			"ra",
			"sp",
			"gp",
			"tp",
			"t0",
			"t1",
			"t2",
			"s0",
			"s1",
			"a0",
			"a1",
			"a2",
			"a3",
			"a4",
			"a5",
			"a6",
			"a7",
			"s2",
			"s3",
			"s4",
			"s5",
			"s6",
			"s7",
			"s8",
			"s9",
			"s10",
			"s11",
			"t3",
			"t4",
			"t5",
			"t6"
		};
		return table[static_cast<usize>(gpr)];
	}

	/// Container for GPRs which can be Statically Typed or not depending on use case.
	/// Pretty cool, huh?
	struct GeneralPurposeRegisters {
		constexpr u32& operator[](Gpr gpr) { return operator[](static_cast<usize>(gpr)); }
		constexpr u32& operator[](usize index) { return gprs_[index]; }
		u32 gprs_[32];
	};

} // namespace riscv
