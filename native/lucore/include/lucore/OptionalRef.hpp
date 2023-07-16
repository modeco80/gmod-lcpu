//! OptionalRef - std::optional<T&> for C++20

#include <lucore/Assert.hpp>

namespace lucore::detail {

	struct Nullref_t {};

	template <class T>
	struct OptionalRef; // sfinae on non-reference types

	/// Like std::optional<T>, but optimized specifically for references to objects.
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
			LUCORE_ASSERT(HasValue() && "Attempt to access OptionalRef without stored value!");
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
} // namespace lucore::detail

namespace lucore {
	using detail::NullRef;
	using detail::OptionalRef;
} // namespace lucore
