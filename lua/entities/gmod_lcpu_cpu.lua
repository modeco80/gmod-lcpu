AddCSLuaFile()
DEFINE_BASECLASS("base_wire_entity")
ENT.PrintName = "LCPU"
ENT.Author = "Lily <3"
-- no more, this deeply uses native APIs
if CLIENT then return end

-- Include the devices which require Wiremod here
-- (hacky, but /shrug)
include("lcpu/devices/wire_interface.lua")

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
	if not self.cpu:PoweredOn() then return end

	-- update instructions per tick if it changed
	if LCPU.cycleCount ~= self.cpu.InstructionsPerTick then
		self.cpu.InstructionsPerTick = LCPU.cycleCount
	end

	-- Run for the amount of ticks
	for i = 1, LCPU.tickCount do
		self.cpu:Cycle()
	end
	
	-- this is gated by tickrate
	self:NextThink(CurTime())
end

function ENT:Reset()
	self.cpu:Reset()
end

function ENT:PowerOn()
	self.cpu:PowerOn()
	self:NextThink(CurTime())
end
