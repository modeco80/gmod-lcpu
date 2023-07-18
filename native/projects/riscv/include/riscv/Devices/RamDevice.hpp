
#include <riscv/Bus.hpp>

#include "riscv/Types.hpp"

namespace riscv::devices {

	struct RamDevice : public Bus::Device {
		RamDevice(AddressT size);
		virtual ~RamDevice();

		AddressT Size() const override;

		// Implementation of Device interface

		void Attached(Bus* bus, AddressT base) override;

		u8 PeekByte(AddressT address) override;
		u16 PeekShort(AddressT address) override;
		u32 PeekWord(AddressT address) override;

		void PokeByte(AddressT address, u8 value) override;
		void PokeShort(AddressT address, u16 value) override;
		void PokeWord(AddressT address, u32 value) override;

	   private:
		/// helper used for implementing stuff
		template <class T>
		constexpr usize AddressToIndex(AddressT address) {
			return ((address - baseAddress) % memorySize) / sizeof(T);
		}

		// remember what we were attached to via "signal"
		Bus* attachedBus {};
		AddressT baseAddress {};

		u8* memory {};
		usize memorySize {};
	};

} // namespace riscv::devices
