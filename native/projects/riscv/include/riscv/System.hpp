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
		static System* Create(Address ramSize);

		~System();

		void Step();

		std::function<void()> OnPowerOff;
		std::function<void()> OnReboot;

		Bus* bus;

		// Required devices.
		CPU* cpu;
		devices::RamDevice* ram;
		devices::SysconDevice* syscon;
		devices::ClntDevice* clnt;

	   private:
		friend struct devices::SysconDevice;

		void SysconPowerOff();
		void SysconReboot();

		System() = default;
	};

} // namespace riscv
