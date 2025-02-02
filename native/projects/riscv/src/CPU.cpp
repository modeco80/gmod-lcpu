//! Portions of this code are copyright 2022 Charles Lohr (CNLohr),
//! from [mini-rv32ima](https://github.com/cnlohr/mini-rv32ima).

#include <riscv/Bus.hpp>
#include <riscv/CPU.hpp>

namespace riscv {

	void CPU::Clock() {
		Step(instructionCount);
	}

	void CPU::Trap(u32 trapCode) {
		// The CPU core will get to this later.
		trapped = true;
		trapCode = trapCode;

		// If this is because of an interrupt
		if(trapCode & 0x80000000) {
			// Set MIP.MEIP.
			// mip |= 1 << 11;
			// Always clear WFI bit.
			extraflags &= ~4;
		}
	}

	void CPU::TimerInterrupt() {
		// Set MIP.MTIP.
		mip |= 1 << 7;
		// extraflags &= ~4;
		// trapped = true;
		// trapCode = 0x80000007;
	}

	void CPU::Step(u32 instCount) {
		auto interruptsInFlight = [&]() {
			return (mip & (1 << 7) /*|| mip & (1 << 11)*/) && (mie & (1 << 7) /*|| mie & (1 << 11)*/) && (mstatus & 0x8 /*mie*/);
		};

		// Don't run if waiting for an interrupt
		if(extraflags & 4)
			return;

		u32 rdid = 0;
		u32 rval = 0;
		u32 cycle = this->cyclel;

		if(interruptsInFlight()) {
			Trap(0x80000007);
		} else {
			for(u32 iInst = 0; iInst < instCount; ++iInst) {
				rdid = 0; // force it to gpr 0 (zero), which is not writable
				cycle++;

				if((pc & 3)) {
					Trap(TrapCode::InstructionAddressMisaligned);
					rval = pc;
					break;
				} else {
					auto ir = bus->PeekWord(pc);
					if(trapped) {
						// Overwrite the trap that the bus generated. This might not really work out
						// in practice but should at least kind-of replicate behaviour (our address
						// space is emulated using the [Bus] class, so there is no heap write issue
						// that could be caused by leaving it unbound).
						Trap(TrapCode::InstructionAccessFault);
						rval = pc;
						break;
					}

					rdid = (ir >> 7) & 0x1f;

					// Do the thing!
					switch(ir & 0x7f) {
						case 0x37: // lui
							rval = (ir & 0xfffff000);
							break;

						case 0x17: // auipc
							rval = pc + (ir & 0xfffff000);
							break;

						case 0x6f: { // jal
							i32 reladdy = ((ir & 0x80000000) >> 11) | ((ir & 0x7fe00000) >> 20) | ((ir & 0x00100000) >> 9) | ((ir & 0x000ff000));

							// Sign extend in this case
							if(reladdy & 0x00100000)
								reladdy |= 0xffe00000;
							rval = pc + 4;
							pc = pc + reladdy - 4;
							break;
						}

						case 0x67: { // jalr
							u32 imm = ir >> 20;
							i32 imm_se = imm | ((imm & 0x800) ? 0xfffff000 : 0);
							rval = pc + 4;
							pc = ((gpr[((ir >> 15) & 0x1f)] + imm_se) & ~1) - 4;
							break;
						}

						case 0x63: { // branch
							u32 immm4 = ((ir & 0xf00) >> 7) | ((ir & 0x7e000000) >> 20) | ((ir & 0x80) << 4) | ((ir >> 31) << 12);
							if(immm4 & 0x1000)
								immm4 |= 0xffffe000;
							i32 rs1 = gpr[(ir >> 15) & 0x1f];
							i32 rs2 = gpr[(ir >> 20) & 0x1f];
							immm4 = pc + immm4 - 4;
							rdid = 0;
							switch((ir >> 12) & 0x7) {
								// BEQ, BNE, BLT, BGE, BLTU, BGEU
								case 0: // BEQ
									if(rs1 == rs2)
										pc = immm4;
									break;
								case 1: // BNE
									if(rs1 != rs2)
										pc = immm4;
									break;

								case 4: // BLT
									if(rs1 < rs2)
										pc = immm4;
									break;

								case 5: // BGE
									if(rs1 >= rs2)
										pc = immm4;
									break;

								case 6: // BLTU
									if((u32)rs1 < (u32)rs2)
										pc = immm4;
									break;

								case 7: // BGEU
									if((u32)rs1 >= (u32)rs2)
										pc = immm4;
									break;
								default: Trap(TrapCode::IllegalInstruction);
							}
							break;
						}

						case 0x03: { // load
							u32 rs1 = gpr[(ir >> 15) & 0x1f];
							u32 imm = ir >> 20;
							i32 imm_se = imm | ((imm & 0x800) ? 0xfffff000 : 0);
							u32 rsval = rs1 + imm_se;

							switch((ir >> 12) & 0x7) {
								// LB, LH, LW, LBU, LHU
								case 0: rval = (i8)bus->PeekByte(rsval); break;
								case 1: rval = (i16)bus->PeekShort(rsval); break;
								case 2: rval = bus->PeekWord(rsval); break;
								case 4: rval = bus->PeekByte(rsval); break;
								case 5: rval = bus->PeekShort(rsval); break;
								default: Trap(TrapCode::IllegalInstruction); break;
							}

							if(trapped) {
								rval = rsval;
							}
							break;
						}

						case 0x23: { // store
							u32 rs1 = gpr[(ir >> 15) & 0x1f];
							u32 rs2 = gpr[(ir >> 20) & 0x1f];
							u32 addy = ((ir >> 7) & 0x1f) | ((ir & 0xfe000000) >> 20);
							if(addy & 0x800)
								addy |= 0xfffff000;
							addy += rs1;
							rdid = 0;

							switch((ir >> 12) & 0x7) {
								// SB, SH, SW
								case 0: bus->PokeByte(addy, rs2); break;
								case 1: bus->PokeShort(addy, rs2); break;
								case 2: bus->PokeWord(addy, rs2); break;
								default: Trap(TrapCode::IllegalInstruction);
							}

							if(trapped) {
								rval = addy;
							}
							break;
						}

						case 0x13:	 // op-imm
						case 0x33: { // op
							u32 imm = ir >> 20;
							imm = imm | ((imm & 0x800) ? 0xfffff000 : 0);
							u32 rs1 = gpr[(ir >> 15) & 0x1f];
							u32 is_reg = !!(ir & 0x20);
							u32 rs2 = is_reg ? gpr[imm & 0x1f] : imm;

							if(is_reg && (ir & 0x02000000)) {
								switch((ir >> 12) & 7) // 0x02000000 = RV32M
								{
									case 0: rval = rs1 * rs2; break;								 // MUL
									case 1: rval = ((i64)((i32)rs1) * (i64)((i32)rs2)) >> 32; break; // MULH
									case 2: rval = ((i64)((i32)rs1) * (u64)rs2) >> 32; break;		 // MULHSU
									case 3: rval = ((u64)rs1 * (u64)rs2) >> 32; break;				 // MULHU

									case 4:
										if(rs2 == 0)
											rval = -1;
										else
											rval = ((i32)rs1 == INT32_MIN && (i32)rs2 == -1) ? rs1 : ((i32)rs1 / (i32)rs2);
										break; // DIV
									case 5:
										if(rs2 == 0)
											rval = 0xffffffff;
										else
											rval = rs1 / rs2;
										break; // DIVU
									case 6:
										if(rs2 == 0)
											rval = rs1;
										else
											rval = ((i32)rs1 == INT32_MIN && (i32)rs2 == -1) ? 0 : ((u32)((i32)rs1 % (i32)rs2));
										break; // REM
									case 7:
										if(rs2 == 0)
											rval = rs1;
										else
											rval = rs1 % rs2;
										break; // REMU
								}
							} else {
								// These could be either op-immediate or op commands.  Be careful.
								switch((ir >> 12) & 7) {
									case 0: rval = (is_reg && (ir & 0x40000000)) ? (rs1 - rs2) : (rs1 + rs2); break;
									case 1: rval = rs1 << (rs2 & 0x1F); break;
									case 2: rval = (i32)rs1 < (i32)rs2; break;
									case 3: rval = rs1 < rs2; break;
									case 4: rval = rs1 ^ rs2; break;
									case 5: rval = (ir & 0x40000000) ? (((i32)rs1) >> (rs2 & 0x1F)) : (rs1 >> (rs2 & 0x1F)); break;
									case 6: rval = rs1 | rs2; break;
									case 7: rval = rs1 & rs2; break;
								}
							}
							break;
						}

						case 0x0f:	  // 0b0001111 (ifenze?)
							rdid = 0; // fencetype = (ir >> 12) & 0b111; We ignore fences in this impl.
							break;

						case 0x73: { // Zifencei+Zicsr  (0b1110011)

							u32 csrno = ir >> 20;
							u32 microop = (ir >> 12) & 0x7;
							// Zicsr
							if((microop & 3)) {
								int rs1imm = (ir >> 15) & 0x1f;
								u32 rs1 = gpr[rs1imm];
								u32 writeval = rs1;

								// https://raw.githubusercontent.com/riscv/virtual-memory/main/specs/663-Svpbmt.pdf
								// Generally, support for Zicsr
								switch(csrno) {
									case 0x340: rval = mscratch; break;
									case 0x305: rval = mtvec; break;
									case 0x304: rval = mie; break;
									case 0xC00: rval = cycle; break;
									case 0x344: rval = mip; break;
									case 0x341: rval = mepc; break;
									case 0x300: rval = mstatus; break; // mstatus
									case 0x342: rval = mcause; break;
									case 0x343: rval = mtval; break;
									case 0xf11: rval = 0xff0ff0ff; break; // mvendorid
									case 0x301:
										rval = 0x40401101;
										break; // misa (XLEN=32, IMA+X)
									// case 0x3B0: rval = 0; break; //pmpaddr0
									// case 0x3a0: rval = 0; break; //pmpcfg0
									// case 0xf12: rval = 0x00000000; break; //marchid
									// case 0xf13: rval = 0x00000000; break; //mimpid
									// case 0xf14: rval = 0x00000000; break; //mhartid
									default: break;
								}

								switch(microop) {
									case 1: writeval = rs1; break;			  // CSRRW
									case 2: writeval = rval | rs1; break;	  // CSRRS
									case 3: writeval = rval & ~rs1; break;	  // CSRRC
									case 5: writeval = rs1imm; break;		  // CSRRWI
									case 6: writeval = rval | rs1imm; break;  // CSRRSI
									case 7: writeval = rval & ~rs1imm; break; // CSRRCI
								}

								switch(csrno) {
									case 0x340: mscratch = writeval; break;
									case 0x305: mtvec = writeval; break;
									case 0x304: mie = writeval; break;
									case 0x344: mip = writeval; break;
									case 0x341: mepc = writeval; break;
									case 0x300: mstatus = writeval; break; // mstatus
									case 0x342: mcause = writeval; break;
									case 0x343: mtval = writeval; break;
									// case 0x3a0: break; //pmpcfg0
									// case 0x3B0: break; //pmpaddr0
									// case 0xf11: break; //mvendorid
									// case 0xf12: break; //marchid
									// case 0xf13: break; //mimpid
									// case 0xf14: break; //mhartid
									// case 0x301: break; //misa
									default: break;
								}
							} else if(microop == 0x0) { // "SYSTEM" 0b000
								rdid = 0;
								if(csrno == 0x105) { // WFI (Wait for interrupts)
									mstatus |= 8;	 // Enable interrupts
									extraflags |= 4; // Set inernal WFI bit
									this->pc = pc + 4;
									return;
								} else if(((csrno & 0xff) == 0x02)) { // MRET
									// https://raw.githubusercontent.com/riscv/virtual-memory/main/specs/663-Svpbmt.pdf
									// Table 7.6. MRET then in mstatus/mstatush sets MPV=0, MPP=0,
									// MIE=MPIE, and MPIE=1. La
									//  Should also update mstatus to reflect correct mode.
									u32 startmstatus = mstatus;
									u32 startextraflags = extraflags;
									mstatus = ((startmstatus & 0x80) >> 4) | ((startextraflags & 3) << 11) | 0x80;
									extraflags = (startextraflags & ~3) | ((startmstatus >> 11) & 3);
									pc = mepc - 4;
								} else {
									switch(csrno) {
										case 0:
											if(extraflags & 3) {
												Trap(TrapCode::EnvCallMMode);
											} else {
												Trap(TrapCode::EnvCallUMode);
											}
											break;
										case 1: // breakpoint
											Trap(TrapCode::Breakpoint);
											break;
										default: Trap(TrapCode::IllegalInstruction); break;
									}
								}
							} else
								Trap(TrapCode::IllegalInstruction);
							break;
						}

						case 0x2f: // RV32A (0b00101111)
						{
							u32 rs1 = gpr[(ir >> 15) & 0x1f];
							u32 rs2 = gpr[(ir >> 20) & 0x1f];
							u32 irmid = (ir >> 27) & 0x1f;

							rval = bus->PeekWord(rs1);
							if(trapped) {
								rval = rs1;
								break;
							}

							u32 dowrite = 1;
							switch(irmid) {
								case 2: // LR.W (0b00010)
									dowrite = 0;
									extraflags = (extraflags & 0x07) | (rs1 << 3);
									break;
								case 3:												// SC.W (0b00011) (Make sure we have a slot, and, it's
																					// valid)
									rval = (extraflags >> 3 != (rs1 & 0x1fffffff)); // Validate that our reservation slot is OK.
									dowrite = !rval;								// Only write if slot is valid.
									break;
								case 1: break;											   // AMOSWAP.W (0b00001)
								case 0: rs2 += rval; break;								   // AMOADD.W (0b00000)
								case 4: rs2 ^= rval; break;								   // AMOXOR.W (0b00100)
								case 12: rs2 &= rval; break;							   // AMOAND.W (0b01100)
								case 8: rs2 |= rval; break;								   // AMOOR.W (0b01000)
								case 16: rs2 = ((i32)rs2 < (i32)rval) ? rs2 : rval; break; // AMOMIN.W (0b10000)
								case 20: rs2 = ((i32)rs2 > (i32)rval) ? rs2 : rval; break; // AMOMAX.W (0b10100)
								case 24: rs2 = (rs2 < rval) ? rs2 : rval; break;		   // AMOMINU.W (0b11000)
								case 28: rs2 = (rs2 > rval) ? rs2 : rval; break;		   // AMOMAXU.W (0b11100)
								default:
									Trap(TrapCode::IllegalInstruction);
									dowrite = 0;
									break; // Not supported.
							}
							if(dowrite)
								bus->PokeWord(rs1, rs2);
							break;
						}
						default: Trap(TrapCode::IllegalInstruction);
					}
				}

				// If some operation caused a trap break out after executing the instruction
				if(trapped)
					break;

				if(rdid)
					gpr[rdid] = rval;
				pc += 4;
			}
		}

		if(trapped) {
			mcause = trapCode;

			// If prefixed with 1 in MSB, it's an interrupt, not a trap.
			if(trapCode & 0x80000000) {
				mtval = 0;
				pc += 4; // PC needs to point to where the PC will return to.
			} else {
				if(trapCode > 5 && trapCode <= 8)
					mtval = rval;
				else
					mtval = pc;
			}
			mepc = pc; // Interrupt handler will advance mepc
			mstatus = (mstatus & 0x08) << 4 | ((extraflags)&3) << 11;
			pc = (mtvec - 4);

			// Always enter machine mode when trapping.
			extraflags |= 3;

			// Reset trap flags
			trapped = false;
			trapCode = 0;
			pc += 4;
		}

		if(cyclel > cycle)
			cycleh++;
		cyclel = cycle;
	}

} // namespace riscv
