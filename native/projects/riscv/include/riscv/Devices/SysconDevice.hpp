#pragma once

#include <riscv/Bus.hpp>

namespace riscv { struct System; }

namespace riscv::devices {
	/// RISC-V SYSCON device. This will later talk to the system to tell it things.
	struct SysconDevice : public Bus::MmioDevice {
		constexpr static AddressT BASE_ADDRESS = 0x11100000;

		SysconDevice(System* system);

		AddressT Size() const override { return sizeof(u32); } // I think this is right?

		u32 Peek(AddressT address) override;
		void Poke(AddressT address, u32 value) override;
	private:
		System* system;
	};
}
