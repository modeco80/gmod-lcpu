#pragma once

#include <riscv/Bus.hpp>

namespace riscv::devices {

	/// A block of RAM which can be used by the CPU.
	struct RamDevice : public Bus::MemoryDevice {
		RamDevice(AddressT base, AddressT size);
		virtual ~RamDevice();

		// Implementation of Device interface

		AddressT Base() const override;
		AddressT Size() const override;


		u8 PeekByte(AddressT address) override;
		u16 PeekShort(AddressT address) override;
		u32 PeekWord(AddressT address) override;

		void PokeByte(AddressT address, u8 value) override;
		void PokeShort(AddressT address, u16 value) override;
		void PokeWord(AddressT address, u32 value) override;

	   private:
		/// helper used for implementing Peek/Poke API
		template <class T>
		constexpr usize AddressToIndex(AddressT address) {
			return ((address - memoryBase) % memorySize) / sizeof(T);
		}

		AddressT memoryBase {};

		u8* memory {};
		usize memorySize {};
	};

} // namespace riscv::devices
