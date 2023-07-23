#include <lucore/Logger.hpp>
#include <riscv/Devices/ClntDevice.hpp>
#include <riscv/CPU.hpp>

namespace riscv::devices {

	constexpr static Address MSIP_ADDRESS = ClntDevice::BASE_ADDRESS,
							  MATCHL_ADDRESS = ClntDevice::BASE_ADDRESS + 0x4000,
							  MATCHH_ADDRESS = ClntDevice::BASE_ADDRESS + 0x4004,
							  TIMERL_ADDRESS = ClntDevice::BASE_ADDRESS + 0xbff8,
							  TIMERH_ADDRESS = ClntDevice::BASE_ADDRESS + 0xbffc;

	void ClntDevice::Clock() {
		// TODO: handle timer
		// If match low and high match the timer during a clock
		// we should fire the interrupt, otherwise not do so

		u32 new_timer = timerCountLow + 1;
		if(new_timer < timerCountLow)
			timerCountHigh++;
		timerCountLow = new_timer;

		if((timerCountHigh > timerMatchHigh ||
			(timerCountHigh == timerMatchHigh && timerCountLow == timerMatchLow)) &&
		   (timerMatchHigh || timerMatchLow)) {
			// Fire the CLNT timer interrupt
			bus->GetCPU()->TimerInterrupt();
		}
	}

	u32 ClntDevice::Peek(Address address) {
		switch(address) {
			case TIMERL_ADDRESS:
				return timerCountLow;

			case TIMERH_ADDRESS:
				return timerCountHigh;

			case MATCHL_ADDRESS:
				return timerMatchLow;

			case MATCHH_ADDRESS:
				return timerMatchHigh;

			default:
				return 0x0;
		}
	}

	void ClntDevice::Poke(Address address, u32 value) {
		switch(address) {
			case MATCHL_ADDRESS:
				timerMatchLow = value;
				break;

			case MATCHH_ADDRESS:
				timerMatchHigh = value;
				break;

				// ?
			default:
				lucore::LogInfo("CLNT({}) unhandled poke @ 0x{:08x} : 0x{:08x}",
								static_cast<void*>(this), address, value);
				break;
		}
	}

} // namespace riscv::devices
