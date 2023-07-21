#pragma once

#include <lucore/Assert.hpp>
#include <riscv/Types.hpp>
#include <unordered_map>
#include <vector>

namespace riscv {

	struct CPU;

	/// An bus that is part of a [System].
	/// Note that no virtual address translation is implemented at all.
	/// All addresses/devices of devices in memory are placed in physical addresses.
	struct Bus {
		struct Device {
			enum class BasicType {
				Device,		 // do not upcast.
				PlainMemory, // upcast to MemoryDevice is safe.
				Mmio		 // upcast to MmioDevice is safe.
			};

			Device() = default;
			Device(const Device&) = delete;
			Device(Device&&) = delete;

			virtual ~Device() = default;

			virtual BasicType Type() const { return BasicType::Device; }

			/// Does this device clock?
			virtual bool Clocked() const { return false; }

			/// This function is called by the bus to clock devices.
			virtual void Clock() {}

			template <class T>
			constexpr bool IsA() {
				if(std::is_same_v<T, Bus::MemoryDevice*>) {
					return this->Type() == BasicType::PlainMemory;
				} else if(std::is_same_v<T, Bus::MmioDevice*>) {
					return this->Type() == BasicType::Mmio;
				}

				// Invalid types should do this.
				return false;
			}

			template <class T>
			constexpr T Upcast() {
				LUCORE_ASSERT(IsA<T>(), "Upcast failure: this is not a T");
				return static_cast<T>(this);
			}
		};

		/// Interface plain memory bus devices use.
		struct MemoryDevice : Device {
			virtual ~MemoryDevice() = default;

			virtual BasicType Type() const override { return BasicType::PlainMemory; }

			virtual AddressT Base() const = 0;
			/// How many bytes does this device occupy of address space? This should
			/// effectively be a constant, and should not change during CPU execution.
			virtual AddressT Size() const = 0;

			/// Peek() -> reads a value from this device.
			virtual u8 PeekByte(AddressT address) = 0;
			virtual u16 PeekShort(AddressT address) = 0;
			virtual u32 PeekWord(AddressT address) = 0;

			/// Poke() -> Writes a value to this device's space in memory
			virtual void PokeByte(AddressT address, u8 value) = 0;
			virtual void PokeShort(AddressT address, u16 value) = 0;
			virtual void PokeWord(AddressT address, u32 value) = 0;
		};

		/// A device in the MMIO region.
		/// (0x10000000-0x12000000)
		struct MmioDevice : Device {
			virtual ~MmioDevice() = default;

			virtual BasicType Type() const override { return BasicType::Mmio; }

			virtual AddressT Base() const;

			/// How many bytes does this device occupy of address space? This should
			/// effectively be a constant, and should not change during CPU execution.
			virtual AddressT Size() const = 0;

			virtual u32 Peek(AddressT address) = 0;
			virtual void Poke(AddressT address, u32 value) = 0;
		};

		Bus(CPU* cpu);
		~Bus();

		CPU* GetCPU() { return attachedCpu; }

		/// Attach a device to the bus.
		///
		/// Note that once this function is called (and the device is successfully added),
		/// the object pointed to by [device] is owned by the Bus object, and should not be deleted.
		///
		/// # Returns
		/// This function returns true if the device was able to be put on the bus.
		/// This function returns false in the following error cases:
		/// 	- [device] is a null pointer
		///		- if [device] is a memory device (and thus reserves address space), adding it would
		///		  end up shadowing another previously-added device.
		bool AttachDevice(Device* device);

		/// Clock all clocked devices mapped onto the bus..
		void Clock();

		u8 PeekByte(AddressT address);
		u16 PeekShort(AddressT address);
		u32 PeekWord(AddressT address);

		void PokeByte(AddressT address, u8 value);
		void PokeShort(AddressT address, u16 value);
		void PokeWord(AddressT address, u32 value);

	   private:
		Bus::Device* FindDeviceForAddress(AddressT address) const;

		// TODO: The CPU needs not be a separate member or be treated specially, it too can be a Bus::Device now
		// In fact that would probably be really clean and elegant for calling Step() properly

		CPU* attachedCpu {};

		/// All devices attached to the bus
		std::vector<Device*> devices;

		// TODO: if these end up being a hotpath replace with ankerl::unordered_dense
		std::unordered_map<AddressT, MemoryDevice*> mapped_devices;
		std::unordered_map<AddressT, MmioDevice*> mmio_devices;
	};

} // namespace riscv
