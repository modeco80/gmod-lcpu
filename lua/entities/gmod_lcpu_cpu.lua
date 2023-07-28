AddCSLuaFile()
DEFINE_BASECLASS("base_wire_entity")
ENT.PrintName = "LCPU"
ENT.Author = "Lily <3"

-- no more, this deeply uses native APIs
if CLIENT then return end

-- TODO: serverside convars to control execution rate & cycle count

function ENT:Initialize()
	self:PhysicsInit(SOLID_VPHYSICS)
	self:SetMoveType(MOVETYPE_VPHYSICS)
	self:SetSolid(SOLID_VPHYSICS)
	self.cpu = LCPUNative.CreateCPU(128 * 1024)

	-- UART
	self.uart = LCPUNative.CreateDevice(0x10000000, 0xc)
	self.uart.buffer = ""

	function self.uart:Peek(address)
		if address == self.Base then return 0 end
		if address == self.Base + 5 then return 0x60 end -- 
		return 0xffffffff
	end

	function self.uart:Poke(address, value)
		if address == self.Base then
			local c = bit.band(value, 0x000000ff)
			if c == 0 then return end

			-- Newline, reset the buffer
			if c == 0xa then
				print(self.buffer)
				self:Reset()
			else
				-- Not a newline so we can keep going with it
				self.buffer = self.buffer .. string.char(c)
			end
		end
	end

	function self.uart:Reset()
		self.buffer = ""
	end

	-- todo: cpu callbacks? once they become a thing
	-- test device framework
	-- (for once something works out how I wanted it to..)
	self.test_device = LCPUNative.CreateDevice(0x11300000, 0x8)
	self.test_device.register = 0x0

	function self.test_device:Peek(address)
		--print(string.format("TestDevice:Peek @ 0x%08x", address))
		if address == self.Base then return CurTime() end -- it a test!
		if address == self.Base + 4 then return self.register end

		return 0xffffffff
	end

	function self.test_device:Poke(address, value)
		--print(string.format("TestDevice:Poke @ 0x%08x -> 0x%08x", address, value))
		if address == self.Base + 4 then
			--print("LUAREG write")
			self.register = value
		end
	end

	function self.test_device:Reset()
		--print("TestDevice:Reset")
		-- clear the register
		self.register = 0
	end


	self.cpu:AttachDevice(self.uart);
	self.cpu:AttachDevice(self.test_device);

	self:SetOverlayText("hi :)")
end

function ENT:Think()
	-- Avoid running if the cpu is not powered on
	if not self.cpu:PoweredOn() then return end
	-- Even though this is gated by tickrate I'm just trying to be nice here
	self:NextThink(CurTime() + 0.1)
	self.cpu:Cycle()
end

function ENT:PowerOn()
	self.cpu:PowerOn()
	self:NextThink(CurTime() + 0.1)
end
