#include <lucore/Logger.hpp>
#include <riscv/Devices/SysconDevice.hpp>
#include <riscv/System.hpp>

// TODO: This device is largely a stub. It needs to be implemented!

namespace riscv::devices {

	SysconDevice::SysconDevice(System* system) : system(system) {
	}

	u32 SysconDevice::Peek(Address address) {
		lucore::LogInfo("SYSCON({}) Peek @ 0x{:08x}", static_cast<void*>(this), address);
		return -1;
	}

	void SysconDevice::Poke(Address address, u32 value) {
		lucore::LogInfo("SYSCON({}) Poke @ 0x{:08x}: 0x{:08x}", static_cast<void*>(this), address,
						value);
		/*
			if(address == BASE_ADDRESS) {
				if(value == 0x5555)
					system->PowerOff();
				else if (value == 0x7777)
					system->Reset();
			}
		*/
		return;
	}

} // namespace riscv::devices
