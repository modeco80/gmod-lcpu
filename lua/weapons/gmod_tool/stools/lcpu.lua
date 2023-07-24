-- default vars
TOOL.Mode = "lcpu_lcpu"
TOOL.short_name = "lcpu"
TOOL.Category = "Lily <3"
TOOL.Name = "#tool.lcpu.name"

if CLIENT then
	language.Add("tool.lcpu.name", "LCPU Tool")
	language.Add("tool.lcpu.desc", "Spawns a LCPU")
	language.Add("tool.lcpu.model", "LCPU Model:")
	TOOL.Information = {
		{
			name = "left",
			text = "Spawn or update a LCPU"
		},
	}
	-- { name = "right", text = "Open editor" },
	-- { name = "reload", text = "Attach debugger" },
	-- { name = "reload_shift", text = "Shift+Reload: Clear" },
end

TOOL.ClientConVar = {
	model = "models/cheeze/wires/cpu.mdl",
}

if CLIENT then
	------------------------------------------------------------------------------
	-- Make sure firing animation is displayed clientside
	------------------------------------------------------------------------------
	function TOOL:LeftClick()
		return true
	end

	function TOOL:Reload()
		return true
	end

	function TOOL:RightClick()
		return false
	end
end

if SERVER then
	--function TOOL:Reload(trace)
	--	if trace.Entity:IsPlayer() then return false end
	--	local player = self:GetOwner()

	--	return true
	--end



	function TOOL:LeftClick()
		return true
	end

	-- Right click: open editor
	--function TOOL:RightClick(trace)
	--	return true
	--end
end

if CLIENT then
	------------------------------------------------------------------------------
	-- Build tool control panel
	------------------------------------------------------------------------------
	function TOOL.BuildCPanel(panel)
		print("bruh")
		panel:AddControl( "Header", { Description = "#tool.lcpu.desc" } )
		panel:AddControl(
			"Label",
			{
				Text = "LCPU settings:"
			}
		)

		panel:AddControl(
			"PropSelect",
			{
				Label = "#tool.lcpu.model",
				ConVar = "model",
				Height = 0,
				Models = list.Get("Wire_gate_Models")
			}
		)

		panel:AddControl(
			"Label",
			{
				Text = ""
			}
		)
	end
	--  function TOOL:DrawToolScreen(width, height)
	--      local currentTime = os.date("*t")
	--      CPULib.RenderCPUTool(currentTime.yday % 4,"CPU")
	--  end
end
