-- skeleton load file to get gmod to recognize this as an addon
-- this will contain files later on in life.

print("hello world?")

-- detect if wiremod is installed
if not istable(WireLib) then
	print("[LCPU] we need wiremod..")
	return
end
