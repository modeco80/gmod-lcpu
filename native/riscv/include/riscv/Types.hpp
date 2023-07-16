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

	namespace detail {

		struct Nullref_t {};

		template <class T>
		struct OptionalRef; // sfinae on non-reference types

		/// Like std::optional, but for references to objects.
		///
		/// Additionally, as a bonus, since the repressentation is so simple, this is
		/// constexpr-safe by default, so it can be relatively well optimized.
		///
		/// # Safety
		/// Treat this class like [std::reference_wrapper].
		/// Do *not* give this class invalid references.
		template <class T>
		struct OptionalRef<T&> {
			constexpr OptionalRef() : ptr(nullptr) {
			}

			// trigger explicit null construction
			constexpr OptionalRef(Nullref_t) : OptionalRef() {
			}

			constexpr OptionalRef(T& ref) : ptr(&ref) {
			}

			// polymorphic downconstruction from another OptionalRef<U>
			template <class U>
			constexpr OptionalRef(const OptionalRef<U&>& other) : ptr(&other.ptr) {
			}

			constexpr T& Value() const {
				assert(HasValue() && "Attempt to access OptionalRef without stored value!");
				return *ptr;
			}

			constexpr bool HasValue() const {
				return ptr != nullptr;
			}

			constexpr T& operator*() const {
				return Value();
			}

			// unchecked access: DO NOT use this without checking beforehand
			constexpr T* operator->() const {
				return ptr;
			}

			constexpr operator bool() const {
				return HasValue();
			}

		   private:
			T* ptr {};
		};

		/// Sentinel value to explicitly not populate an OptionalRef.
		inline static Nullref_t NullRef {};
	} // namespace detail

	using detail::NullRef;
	using detail::OptionalRef;

} // namespace riscv
