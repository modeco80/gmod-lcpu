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

	--LCPUNative.EnableDebug()

	AddCSLuaFile("entities/gmod_lcpu_cpu.lua")
end
