#include <riscv/Devices/RamDevice.hpp>

namespace riscv::devices {

	RamDevice::RamDevice(Address base, Address size) : memoryBase(base), memorySize(size) {
		memory = new u8[size]{};
		LUCORE_CHECK(memory, "Could not allocate buffer for memory device with size 0x{:08x}.", size);
	}

	RamDevice::~RamDevice() {
		delete[] memory;
	}

	void RamDevice::Resize(Address newSize) {
		if(memory) {
			delete[] memory;
		}

		memory = new u8[newSize];
		memorySize = newSize;
	}

	Address RamDevice::Base() const {
		return memoryBase;
	}

	Address RamDevice::Size() const {
		return memorySize;
	}

	u8 RamDevice::PeekByte(Address address) {
		return memory[AddressToIndex<u8>(address)];
	}

	u16 RamDevice::PeekShort(Address address) {
		return std::bit_cast<u16*>(memory)[AddressToIndex<u16>(address)];
	}

	u32 RamDevice::PeekWord(Address address) {
		return std::bit_cast<u32*>(memory)[AddressToIndex<u32>(address)];
	}

	void RamDevice::PokeByte(Address address, u8 value) {
		memory[AddressToIndex<u8>(address)] = value;
	}

	void RamDevice::PokeShort(Address address, u16 value) {
		std::bit_cast<u16*>(memory)[AddressToIndex<u16>(address)] = value;
	}

	void RamDevice::PokeWord(Address address, u32 value) {
		std::bit_cast<u32*>(memory)[AddressToIndex<u32>(address)] = value;
	}

} // namespace riscv::devices
