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

	-- test device framework
	-- this is how I ideally want to do things,
	-- dunno if it's possible
	self.test_device = LCPUNative.CreateDevice()
	print(self.test_device)
	self.test_device:SetBase(0x12000000)
	self.test_device:SetSize(0x10)
	function self.test_device:Clock()
		print("TestDevice Clock()")
	end

	function self.test_device:Peek(address)
		if address == 0x12000000 then
			-- it a test!
			return CurTime()
		end
		return 0x10000
	end

	function self.test_device:Poke(address, value)
		if address == 0x12000000 then
			print("What you doing?")
		end
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
