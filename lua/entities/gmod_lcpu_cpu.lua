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
end

function ENT:Think()
	-- 
	if not self.cpu:PoweredOn() then return end

	self.cpu:Cycle()
	-- Even though this is gated by tickrate I'm just trying to be nice here
	self:NextThink(CurTime() + 0.1)
end
