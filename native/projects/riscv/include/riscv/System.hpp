#include <riscv/Bus.hpp>
#include <riscv/Devices/RamDevice.hpp>

namespace riscv {

	// fwd decls
	struct CPU;

	/// a system.
	struct System {

		/// Create
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

		// Most of our basic required devices.
		CPU* cpu;
		Bus* bus;
		devices::RamDevice* ram;
	}

} // namespace riscv
