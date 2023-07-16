#include <riscv/Bus.hpp>

#include <algorithm>

namespace riscv {

	bool Bus::AttachDevice(AddressT baseAddress, Device* device) {
		if(!device)
			return false;

		// Refuse to overlap a device at its base address..
		if(FindDeviceForAddress(baseAddress))
			return false;
		// ... or have the end overlap the start of another device.
		else if(FindDeviceForAddress(baseAddress + device->Size()))
			return false;

		//mapped_devices.push_back({ .baseAddress = baseAddress, .device = device });
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
			return opt->PeekByte(address - opt->BaseAddress());
		return -1;
	}

	u16 Bus::PeekShort(AddressT address) {
		if(auto opt = FindDeviceForAddress(address); opt)
			return opt->PeekShort(address - opt->BaseAddress());
		return -1;
	}

	u32 Bus::PeekWord(AddressT address) {
		if(auto opt = FindDeviceForAddress(address); opt)
			return opt->PeekWord(address - opt->BaseAddress());
		return -1;
	}

	void Bus::PokeByte(AddressT address, u8 value) {
		if(auto opt = FindDeviceForAddress(address); opt)
			return opt->PokeByte(address - opt->BaseAddress(), value);
	}

	void Bus::PokeShort(AddressT address, u16 value) {
		if(auto opt = FindDeviceForAddress(address); opt)
			return opt->PokeShort(address - opt->BaseAddress(), value);
	}

	void Bus::PokeWord(AddressT address, u32 value) {
		if(auto opt = FindDeviceForAddress(address); opt)
			return opt->PokeWord(address - opt->BaseAddress(), value);
	}

	OptionalRef<Bus::Device&> Bus::FindDeviceForAddress(AddressT address) const {
		/*
		for(auto& device : mapped_devices) {
			// If the requested address directly matches the base address of a device
			// mapped into memory, then we do not even need to consider checking the layout.
			if(device.baseAddress == address)
				return *device.device;

			// Otherwise, we *do* unfortunately have to do so.
			if(address >= device.baseAddress &&
			   address < device.baseAddress + device.device->Size())
				return *device.device;
		}*/

		auto it = std::find_if(mapped_devices.begin(), mapped_devices.end(), [&](const auto& pair) {
			return 
				// We can shorcut the region checking if the requested addess matches base address.
				pair.first == address ||
				// If it doesn't we really can't, though.
				(address >= pair.first &&
			   		address < pair.first + pair.second->Size());
		});
		

		// No device was found at this address
		if(it == mapped_devices.end())
			return NullRef;
		else
			return *it->second;
	}

} // namespace riscv
