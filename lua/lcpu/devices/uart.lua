-- UART device
-- For now, this just prints to the server console.
function LCPU.Devices.UART(base)
	local uart = LCPUNative.CreateDevice(base, 0xc)
	uart.buffer = ""

	function uart:Peek(address)
		if address == self.Base then return 0 end
		if address == self.Base + 5 then return 0x60 end -- Active, but no keyboard input
		return 0xffffffff
	end

	function uart:Poke(address, value)
		if address == self.Base then
			local c = bit.band(value, 0x000000ff)
			if c == 0 then return end

			-- On newline or reaching length limit
			-- print the buffer and then reset it
			if c == 0xa or #self.buffer >= 256 then
				print(string.format("UART: %s", self.buffer))
				self:Reset()
			else
				-- Not a newline so we can keep going with it
				self.buffer = self.buffer .. string.char(c)
			end
		end
	end

	function uart:Reset()
		self.buffer = ""
	end

	return uart
end
