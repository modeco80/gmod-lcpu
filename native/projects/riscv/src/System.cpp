#include <riscv/System.hpp>

namespace riscv {

	System* System::Create(Address ramSize) {
		auto* system = new System;

		system->bus = new Bus();
		system->cpu = new CPU();
		system->ram = new devices::RamDevice(0x80000000, ramSize);
		system->clnt = new devices::ClntDevice();
		system->syscon = new devices::SysconDevice(system);

		system->cpu->Reset();

		system->bus->AttachDevice(system->cpu);
		system->bus->AttachDevice(system->clnt);
		system->bus->AttachDevice(system->syscon);
		system->bus->AttachDevice(system->ram);
		
		return system;
	}

	System::~System() {
		delete cpu;
		delete bus; // the rest of the device pointers will be deleted by the bus.
	}

	void System::Step() {
		// Clock the bus, it'll do everything else.
		bus->Clock();

		// Later: handling for invalid cases!
	}

} // namespace riscv
