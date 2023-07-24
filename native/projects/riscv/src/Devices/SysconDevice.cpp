#include <lucore/Logger.hpp>
#include <riscv/Devices/SysconDevice.hpp>
#include <riscv/System.hpp>

namespace riscv::devices {

	SysconDevice::SysconDevice(System* system) : system(system) {
	}

	u32 SysconDevice::Peek(Address address) {
		return -1;
	}

	void SysconDevice::Poke(Address address, u32 value) {
		if(address == BASE_ADDRESS) {
			if(value == 0x5555)
				system->SysconPowerOff();
			else if(value == 0x7777)
				system->SysconReboot();
		}
	}

} // namespace riscv::devices
