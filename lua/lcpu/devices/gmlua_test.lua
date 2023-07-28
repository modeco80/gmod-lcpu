-- Lua test device. This'll probably get removed soon, this is just for testing
-- that Lua->C++ interop actually works like it should

function LCPU.Devices.LuaTest()
	local test_device = LCPUNative.CreateDevice(0x11300000, 0x8)
	test_device.register = 0x0

	function test_device:Peek(address)
		--print(string.format("TestDevice:Peek @ 0x%08x", address))
		if address == self.Base then return CurTime() end -- it a test!
		if address == self.Base + 4 then return self.register end

		return 0xffffffff
	end

	function test_device:Poke(address, value)
		--print(string.format("TestDevice:Poke @ 0x%08x -> 0x%08x", address, value))
		if address == self.Base + 4 then
			--print("LUAREG write")
			self.register = value
		end
	end

	function test_device:Reset()
		--print("TestDevice:Reset")
		-- clear the register
		self.register = 0
	end

	return test_device
end
