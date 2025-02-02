-- these are where all the (server) cvars the addon uses go

--[[
LCPU.tickCount = 4
LCPU.cycleCount = 1024

-- cvars for limiting/reducing lcpu usage
local lcpu_cpu_tick_count = CreateConVar("lcpu_cpu_tick_count", LCPU.tickCount, FCVAR_REPLICATED, "How many cycle steps a LCPU will do")
local lcpu_cpu_cycle_count = CreateConVar("lcpu_cpu_instructions_per_tick", LCPU.cycleCount, FCVAR_REPLICATED, "How many CPU instructions run in each CPU tick")

cvars.AddChangeCallback(
	"lcpu_cpu_tick_count",				
	function()
		-- this seems reasonable enough right?
		LCPU.tickCount = math.Clamp(math.floor(lcpu_cpu_tick_count:GetInt()), 1, 16)
	end
)

cvars.AddChangeCallback(
	"lcpu_cpu_instructions_per_tick",
	function()
		-- these are NOT final values
		LCPU.cycleCount = math.Clamp(math.floor(lcpu_cpu_cycle_count:GetInt()), 1, 16384)
	end
)

]]
