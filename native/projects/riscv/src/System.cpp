#include <riscv/System.hpp>

namespace riscv {

	System* System::Create(Address ramSize) {
		auto* system = new System;

		// create all the devices we require.
		system->bus = new Bus();
		system->cpu = new CPU();
		system->ram = new devices::RamDevice(0x80000000, ramSize);
		system->clnt = new devices::ClntDevice();
		system->syscon = new devices::SysconDevice(system);

		// techinically this is done on construction but lets be hard about it
		system->cpu->Reset();

		// attach everything into the bus
		if(!system->bus->AttachDevice(system->cpu))
			return nullptr;
		if(!system->bus->AttachDevice(system->clnt))
			return nullptr;
		if(!system->bus->AttachDevice(system->syscon))
			return nullptr;
		if(!system->bus->AttachDevice(system->ram))
			return nullptr;

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

	void System::SysconPowerOff() {
		if(OnPowerOff)
			OnPowerOff();
	}

	void System::SysconReboot() {
		if(OnReboot)
			OnReboot();
		bus->GetCPU()->Reset();
	}

} // namespace riscv
