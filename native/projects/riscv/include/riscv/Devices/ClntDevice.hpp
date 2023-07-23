#pragma once

#include <riscv/Bus.hpp>

namespace riscv::devices {

	/// Partial implementation of a CLINT.
	/// The timer device is implemented, SIP is not.
	struct ClntDevice : public Bus::MmioDevice {
		constexpr static AddressT BASE_ADDRESS = 0x11000000;

		AddressT Base() const override { return BASE_ADDRESS; }
		AddressT Size() const override { return 0xbfff; }

		bool Clocked() const override { return true; } 
		void Clock() override;


		u32 Peek(AddressT address) override;
		void Poke(AddressT address, u32 value) override;

	private:
		u32 timerCountHigh;
		u32 timerCountLow;

		u32 timerMatchHigh;
		u32 timerMatchLow;	
	};

}
