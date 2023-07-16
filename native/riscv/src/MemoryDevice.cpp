#include <riscv/Bus.hpp>
#include "riscv/Types.hpp"

#include <span>

namespace riscv {

	namespace {


		template<bool Rom>
		struct BasicMemoryDevice : public Bus::Device {
			BasicMemoryDevice(usize size)
				: memorySize(size) {
				memory = new u8[size];
				// TODO(feat): we should have a global panic system which is hooked in
				// so that we don't just blindly crash everything
				assert(memory && "Out of host memory");
			}

			virtual ~BasicMemoryDevice() {
				if(memory)
					delete[] memory;
			}

			// Implementation of Device interface

			void Attached(Bus* bus, AddressT base) override {
				attachedBus = bus;
				baseAddress = base;
			}

			AddressT BaseAddress() const override {
				return baseAddress;
			}

			u8 PeekByte(AddressT offset) override {
				return memory[offset % memorySize];
			}

			u16 PeekShort(AddressT offset) override {
				return std::bit_cast<u16*>(memory)[OffsetToIndex<u16>(offset)];
			}

			u32 PeekWord(AddressT offset) override {
				return  std::bit_cast<u32*>(memory)[OffsetToIndex<u32>(offset)];
			}

			void PokeByte(AddressT offset, u8 value) override {
				if constexpr(!Rom) {
					memory[offset % memorySize] = value;
				} else {
					// TODO: trap here
				}
			}

			void PokeShort(AddressT offset, u16 value) override {
				if constexpr(!Rom) {
					std::bit_cast<u16*>(memory)[OffsetToIndex<u16>(offset)] = value;
				} else {
					// TODO: trap here
				}
			}

			void PokeWord(AddressT offset, u32 value) override {
				if constexpr(!Rom) {
					std::bit_cast<u32*>(memory)[OffsetToIndex<u32>(offset)] = value;
				} else {
					// TODO: trap here
				}
			}

		private:


			/// helper used for implementing stuff
			template<class T>
			constexpr usize OffsetToIndex(AddressT offset) {
				return (offset % memorySize) / sizeof(T);
			}

			// remember what we were attached to via "signal"
			Bus* attachedBus{};
			AddressT baseAddress{};

			u8* memory{};
			usize memorySize{};
		};

		using RamDevice = BasicMemoryDevice<false>;
		using RomDevice = BasicMemoryDevice<true>;

	}


	//Bus::Device* NewRam()
	
}
