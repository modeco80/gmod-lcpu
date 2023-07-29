-- Basic Wiremod interface device.

function LCPU.Devices.WireInterface(base, entity, nrInputs, nrOutputs)
	local HEADER_SIZE = 8
	local function MmioSize()
		return HEADER_SIZE + (nrInputs * 4) + (nrOutputs * 4)
	end

	local device = LCPUNative.CreateDevice(base, MmioSize())
	device.data = {
		entity = entity,
		nrInputs = nrInputs,
		nrOutputs = nrOutputs
	}

	function device:AddressToIndex(address)
		return ((address - self.Base) / 4) - 1
	end

	function device:InitWireStuff()
		local inputNames = {}
		local outputNames = {}

		for i = 1, self.data.nrInputs do
			inputNames[i] = string.format("Input%d", i)
		end

		for i = 1, self.data.nrOutputs do
			outputNames[i] = string.format("Output%d", i)
		end

		-- this will also pollute the attached entity but that's fine
		self.data.wireInputs = WireLib.CreateSpecialInputs(self.data.entity, inputNames, {}, {})
		self.data.wireOutputs = WireLib.CreateSpecialOutputs(self.data.entity, outputNames, {}, {})
	end

	function device:Peek(address)
		if address == self.Base then return self.data.nrInputs end
		if address == self.Base + 4 then return self.data.nrOutputs end

		local inputIndex = self:AddressToIndex(address)
		if inputIndex > self.data.nrInputs then
			-- Invalid input register read or trying to read an output register
			return 0xffffffff
		end

		return self.data.wireInputs[string.format("Input%d", inputIndex)].Value
	end

	function device:Poke(address, value)
		if address == self.Base or address == self.Base + 4 then return end -- Don't allow writing read-only registers
		local outputIndex = self:AddressToIndex(address) - self.data.nrInputs
		if outputIndex > self.data.nrInputs then
			return
		end
		WireLib.TriggerOutput(self.data.entity, string.format("Output%d", outputIndex), value)
	end

	function device:Reset()
		for i = 1, self.data.nrOutputs do
			WireLib.TriggerOutput(self.data.entity, string.format("Output%d", i), 0)
		end
	end

	device:InitWireStuff()
	return device
end
