//! A test harness for testing the riscv library.
#include <riscv/System.hpp>

/// simple 16550 UART implementation
struct SimpleUartDevice : public riscv::Bus::MmioDevice {
	constexpr static riscv::Address BASE_ADDRESS = 0x10000000;

	riscv::Address Base() const override { return BASE_ADDRESS; }

	riscv::Address Size() const override { return 5; }

	// TODO: emulate properly
	u32 Peek(riscv::Address address) override {
		switch(address) {
			case BASE_ADDRESS:
				break;
			case BASE_ADDRESS + 5:
				break;
		}

		return 0;
	}

	void Poke(riscv::Address address, u32 value) override {
		if(address == BASE_ADDRESS) { // write to data buffer
			printf("%c\n", value);
		}
	}
};

int main() {
	auto system = riscv::System::WithMemory(128 * 1024);
	system->AddDeviceToBus(new SimpleUartDevice);
	return 0;
}
