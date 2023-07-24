//! A test harness for testing if the riscv library actually works.
#include <cstdio> // I know, I know, but this is a test program. Yell later :)
#include <lucore/Assert.hpp>
#include <lucore/Logger.hpp>
#include <lucore/StdoutSink.hpp>
#include <riscv/System.hpp>

/// simple 16550 UART implementation
struct SimpleUartDevice : public riscv::Bus::MmioDevice {
	constexpr static riscv::Address BASE_ADDRESS = 0x10000000;

	riscv::Address Base() const override { return BASE_ADDRESS; }

	riscv::Address Size() const override { return 12; } // for now

	u32 Peek(riscv::Address address) override {
		switch(address) {
			case BASE_ADDRESS: return 0x60; // active, but no keyboard input
			case BASE_ADDRESS + 5: return '\0';
		}

		return 0;
	}

	void Poke(riscv::Address address, u32 value) override {
		if(address == BASE_ADDRESS) {
			char c = value & 0x000000ff;
			fputc(c, stderr);
		}
	}
};

int main(int argc, char** argv) {
	lucore::LoggerAttachStdout();

	LUCORE_CHECK(argc == 2, "this test harness expects one argument (the file to load into riscv memory and execute). got {} arguments", argc);

	// 128 KB of ram. Won't be enough to boot linux but should be good enough to test most baremetal apps
	auto system = riscv::System::Create(128 * 1024);
	LUCORE_CHECK(system, "could not create system for some reason.");

	// Attach our UART device
	system->bus->AttachDevice(new SimpleUartDevice);

	auto fp = std::fopen(argv[1], "rb");
	LUCORE_CHECK(fp, "could not open file \"{}\"", argv[1]);

	std::fseek(fp, 0, SEEK_END);
	auto len = std::ftell(fp);
	std::fseek(fp, 0, SEEK_SET);

	std::fread(system->ram->Raw(), 1, len, fp);
	std::fclose(fp);

	// This allows the host program running under the test
	// harness to tell us to shut down.
	bool shouldExit = false;
	system->OnPowerOff = [&shouldExit]() { shouldExit = true; };

	while(!shouldExit) {
		system->Step();
	}

	delete system;
	return 0;
}
