#include <algorithm>
#include <riscv/Bus.hpp>

#include "riscv/Types.hpp"

namespace riscv {

	Bus::Bus(CPU* cpu) : attachedCpu(cpu) {
	}

	Bus::~Bus() {
		// Free all devices
		for(auto device: devices)
			delete device;
	}

	bool Bus::AttachDevice(Device* device) {
		if(!device)
			return false;

		if(device->IsA<MemoryDevice*>()) {
			auto* upcasted = device->Upcast<MemoryDevice*>();

			// Refuse to overlap a device at its base address..
			if(FindDeviceForAddress(upcasted->Base()))
				return false;
			// ... or have the end overlap the start of another device.
			else if(FindDeviceForAddress(upcasted->Base() + upcasted->Size()))
				return false;

			mapped_devices[upcasted->Base()] = upcasted;
		} else if(device->IsA<MmioDevice*>()) {
			auto* upcasted = device->Upcast<MmioDevice*>();

			// Refuse to overlap a device at its base address..
			if(FindDeviceForAddress(upcasted->Base()))
				return false;
			// ... or have the end overlap the start of another device.
			else if(FindDeviceForAddress(upcasted->Base() + upcasted->Size()))
				return false;

			mmio_devices[upcasted->Base()] = upcasted;
		}

		devices.push_back(device);

		return true;
	}

	void Bus::Clock() {
		for(auto device : devices) {
			if(device->Clocked())
				device->Clock();
		}
	}

	u8 Bus::PeekByte(AddressT address) {
		if(auto dev = FindDeviceForAddress(address); dev)
			return dev->Upcast<MemoryDevice*>()->PeekByte(address);
		return -1;
	}

	u16 Bus::PeekShort(AddressT address) {
		if(auto dev = FindDeviceForAddress(address); dev)
			return dev->Upcast<MemoryDevice*>()->PeekShort(address);
		return -1;
	}

	u32 Bus::PeekWord(AddressT address) {
		if(auto dev = FindDeviceForAddress(address); dev) {
			if(dev->IsA<MmioDevice*>()) 
				return dev->Upcast<MmioDevice*>()->Peek(address);
			else
				return dev->Upcast<MemoryDevice*>()->PeekWord(address);
		}
		return -1;
	}

	void Bus::PokeByte(AddressT address, u8 value) {
		if(auto dev = FindDeviceForAddress(address); dev)
			return dev->Upcast<MemoryDevice*>()->PokeByte(address, value);
	}

	void Bus::PokeShort(AddressT address, u16 value) {
		if(auto dev = FindDeviceForAddress(address); dev)
			return dev->Upcast<MemoryDevice*>()->PokeShort(address, value);
	}

	void Bus::PokeWord(AddressT address, u32 value) {
		if(auto dev = FindDeviceForAddress(address); dev) {
			if(dev->IsA<MmioDevice*>()) 
				dev->Upcast<MmioDevice*>()->Poke(address, value);
			else
				dev->Upcast<MemoryDevice*>()->PokeWord(address, value);
		}
	}

	Bus::Device* Bus::FindDeviceForAddress(AddressT address) const {
		auto try_find_device = [&](auto container, AddressT address) {
			return std::find_if(container.begin(), container.end(), [&](const auto& pair) {
				return
				// We can shorcut region checking if the requested addess matches base address.
				pair.first == address ||
				// If it doesn't we really can't, though.
				(address >= pair.first && address < pair.first + pair.second->Size());
			});
		};

		if(auto it = try_find_device(mapped_devices, address); it != mapped_devices.end())
			return static_cast<Bus::Device*>(it->second);
		else if(auto it = try_find_device(mmio_devices, address); it != mmio_devices.end())
			return static_cast<Bus::Device*>(it->second);

		return nullptr;
	}

} // namespace riscv
