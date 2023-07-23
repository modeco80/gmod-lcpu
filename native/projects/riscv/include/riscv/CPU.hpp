#include <riscv/Bus.hpp>
#include <riscv/CPUTypes.hpp>
#include <riscv/Types.hpp>

namespace riscv {

	/// The CPU core.
	struct CPU : Bus::Device {
		bool Clocked() const override { return true; }
		void Clock() override;

		void Trap(u32 trapCode);
		inline void Trap(TrapCode trapCode) { Trap(static_cast<u32>(trapCode)); }

		void TimerInterrupt();

		// TODO: Handlers for CSR read/write (if we need it?)

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
		/// Codes with the sign bit set are actually interrupts,
		/// and are processed first.
		bool trapped { false };
		u32 trapCode { 0 };

		u32 Step(u32 instCount);

		// todo: counters for chrono/inst count.
	};

} // namespace riscv
