# Notes

- gmsv_lcpu doesn't use the upstream cmake buildsystem for gmod lua headers (instead crafting our own)
	- this is because, in facepunchs infinite wisdom, they decided to unconditionally build the examples.
