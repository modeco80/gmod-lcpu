#include <riscv/Types.hpp>

#include <vector>
#include <unordered_map>


namespace riscv {

	struct CPU;

	/// An address bus.
	struct Bus {

		struct Device {
			virtual ~Device() = default;

			// How many bytes does this device occupy of address space?
			virtual AddressT Size() const = 0;

			// Used to allow bus devices to know when they are attached to a memory bus,
			// and ultimately, an instance of a System
			virtual void Attached(Bus* memoryBus, AddressT baseAddress) = 0;
			virtual AddressT BaseAddress() const = 0; // TODO(cleanup): Make this non-virtual?


			/// Is this device clocked?
			virtual bool Clocked() const { return false; }

			/// This function is called during clocks to give clocked devices
			/// the ability to update
			virtual void Clock() {}

			// TODO(feat): Need to implement ability to generate interrupts
			// from devices. This needs to be implemented to facilitate the
			// implementation of the timer device as an actual Device implmentation
			// instead of poorly hard-coding it into the CPU core logic.

			// Peek() -> reads a value from this device.
			virtual u8 PeekByte(AddressT offset) = 0;
			virtual u16 PeekShort(AddressT offset) = 0;
			virtual u32 PeekWord(AddressT offset) = 0;

			/// Poke() -> Writes a value to this device's space in memory
			virtual void PokeByte(AddressT offset, u8 value) = 0;
			virtual void PokeShort(AddressT offset, u16 value) = 0;
			virtual void PokeWord(AddressT offset, u32 value) = 0;

		};


		/// Attach a device to the bus.
		///
		/// # Returns
		/// This function returns true if the device was able to be put on the bus.
		/// This function returns false in the following error cases:
		/// 	- [device] is a null pointer
		///		- The provided base address overlaps a already attached device in some way 
		bool AttachDevice(AddressT baseAddress, Device* device);

		/// Clock all clocked devices.
		void Clock();

		// 
		u8 PeekByte(AddressT address);
		u16 PeekShort(AddressT address);
		u32 PeekWord(AddressT address);

		void PokeByte(AddressT address, u8 value);
		void PokeShort(AddressT address, u16 value);
		void PokeWord(AddressT address, u32 value);
	private:

		OptionalRef<Device&> FindDeviceForAddress(AddressT address) const;

		CPU* attachedCpu{};

		// TODO: if this ends up being a hotpath replace with robinhood unordered map
		std::unordered_map<AddressT, Device*> mapped_devices;

		// this is essentially a flat map
		// we don't particularly need hashing :p
		//struct AttachedDevice {
		//	AddressT baseAddress{};
		//	Device* device{};
		//};

		//std::vector<AttachedDevice> mapped_devices{};
	};



}
