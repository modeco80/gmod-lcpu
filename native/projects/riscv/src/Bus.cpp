#include <algorithm>
#include <riscv/Bus.hpp>

namespace riscv {

	Bus::Bus(CPU* cpu)
		: attachedCpu(cpu) {

	}

	Bus::~Bus() {
		// Free all devices
		for(auto& pair : mapped_devices)
			delete pair.second;
	}

	bool Bus::AttachDevice(AddressT baseAddress, Device* device) {
		if(!device)
			return false;

		// Refuse to overlap a device at its base address..
		if(FindDeviceForAddress(baseAddress))
			return false;
		// ... or have the end overlap the start of another device.
		else if(FindDeviceForAddress(baseAddress + device->Size()))
			return false;

		mapped_devices[baseAddress] = device;
		return true;
	}

	void Bus::Clock() {
		for(auto& device : mapped_devices) {
			if(device.second->Clocked())
				device.second->Clock();
		}
	}

	u8 Bus::PeekByte(AddressT address) {
		if(auto opt = FindDeviceForAddress(address); opt)
			return opt->PeekByte(address);
		return -1;
	}

	u16 Bus::PeekShort(AddressT address) {
		if(auto opt = FindDeviceForAddress(address); opt)
			return opt->PeekShort(address);
		return -1;
	}

	u32 Bus::PeekWord(AddressT address) {
		if(auto opt = FindDeviceForAddress(address); opt)
			return opt->PeekWord(address);
		return -1;
	}

	void Bus::PokeByte(AddressT address, u8 value) {
		if(auto opt = FindDeviceForAddress(address); opt)
			return opt->PokeByte(address, value);
	}

	void Bus::PokeShort(AddressT address, u16 value) {
		if(auto opt = FindDeviceForAddress(address); opt)
			return opt->PokeShort(address, value);
	}

	void Bus::PokeWord(AddressT address, u32 value) {
		if(auto opt = FindDeviceForAddress(address); opt)
			return opt->PokeWord(address, value);
	}

	lucore::OptionalRef<Bus::Device> Bus::FindDeviceForAddress(AddressT address) const {
		auto it = std::find_if(mapped_devices.begin(), mapped_devices.end(), [&](const auto& pair) {
			return
			// We can shorcut region checking if the requested addess matches base address.
			pair.first == address ||
			// If it doesn't we really can't, though.
			(address >= pair.first && address < pair.first + pair.second->Size());
		});

		// No device was found at this address
		if(it == mapped_devices.end())
			return lucore::NullRef;
		else
			return *it->second;
	}

} // namespace riscv
