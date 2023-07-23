#include <riscv/Bus.hpp>
#include <riscv/Types.hpp>

namespace riscv {

	/// The CPU core.
	struct CPU : Bus::Device {
		/// CPU core state.
		struct State {
			u32 gpr[32];
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
		};

		CPU(Bus* bus);

		State& GetState() { return state; }

		bool Clocked() const override { return true; } 
		void Clock() override;

		// TODO: Handlers for CSR read/write

	   private:
		State state;
		Bus* bus;

		u32 Step(u32 elapsedMicroseconds, u32 instCount);

		// todo: counters for chrono/inst count.
	};

} // namespace riscv
