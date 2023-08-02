#include <riscv/System.hpp>

namespace riscv {

	lucore::Unique<System> System::Create(Address ramSize) {
		auto system = std::make_unique<System>();

		// create all the devices we require.
		system->bus = std::make_unique<Bus>();
		system->cpu = std::make_unique<CPU>();
		system->ram = std::make_unique<devices::RamDevice>(0x80000000, ramSize);
		system->clnt = std::make_unique<devices::ClntDevice>();
		system->syscon = std::make_unique<devices::SysconDevice>(system.get());

		// attach everything into the bus
		if(!system->bus->AttachDevice(system->cpu.get()))
			return nullptr;
		if(!system->bus->AttachDevice(system->clnt.get()))
			return nullptr;
		if(!system->bus->AttachDevice(system->syscon.get()))
			return nullptr;
		if(!system->bus->AttachDevice(system->ram.get()))
			return nullptr;

		// reset the bus and all devices on it
		system->bus->Reset();

		return system;
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
