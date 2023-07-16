//! Common types
#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>

namespace riscv {

	using u8 = std::uint8_t;
	using s8 = std::int8_t;
	using u16 = std::uint16_t;
	using s16 = std::int16_t;
	using u32 = std::uint32_t;
	using s32 = std::int32_t;
	using u64 = std::uint64_t;
	using s64 = std::int64_t;
	using usize = std::size_t;
	using ssize = std::intptr_t;

	/// A type that can repressent address space or address space offsets.
	using AddressT = u32;

	

} // namespace riscv
