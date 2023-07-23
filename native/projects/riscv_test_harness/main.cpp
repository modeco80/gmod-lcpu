#include <riscv/Bus.hpp>
#include <riscv/CPU.hpp>
#include <riscv/Devices/RamDevice.hpp>

#include "riscv/Types.hpp"

/// simple 16550 UART implementation
struct SimpleUartDevice : public riscv::Bus::MmioDevice {
	constexpr static riscv::AddressT BASE_ADDRESS = 0x10000000;

	riscv::AddressT Base() const override { return BASE_ADDRESS; }

	riscv::AddressT Size() const override { return 5; }

	// TODO: emulate properly
	u32 Peek(riscv::AddressT address) override {
		switch(address) {
			case BASE_ADDRESS:
				break;
			case BASE_ADDRESS + 5:
				break;
		}

		return 0;
	}

	void Poke(riscv::AddressT address, u32 value) override {
		if(address == BASE_ADDRESS) { // write to data buffer
			printf("%c\n", value);
		}
	}
};

int main() {
	return 0;
}
