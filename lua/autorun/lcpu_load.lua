AddCSLuaFile()
-- prime the native lua module & add clientside files to send
-- if running on the server
if SERVER then
	require("lcpu_native")
	if LCPUNative.ModuleVersion ~= 1 then
		print("Your LCPU native module is somehow lagging behind the Lua code. Please rebuild it.")
		LCPUNative = nil
		return
	end

	LCPU = {};
	LCPU.Devices = {};

	-- Uncomment this to enable debug logging (useful for troubleshooting bugs)
	--LCPUNative.EnableDebug()

	include("lcpu/cvars.lua")

	AddCSLuaFile("entities/gmod_lcpu_cpu.lua")

	-- Serverside devices (that don't depend on wiremod being loaded)
	include("lcpu/devices/uart.lua")
end
