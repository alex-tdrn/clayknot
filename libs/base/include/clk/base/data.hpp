#pragma once

#include <cassert>
#include <cstddef>
#include <type_traits>
#include <typeinfo>
#include <utility>

namespace clk
{

namespace impl
{

template <bool Mutable>
class data
{
public:
	template <typename T>
	data(T* v) : _vtable(vtable_for<T>()), _data(v)
	{
	}

	data() = default;
	data(data const&) = default;
	data(data&&) noexcept = default;
	auto operator=(data const&) -> data& = default;
	auto operator=(data&&) noexcept -> data& = default;
	~data() = default;

	auto is_empty() const -> bool
	{
		return _data == nullptr;
	}

	auto pointer()
	{
		return _data;
	}

	auto type_hash() const -> std::size_t
	{
		if(is_empty())
		{
			return 0;
		}
		else
		{
			return _vtable->type_hash();
		}
	}

private:
	struct vtable
	{
		std::size_t (*type_hash)();
	};

	vtable const* _vtable = nullptr;
	std::conditional_t<Mutable, void*, void const*> _data = nullptr;

	template <typename T>
	static auto vtable_for() -> vtable const*
	{
		static vtable vtable = {[]() -> std::size_t {
			return typeid(T).hash_code();
		}};
		return &vtable;
	}
};

} // namespace impl

using mutable_data = impl::data<true>;
using const_data = impl::data<false>;

} // namespace clk