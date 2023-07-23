#include <riscv/CPU.hpp>
#include <riscv/Bus.hpp>

namespace riscv {

	void CPU::Clock() {
		// do the thing
		Step(1024);
	}

	void CPU::Trap(u32 trapCode) {
		// The CPU core will get to this later.
		trapped = true;
		trapCode = trapCode;

		// If this is because of an interrupt
		if(trapCode & 0x80000000) {
			// Set MIP.MEIP.
			//mip |= 1 << 11;
			// Always clear WFI bit.
			extraflags &= ~4; 
		}
	}

	void CPU::TimerInterrupt() {
		// Set MIP.MTIP.
		mip |= 1 << 7;
		//extraflags &= ~4; 
		//trapped = true;
		//trapCode = 0x80000007;
	}

	u32 CPU::Step(u32 instCount) {
		auto interruptsInFlight = [&]() {
			return 
				(mip & (1 << 7) /*|| mip & (1 << 11)*/) &&
				(mie & (1 << 7) /*|| mie & (1 << 11)*/) &&
				(mstatus & 0x8 /*mie*/);
		};

		// Don't run if waiting for an interrupt
		if(extraflags & 4)
			return 1;

		if(interruptsInFlight()) {
			Trap(0x80000007);
		} else {
			u32 rval = 0;
			u32 pc = this->pc;
			u32 cycle = this->cyclel;

			for(u32 iInst = 0; iInst < instCount; ++iInst) {
				
				cycle++;

				if(pc & 3) {
					Trap(TrapCode::InstructionAddressMisaligned);
					break;
				} else {
					auto ir = bus->PeekWord(pc);
					auto rdid = (ir >> 7) & 0x1f;
					//switch(ir & 0x7f) {

					//}
				}

			}
		}

		if(trapped) {
			mcause = trapCode;
			if(trapCode & 0x80000000) {
				mtval = 0;
				// pc -= 4
			} else {

			}
		}
	}

} // namespace riscv
