
LCPU.ModelClass = {
	MODELCLASS_M = 0,
	MODELCLASS_C = 1,
	MODELCLASS_B = 2,
	MODELCLASS_S = 3,
}

LCPU.ModelDescriptions = {
	[LCPU.ModelClass.MODELCLASS_M] = "A tiny microcontroller.",
	[LCPU.ModelClass.MODELCLASS_C] = "A basic general-purpose CPU for low performance tasks.",
	[LCPU.ModelClass.MODELCLASS_B] = "A higher performance CPU with more memory.",
	[LCPU.ModelClass.MODELCLASS_S] = "An adminstrator only CPU type with the maximum possible speed. Use sparingly.",
}

LCPU.Models = {
	[LCPU.ModelClass.MODELCLASS_M] = {
		[0] = {
			name = "M1",
			memorySize = 128 * 1024,
			ticksPerServerTick = 1,
			instructionsPerTick = 128
		},

		[1] = {
			name = "M2",
			memorySize = 256 * 1024,
			ticksPerServerTick = 2,
			instructionsPerTick = 128
		}
	},

	[LCPU.ModelClass.MODELCLASS_C] = {
		[0] = {
			name = "C1",
			memorySize = 1024 * 1024,
			ticksPerServerTick = 4,
			instructionsPerTick = 512
		},

		[1] = {
			name = "C2",
			memorySize = 2 * (1024 * 1024),
			ticksPerServerTick = 8,
			instructionsPerTick = 1024
		}
	},


	[LCPU.ModelClass.MODELCLASS_B] = {
		[0] = {
			name = "B1",
			memorySize = 4 * (1024 * 1024),
			ticksPerServerTick = 12,
			instructionsPerTick = 2048
		},

		[1] = {
			name = "B2",
			memorySize = 8 * (1024 * 1024),
			ticksPerServerTick = 16,
			instructionsPerTick = 4096
		}
	},

	[LCPU.ModelClass.MODELCLASS_S] = {
		[0] = {
			name = "S1",
			memorySize = 32 * (1024 * 1024),
			ticksPerServerTick = 16,
			instructionsPerTick = 16384
		},

		[0] = {
			name = "S2",
			memorySize = 64 * (1024 * 1024),
			ticksPerServerTick = 16,
			instructionsPerTick = 16384
		},
	},

}
