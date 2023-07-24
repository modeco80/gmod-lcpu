AddCSLuaFile()

-- prime the native lua module if running on the server
if SERVER then
	require("lcpu_native")

	if LCPUNative.ModuleVersion != 1 then
		print("Your LCPU native module is somehow lagging behind the Lua code. Please rebuild it.")
		LCPUNative = nil
		return
	end

	AddCSLuaFile("entities/gmod_lcpu_cpu.lua")
	AddCSLuaFile("lcpu/stool_helper.lua")
end
