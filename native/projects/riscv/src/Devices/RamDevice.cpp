#include <riscv/Devices/RamDevice.hpp>

#include "riscv/Types.hpp"

namespace riscv::devices {

	RamDevice::RamDevice(AddressT size) : Bus::Device(), memorySize(size) {
		memory = new u8[size];
		LUCORE_CHECK(memory, "Could not allocate buffer for memory device with size 0x{:08x}.",
					 size);
	}

	RamDevice::~RamDevice() {
		if(memory)
			delete[] memory;
	}

	AddressT RamDevice::Size() const {
		return memorySize;
	}

	// Implementation of Device interface

	void RamDevice::Attached(Bus* bus, AddressT base) {
		attachedBus = bus;
		baseAddress = base;
	}

	u8 RamDevice::PeekByte(AddressT address) {
		return memory[AddressToIndex<u8>(address)];
	}

	u16 RamDevice::PeekShort(AddressT address) {
		return std::bit_cast<u16*>(memory)[AddressToIndex<u16>(address)];
	}

	u32 RamDevice::PeekWord(AddressT address) {
		return std::bit_cast<u32*>(memory)[AddressToIndex<u32>(address)];
	}

	void RamDevice::PokeByte(AddressT address, u8 value) {
		memory[AddressToIndex<u8>(address)] = value;
	}

	void RamDevice::PokeShort(AddressT address, u16 value) {
		std::bit_cast<u16*>(memory)[AddressToIndex<u16>(address)] = value;
	}

	void RamDevice::PokeWord(AddressT address, u32 value) {
		std::bit_cast<u32*>(memory)[AddressToIndex<u32>(address)] = value;
	}

} // namespace riscv::devices
