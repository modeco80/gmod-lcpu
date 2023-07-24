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

		// TODO: callbacks for SYSCON PowerOff and Reboot.

		Bus* bus;

		// Required devices.
		CPU* cpu;
		devices::RamDevice* ram;
		devices::SysconDevice* syscon;
		devices::ClntDevice* clnt;

	   private:
		System() = default;
	};

} // namespace riscv
