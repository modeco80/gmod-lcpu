#include <algorithm>
#include <riscv/Bus.hpp>
#include <riscv/CPU.hpp>

namespace riscv {

	Bus::~Bus() {
		// Free all devices
		for(auto device : devices)
			delete device;
	}

	bool Bus::AttachDevice(Device* device) {
		if(!device)
			return false;

		if(device->IsA<CPU*>()) {
			// Return early to avoid putting the CPU pointer inside the devices vector.
			// We do not actually own the CPU.
			cpu = device->Upcast<CPU*>();
			return true;
		}

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

		// The CPU is always clocked last to ensure devices can generate
		// interrupts properly. I don't make the rules.
		cpu->Clock();
	}

	u8 Bus::PeekByte(Address address) {
		if(auto dev = FindDeviceForAddress(address); dev)
			return dev->Upcast<MemoryDevice*>()->PeekByte(address);
		else {
			cpu->Trap(TrapCode::LoadAccessFault);
			return -1;
		}
	}

	u16 Bus::PeekShort(Address address) {
		if(auto dev = FindDeviceForAddress(address); dev)
			return dev->Upcast<MemoryDevice*>()->PeekShort(address);
		else {
			cpu->Trap(TrapCode::LoadAccessFault);
			return -1;
		}
	}

	u32 Bus::PeekWord(Address address) {
		if(auto dev = FindDeviceForAddress(address); dev) {
			if(dev->IsA<MmioDevice*>())
				return dev->Upcast<MmioDevice*>()->Peek(address);
			else
				return dev->Upcast<MemoryDevice*>()->PeekWord(address);
		} else {
			cpu->Trap(TrapCode::LoadAccessFault);
			return -1;
		}
	}

	void Bus::PokeByte(Address address, u8 value) {
		if(auto dev = FindDeviceForAddress(address); dev)
			return dev->Upcast<MemoryDevice*>()->PokeByte(address, value);
		else {
			cpu->Trap(TrapCode::StoreAccessFault);
		}
	}

	void Bus::PokeShort(Address address, u16 value) {
		if(auto dev = FindDeviceForAddress(address); dev)
			return dev->Upcast<MemoryDevice*>()->PokeShort(address, value);
		else {
			cpu->Trap(TrapCode::StoreAccessFault);
		}
	}

	void Bus::PokeWord(Address address, u32 value) {
		if(auto dev = FindDeviceForAddress(address); dev) {
			if(dev->IsA<MmioDevice*>())
				dev->Upcast<MmioDevice*>()->Poke(address, value);
			else
				dev->Upcast<MemoryDevice*>()->PokeWord(address, value);
		} else {
			cpu->Trap(TrapCode::StoreAccessFault);
		}
	}

	Bus::Device* Bus::FindDeviceForAddress(Address address) const {
		auto try_find_device = [&](auto container, Address address) {
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
