AddCSLuaFile()
DEFINE_BASECLASS("base_wire_entity") -- for now?
ENT.PrintName = "LCPU"
ENT.Author = "Lily <3"
-- no more, this deeply uses native APIs
if CLIENT then return end
function ENT:Initialize()
	self:PhysicsInit(SOLID_VPHYSICS)
	self:SetMoveType(MOVETYPE_VPHYSICS)
	self:SetSolid(SOLID_VPHYSICS)
	-- 64 kb of ram for now.
	self.cpu = LCPUNative.CreateCPU(128 * 1024)
	print(self.cpu)
	-- todo: cpu callbacks? once they become a thing
	-- test device framework
	-- (for once something works out how I wanted it to..)
	self.test_device = LCPUNative.CreateDevice(0x11300000, 0x8)
	self.test_device.register = 0x0

	--function self.test_device:Clock()
	--print("TestDevice Clock()")
	--end
	
	function self.test_device:Peek(address)
		--print("peek @ " .. address)
		if address == self.Base then return CurTime() end -- it a test!
		if address == self.Base + 4 then return self.register end

		return 0xffffffff
	end

	function self.test_device:Poke(address, value)
		print("poke of address " .. address .. " -> " .. value)
		if address == self.Base + 4 then
			print("LUAREG write")
			self.register = value
		end
	end

	function self.test_device:Reset()
		print("device was reset")
		-- clear the register
		self.register = 0
	end

	self.cpu:AttachDevice(self.test_device)
end

function ENT:Think()
	-- 
	if not self.cpu:PoweredOn() then return end
	self.cpu:Cycle()
	-- Even though this is gated by tickrate I'm just trying to be nice here
	self:NextThink(CurTime() + 0.1)
end
