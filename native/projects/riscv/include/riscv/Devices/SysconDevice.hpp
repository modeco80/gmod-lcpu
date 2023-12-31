#pragma once

#include <riscv/Bus.hpp>

namespace riscv {
	struct System;
}

namespace riscv::devices {
	/// RISC-V SYSCON device. This will later talk to the system to tell it things.
	struct SysconDevice : public Bus::MmioDevice {
		constexpr static Address BASE_ADDRESS = 0x11100000;

		SysconDevice(System* system);

		Address Base() const override { return BASE_ADDRESS; }
		Address Size() const override { return sizeof(u32); } // I think this is right?

		u32 Peek(Address address) override;
		void Poke(Address address, u32 value) override;

	   private:
		System* system;
	};
} // namespace riscv::devices
