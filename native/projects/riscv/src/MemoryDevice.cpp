#include <riscv/Bus.hpp>

namespace riscv {

	namespace {

		template <bool Rom>
		struct BasicMemoryDevice : public Bus::Device {
			BasicMemoryDevice(usize size) : Bus::Device(), memorySize(size) {
				memory = new u8[size];
				LUCORE_CHECK(memory, "Could not allocate buffer for memory device.");
			}

			virtual ~BasicMemoryDevice() {
				if(memory)
					delete[] memory;
			}

			AddressT Size() const override {
				return memorySize;
			}

			// Implementation of Device interface

			void Attached(Bus* bus, AddressT base) override {
				attachedBus = bus;
				baseAddress = base;
			}

			u8 PeekByte(AddressT address) override {
				return memory[AddressToIndex<u8>(address)];
			}

			u16 PeekShort(AddressT address) override {
				return std::bit_cast<u16*>(memory)[AddressToIndex<u16>(address)];
			}

			u32 PeekWord(AddressT address) override {
				return std::bit_cast<u32*>(memory)[AddressToIndex<u32>(address)];
			}

			void PokeByte(AddressT address, u8 value) override {
				if constexpr(!Rom) {
					memory[AddressToIndex<u8>(address)] = value;
				}
			}

			void PokeShort(AddressT address, u16 value) override {
				if constexpr(!Rom) {
					std::bit_cast<u16*>(memory)[AddressToIndex<u16>(address)] = value;
				}
			}

			void PokeWord(AddressT address, u32 value) override {
				if constexpr(!Rom) {
					std::bit_cast<u32*>(memory)[AddressToIndex<u32>(address)] = value;
				}
			}

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

		using RamDevice = BasicMemoryDevice<false>;
		using RomDevice = BasicMemoryDevice<true>;

	} // namespace

	Bus::Device* NewRam(usize size) {
		return new RamDevice(size);
	}

} // namespace riscv
