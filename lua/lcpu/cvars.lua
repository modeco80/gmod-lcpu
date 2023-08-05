-- these are where all the cvars the addon uses go


-- honestly I wonder if I could get away with models/classes instead of a global limitation
--[[
	c class: basic
		c1: 128k ram, 1 tick, 1024 cycles
		c2: 512k ram, 4 ticks, 2048 cycles

	b class: more suited to crazier programs
		b1: 1mb ram, 8 ticks, 4096 cycles
		b2: 2mb ram, 12 ticks, 6144 cycles

	a class: higher performance
		a1: 4mb ram, 16 ticks, 8192 cycles
		a2: 8mb ram, 16 ticks, 12240 cycles

	(this one will definitly be limited in some way if I ever do this since it is a walking lag machine in the making) 
	s class: you don't like your gmod server very much
		s1: 64mb ram, 16 ticks, 16384 cycles
]]

LCPU.tickCount = 4
LCPU.cycleCount = 1024

-- cvars for limiting/reducing lcpu usage
local lcpu_cpu_tick_count = CreateConVar("lcpu_cpu_tick_count", LCPU.tickCount, FCVAR_REPLICATED, "How many cycle steps a LCPU will do")
local lcpu_cpu_cycle_count = CreateConVar("lcpu_cpu_cycle_count", LCPU.cycleCount, FCVAR_REPLICATED, "How many CPU cycles run in each CPU tick")

cvars.AddChangeCallback(
	"lcpu_cpu_tick_count",
	function()
		-- this seems reasonable enough right?
		LCPU.tickCount = math.Clamp(math.floor(lcpu_cpu_tick_count:GetInt()), 1, 16)
	end
)

cvars.AddChangeCallback(
	"lcpu_cpu_cycle_count",
	function()
		-- these are NOT final values
		LCPU.cycleCount = math.Clamp(math.floor(lcpu_cpu_cycle_count:GetInt()), 1, 16384)
	end
)
