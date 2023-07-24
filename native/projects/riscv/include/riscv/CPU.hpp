#include <riscv/Bus.hpp>
#include <riscv/CPUTypes.hpp>
#include <riscv/Types.hpp>

namespace riscv {

	/// The CPU core.
	struct CPU : Bus::Device {
		BasicType Type() const override { return BasicType::Cpu; } 
		bool Clocked() const override { return true; }
		void Clock() override;

		/// Trap the CPU. Bus devices can call this.
		void Trap(u32 trapCode);
		inline void Trap(TrapCode trapCode) { Trap(static_cast<u32>(trapCode)); }

		void TimerInterrupt();

		constexpr CPU() {
			Reset();
		}

		constexpr void Reset() {
			// Initalize some state. We're cool like that :)
			pc = 0x80000000;
			gpr[Gpr::A0] = 0x0; // HART id
			extraflags |= 3; // Start in Machine mode
		}

		// TODO: Handlers for CSR read/write (if we need it?)

		/// CPU state
		GeneralPurposeRegisters gpr;
		u32 pc;
		u32 mstatus;
		u32 cyclel;
		u32 cycleh;
		u32 mscratch;
		u32 mtvec;
		u32 mie;
		u32 mip;

		u32 mepc;
		u32 mtval;
		u32 mcause;

		// Note: only a few bits are used.  (Machine = 3, User = 0)
		// Bits 0..1 = privilege.
		// Bit 2 = WFI (Wait for interrupt)
		// Bit 3+ = Load/Store reservation LSBs.
		u32 extraflags;

	   private:
		/// Set by [CPU::Trap] to tell the CPU it was trapped.
		bool trapped { false };

		/// Set by [CPU::Trap] for the trap code.
		u32 trapCode { 0 };

		void Step(u32 instCount);
	};

} // namespace riscv
