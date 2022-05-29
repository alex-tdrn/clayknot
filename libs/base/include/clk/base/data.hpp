#pragma once

#include <cassert>
#include <cstddef>
#include <type_traits>
#include <typeinfo>
#include <utility>
#include <vector>

#include "clk/util/traits.hpp"

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

	auto is_convertible_to_list() const -> bool
	{
		if(is_empty())
		{
			return false;
		}
		else
		{
			return _vtable->is_convertible_to_list();
		}
	}

	auto as_list() -> std::vector<data<Mutable>>
	{
		if(is_empty())
		{
			return {};
		}
		else
		{
			return _vtable->as_list(*this);
		}
	}

private:
	struct vtable
	{
		std::size_t (*type_hash)();
		bool (*is_convertible_to_list)();
		std::vector<data<Mutable>> (*as_list)(data<Mutable>&);
	};

	vtable const* _vtable = nullptr;
	std::conditional_t<Mutable, void*, void const*> _data = nullptr;

	template <typename T>
	static auto vtable_for() -> vtable const*
	{
		static vtable v = []() -> vtable {
			vtable table;
			table.type_hash = []() -> std::size_t {
				return typeid(T).hash_code();
			};

			if constexpr(clk::is_range_v<T>)
			{
				table.is_convertible_to_list = []() -> bool {
					return true;
				};

				table.as_list = [](data<Mutable>& instance) -> std::vector<data<Mutable>> {
					std::vector<data<Mutable>> result;
					for(auto& element : *static_cast<std::conditional_t<Mutable, T*, T const*>>(instance.pointer()))
					{
						result.push_back(&element);
					}
					return result;
				};
			}
			else
			{
				table.is_convertible_to_list = []() -> bool {
					return false;
				};

				table.as_list = [](data<Mutable>&) -> std::vector<data<Mutable>> {
					return {};
				};
			}
		}();
		return &v;
	}
};

} // namespace impl

using mutable_data = impl::data<true>;
using const_data = impl::data<false>;

} // namespace clk