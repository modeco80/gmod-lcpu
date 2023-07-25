-- TODO: I'm not sure if I should use the wirelib stuff
-- I depend on it anyways but /shrug
WireToolSetup.setCategory("Chips, Gates", "Advanced")
WireToolSetup.open("lcpu", "LCPU", "gmod_lcpu_cpu", nil, "LCPUs")
if CLIENT then
	language.Add("Tool.wire_lcpu.name", "LCPU Tool")
	language.Add("Tool.wire_lcpu.desc", "Spawns a LCPU")
	language.Add("ToolWirelcpu_Model", "Model:")
	TOOL.Information = {
		{
			name = "left",
			text = "Create/Update LCPU"
		},
	}
	--{ name = "right", text = "Open editor" },
	--{ name = "reload", text = "Attach debugger" },
	--{ name = "reload_shift", text = "Shift+Reload: Clear" },
end

WireToolSetup.BaseLang()
WireToolSetup.SetupMax(7)
TOOL.ClientConVar = {
	model = "models/cheeze/wires/cpu.mdl",
}

if CLIENT then
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
	function TOOL:Reload(trace)
		if trace.Entity:IsPlayer() then return false end
		local player = self:GetOwner()

		return true
	end

	-- Left click: spawn CPU or upload current program into it
	function TOOL:CheckHitOwnClass(trace)
		return trace.Entity:IsValid() and (trace.Entity:GetClass() == self.WireClass)
	end

	function TOOL:LeftClick_Update(trace)
	end

	function TOOL:MakeEnt(ply, model, Ang, trace)
		local ent = WireLib.MakeWireEnt(
			ply,
			{
				Class = self.WireClass,
				Pos = trace.HitPos,
				Angle = Ang,
				Model = model
			}
		)

		self:LeftClick_Update(trace)

		return ent
	end

	-- Right click: open editor
	function TOOL:RightClick(trace)
		return true
	end
end

if CLIENT then
	function TOOL.BuildCPanel(panel)
		local modelPanel = WireDermaExts.ModelSelect(panel, "lcpu_cpu_model", list.Get("Wire_gate_Models"), 2)
		panel:AddControl(
			"Label",
			{
				Text = ""
			}
		)
	end
end
