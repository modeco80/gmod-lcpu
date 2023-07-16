//! Lucore Assert Wrappers

// this just plumbs everything to libc by default
// your project can choose to configure its own assertion handler by
// defining LUCORE_ASSERT()
// TODO: LUCORE_VERIFY/plumbing this into our own assertion code?
#ifndef LUCORE_ASSERT
#include <cassert>

#ifdef NDEBUG
	#define LUCORE_ASSERT(...) assert(__VA_ARGS__)
#else
	#define LUCORE_ASSERT(...)
#endif

#endif
