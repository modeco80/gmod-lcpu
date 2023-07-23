#include <riscv/Bus.hpp>
#include <riscv/CPU.hpp>
#include <riscv/Devices/RamDevice.hpp>
#include <riscv/Devices/SysconDevice.hpp>
#include <riscv/Devices/ClntDevice.hpp>

namespace riscv {

	/// a system.
	struct System {
		/// Create a basic system with the basic periphials created.
		/// All other periphials should be managed by the creator of this System
		static System* WithMemory(AddressT ramSize);

		~System();

		void AddDeviceToBus(Bus::Device* device);

		/// returns false if the cpu broke execution
		bool Step();

		CPU* GetCPU();
		Bus* GetBus();

	   private:
		/// How many Cycle() calls will the bus get
		/// (also decides ipsRate)
		u32 cycleRate;

		/// How many instructions will the CPU execute each step
		u32 ipsRate;

		Bus* bus;

		// Required devices.
		CPU* cpu;
		devices::RamDevice* ram;
		devices::SysconDevice* syscon;
		devices::ClntDevice* clnt;
	};

} // namespace riscv
