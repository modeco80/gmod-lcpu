//! OptionalRef - std::optional<T&> for C++20
#pragma once

#include <lucore/Assert.hpp>

namespace lucore::detail {

	struct Nullref_t {};

	/// Sentinel value to explicitly not populate an OptionalRef.
	inline static Nullref_t NullRef {};

	/// Like std::optional<T>, but optimized specifically for references to objects.
	///
	/// Additionally, as a bonus, since the repressentation is so simple, this is
	/// constexpr-safe by default, so it can be relatively well optimized.
	///
	/// # Safety
	/// Treat this class like [std::reference_wrapper].
	/// Do *not* give this class invalid references.
	template <class T>
	struct OptionalRef {
		constexpr OptionalRef() : ptr(nullptr) {
		}

		// trigger explicit null construction
		constexpr OptionalRef(Nullref_t) : OptionalRef() {
		}

		constexpr OptionalRef(T& ref) : ptr(&ref) {
		}

		// polymorphic downconstruction from another OptionalRef<U>
		template <class U>
		constexpr OptionalRef(const OptionalRef<U>& other) : ptr(&other.ptr) {
		}

		constexpr T& Value() const {
			// this is a CHECK() since allowing unchecked access in release builds is probably a
			// very very bad idea
			LUCORE_CHECK(HasValue(), "Attempt to access OptionalRef without stored value!");
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

} // namespace lucore::detail

namespace lucore {
	using detail::NullRef;	   // NOLINT ..yes, yes they are used. why do you think they aren't.
	using detail::OptionalRef; // NOLINT ... ditto!!!
} // namespace lucore
