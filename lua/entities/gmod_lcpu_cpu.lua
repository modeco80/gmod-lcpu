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

	-- CPU callbacks?
	self.cpu = LCPUNative.CreateCPU(128 * 1024)

	-- UART & GLua test device
	self.uart = LCPU.Devices.UART(0x10000000)
	self.test_device = LCPU.Devices.LuaTest()

	self.cpu:AttachDevice(self.uart)
	self.cpu:AttachDevice(self.test_device)

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
