#include <lucore/OptionalRef.hpp>
#include <riscv/Types.hpp>
#include <unordered_map>
#include <vector>

namespace riscv {

	struct CPU;

	/// An address/memory bus. No virtual address translation is implemented;
	/// all addresses/devices are placed in physical addresses.
	struct Bus {
		/// Interface all memory bus devices use.
		struct Device {
			Device() = default;
			
			// Devices have no need to be copied or moved.
			Device(const Device&) = delete;
			Device(Device&&) = delete;

			virtual ~Device() = default;

			/// How many bytes does this device occupy of address space? This should
			/// effectively be a constant, and should probably not change during CPU execution.
			virtual AddressT Size() const = 0;

			/// Used to allow bus devices to know when they are attached to a memory bus,
			/// and ultimately, an instance of a System
			virtual void Attached(Bus* memoryBus, AddressT baseAddress) = 0;

			/// Does this device require a clock "signal"?
			virtual bool Clocked() const { return false; }

			/// This function is called to give clocked devices
			/// the ability to... well, clock!
			virtual void Clock() {}

			// TODO(feat): default implementations of Peek* and Poke* should exist
			// and trap the CPU (similarly to what happens if a unmapped bus read occurs).

			/// Peek() -> reads a value from this device.
			virtual u8 PeekByte(AddressT offset) = 0;
			virtual u16 PeekShort(AddressT offset) = 0;
			virtual u32 PeekWord(AddressT offset) = 0;

			/// Poke() -> Writes a value to this device's space in memory
			virtual void PokeByte(AddressT offset, u8 value) = 0;
			virtual void PokeShort(AddressT offset, u16 value) = 0;
			virtual void PokeWord(AddressT offset, u32 value) = 0;
		};

		Bus(CPU* cpu);
		~Bus();

		/// Attach a device to the bus.
		///
		/// Note that once this function is called (and the device is successfully added),
		/// the object pointed to by [device] is owned by the Bus object, and should not be deleted.
		///
		/// # Returns
		/// This function returns true if the device was able to be put on the bus.
		/// This function returns false in the following error cases:
		/// 	- [device] is a null pointer
		///		- The provided base address overlaps a already attached device in some way
		bool AttachDevice(AddressT baseAddress, Device* device);

		/// Clock all clocked devices mapped onto the bus..
		void Clock();

		u8 PeekByte(AddressT address);
		u16 PeekShort(AddressT address);
		u32 PeekWord(AddressT address);

		void PokeByte(AddressT address, u8 value);
		void PokeShort(AddressT address, u16 value);
		void PokeWord(AddressT address, u32 value);

	   private:
		lucore::OptionalRef<Device> FindDeviceForAddress(AddressT address) const;

		CPU* attachedCpu {};

		// TODO: if this ends up being a hotpath replace with ankerl::unordered_dense
		std::unordered_map<AddressT, Device*> mapped_devices;
	};

} // namespace riscv
