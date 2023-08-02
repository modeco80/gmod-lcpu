#include <functional> // use function_ref when we get c++23?
#include <riscv/Bus.hpp>
#include <riscv/CPU.hpp>
#include <riscv/Devices/ClntDevice.hpp>
#include <riscv/Devices/RamDevice.hpp>
#include <riscv/Devices/SysconDevice.hpp>

namespace riscv {

	/// a system.
	struct System {
		/// Create a basic system with the basic periphials created.
		/// All other periphials should be managed by the creator of this System
		static lucore::Unique<System> Create(Address ramSize);

		void Step();

		std::function<void()> OnPowerOff;
		std::function<void()> OnReboot;

		lucore::Unique<Bus> bus;

		// Required devices.
		lucore::Unique<CPU> cpu;
		lucore::Unique<devices::RamDevice> ram;
		lucore::Unique<devices::SysconDevice> syscon;
		lucore::Unique<devices::ClntDevice> clnt;

		// call this and you get the door
		System() = default;

	   private:
		friend struct devices::SysconDevice;

		void SysconPowerOff();
		void SysconReboot();

	};

} // namespace riscv
