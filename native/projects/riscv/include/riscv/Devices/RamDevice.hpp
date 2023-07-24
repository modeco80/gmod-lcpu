#pragma once

#include <riscv/Bus.hpp>

namespace riscv::devices {

	/// A block of RAM which can be used by the CPU.
	struct RamDevice : public Bus::MemoryDevice {
		RamDevice(Address base, Address size);
		virtual ~RamDevice();

		// Implementation of Device interface

		Address Base() const override;
		Address Size() const override;

		u8* Raw() const { return memory; }

		u8 PeekByte(Address address) override;
		u16 PeekShort(Address address) override;
		u32 PeekWord(Address address) override;

		void PokeByte(Address address, u8 value) override;
		void PokeShort(Address address, u16 value) override;
		void PokeWord(Address address, u32 value) override;

	   private:
		/// helper used for implementing Peek/Poke API
		template <class T>
		constexpr usize AddressToIndex(Address address) {
			return ((address - memoryBase) % memorySize) / sizeof(T);
		}

		Address memoryBase {};

		u8* memory {};
		usize memorySize {};
	};

} // namespace riscv::devices
