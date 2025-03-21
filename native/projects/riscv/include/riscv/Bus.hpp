#pragma once

#include <lucore/Assert.hpp>
#include <riscv/CPUTypes.hpp>
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
				Cpu,		 // upcast to CPU is safe.
				PlainMemory, // upcast to MemoryDevice is safe.
				Mmio		 // upcast to MmioDevice is safe.
			};

			Device() = default;
			Device(const Device&) = delete;
			Device(Device&&) = delete;

			virtual ~Device() = default;

			virtual void Attached(Bus* bus) { this->bus = bus; }

			virtual BasicType Type() const { return BasicType::Device; }

			/// Does this device clock?
			virtual bool Clocked() const { return false; }

			/// This function is called by the bus to clock devices.
			virtual void Clock() {}

			virtual void Reset() {}

			template <class T>
			constexpr bool IsA() {
				if constexpr(std::is_same_v<T, CPU*>) {
					return this->Type() == BasicType::Cpu;
				} else if constexpr(std::is_same_v<T, Bus::MemoryDevice*>) {
					return this->Type() == BasicType::PlainMemory;
				} else if constexpr(std::is_same_v<T, Bus::MmioDevice*>) {
					return this->Type() == BasicType::Mmio;
				} else {
					// Invalid types should do this.
					return false;
				}
			}

			template <class T>
			constexpr T Upcast() {
				LUCORE_ASSERT(IsA<T>(), "Upcast failure: this is not a T");
				return static_cast<T>(this);
			}

		   protected:
			/// The bus this device is attached to.
			Bus* bus;
		};

		/// Interface plain memory bus devices use.
		struct MemoryDevice : Device {
			virtual ~MemoryDevice() = default;

			virtual BasicType Type() const override { return BasicType::PlainMemory; }

			virtual Address Base() const = 0;

			/// How many bytes does this device occupy of address space?
			/// This should not change during execution.
			virtual Address Size() const = 0;

			/// Peek() -> reads a value from this device.
			virtual u8 PeekByte(Address address) = 0;
			virtual u16 PeekShort(Address address) = 0;
			virtual u32 PeekWord(Address address) = 0;

			/// Poke() -> Writes a value to this device's space in memory
			virtual void PokeByte(Address address, u8 value) = 0;
			virtual void PokeShort(Address address, u16 value) = 0;
			virtual void PokeWord(Address address, u32 value) = 0;
		};

		/// A device in the MMIO region.
		/// (0x10000000-0x12000000)
		struct MmioDevice : Device {
			virtual ~MmioDevice() = default;

			virtual BasicType Type() const override { return BasicType::Mmio; }

			virtual Address Base() const = 0;

			/// How many bytes does this device occupy of address space?
			/// This should not change during execution.
			virtual Address Size() const = 0;

			virtual u32 Peek(Address address) = 0;
			virtual void Poke(Address address, u32 value) = 0;
		};

		/// Attach a device to the bus.
		///
		/// Note that once this function is called (and the device is successfully added),
		/// the object pointed to by [device] should last at least as long as the bus.
		/// The bus does not own objects attached to it.
		///
		/// # Returns
		/// This function returns true if the device was able to be put on the bus.
		/// This function returns false in the following error cases:
		/// 	- [device] is a null pointer
		///		- If [device] is a memory device (and thus reserves some address space), adding it
		///		  to the address space would end up shadowing another previously-added device.
		bool AttachDevice(Device* device);

		/// Clock all clocked devices mapped onto the bus..
		void Clock();
		void Reset();

		u8 PeekByte(Address address);
		u16 PeekShort(Address address);
		u32 PeekWord(Address address);

		void PokeByte(Address address, u8 value);
		void PokeShort(Address address, u16 value);
		void PokeWord(Address address, u32 value);

		CPU* GetCPU() { return cpu; }

	   private:
		// TODO: version which takes Device::BasicType
		Bus::Device* FindDeviceForAddress(Address address) const;

		CPU* cpu;

		/// All plain memory or mmio devices attached to the bus
		std::vector<Device*> devices;

		// TODO: if these end up being a hotpath replace with ankerl::unordered_dense
		// (or just use the [devices] vector, probably.)
		std::unordered_map<Address, MemoryDevice*> mapped_devices;
		std::unordered_map<Address, MmioDevice*> mmio_devices;
	};

} // namespace riscv
