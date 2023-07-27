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

----[[
	testobj = LCPUNative.CreateTest()
	print(testobj:Test())
	print(testobj.Variable)

	testobj.MemberVariable = 32.9
	print(testobj.MemberVariable)

	testobj.Variable = 32.1 -- this should fial
	testobj.Test = nil;

	print(testobj.Variable)

	print(testobj.Name)
--]]

	-- rapid iteration requires rapid solutions
	--[[
	device = LCPUNative.CreateDevice(0x100000f0, 0x10)
	device.a = 12
	device.apple = {}

	--print(device)
	--print(device.a)
	--print(device.apple)

function device:Clock() 
	print("a")
end

function device:Peek(address)
	print("peek @ " .. address)
	if address == 0x100000f0 then return 0x1010 end -- it a test!

	return 0x10000
end

function device:Poke(address, value)
	if address == 0x100000f0 then
		print("What you doing?")
	end
end

function device:Reset()
	print("device was reset")
end

	cpu = LCPUNative.CreateCPU(64*1024)

	cpu:AttachDevice(device)

	print(device.Clock)

	print(cpu)

	-- unrolled test loops ftw
	cpu:Cycle();cpu:Cycle();cpu:Cycle();cpu:Cycle();
	cpu:Cycle();cpu:Cycle();cpu:Cycle();cpu:Cycle();
	cpu:Cycle();cpu:Cycle();cpu:Cycle();cpu:Cycle();
	cpu:Cycle();cpu:Cycle();cpu:Cycle();cpu:Cycle();
]]
	AddCSLuaFile("entities/gmod_lcpu_cpu.lua")
end
