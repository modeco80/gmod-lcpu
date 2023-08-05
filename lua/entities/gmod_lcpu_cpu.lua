AddCSLuaFile()
DEFINE_BASECLASS("base_wire_entity")
ENT.PrintName = "LCPU"
ENT.Author = "Lily <3"
-- no more, this deeply uses native APIs
if CLIENT then return end

-- Include the devices which require Wiremod here
-- (hacky, but /shrug)
include("lcpu/devices/wire_interface.lua")

-- TODO: serverside convars to control execution rate & cycle count
function ENT:Initialize()
	self:PhysicsInit(SOLID_VPHYSICS)
	self:SetMoveType(MOVETYPE_VPHYSICS)
	self:SetSolid(SOLID_VPHYSICS)
	-- CPU callbacks?
	self.cpu = LCPUNative.CreateCPU(128 * 1024)
	self.uart = LCPU.Devices.UART(0x10000000)
	self.wireInterface = LCPU.Devices.WireInterface(0x11310000, self, 8, 8)
	self.cpu:AttachDevice(self.uart)
	self.cpu:AttachDevice(self.wireInterface)
	self:SetOverlayText("hi :)")
end

function ENT:Think()
	-- Avoid running if the cpu is not powered on
	if not self.cpu:PoweredOn() then return end

	if LCPU.cycleCount ~= self.cpu.CycleCount then
		--print(string.format("bumping up cycle count to %d", LCPU.cycleCount));
		self.cpu.CycleCount = LCPU.cycleCount
	end

	for i = 1, LCPU.tickCount do
		self.cpu:Cycle()
	end
	
	-- Even though this is gated by tickrate I'm just trying to be nice here
	self:NextThink(CurTime() + 0.1)
end

function ENT:Reset()
	self.cpu:Reset()
end

function ENT:PowerOn()
	self.cpu:PowerOn()
	self:NextThink(CurTime() + 0.1)
end
