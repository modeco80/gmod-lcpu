#include <cstdint>
#include <memory>

using u8 = std::uint8_t;
using i8 = std::int8_t;
using u16 = std::uint16_t;
using i16 = std::int16_t;
using u32 = std::uint32_t;
using i32 = std::int32_t;
using u64 = std::uint64_t;
using i64 = std::int64_t;
using usize = std::size_t;
using isize = std::intptr_t;

 namespace lucore {
	template<class T, class Deleter = std::default_delete<T>>
	using Unique = std::unique_ptr<T, Deleter>;

	template<class T>
	using Ref = std::shared_ptr<T>;

} // namespace lucore
