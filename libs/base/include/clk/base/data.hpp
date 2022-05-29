#pragma once

#include <cassert>
#include <cstddef>
#include <typeinfo>

namespace clk
{

class data
{
public:
	template <typename T>
	data(T* v) : _vtable(vtable_for<T>()), _data(v)
	{
	}

	data() = default;
	data(data const&) = default;
	data(data&&) = default;
	auto operator=(data const&) -> data& = default;
	auto operator=(data&&) -> data& = default;
	~data() = default;

	auto pointer() const -> void const*
	{
		return _data;
	}

	auto pointer() -> void*
	{
		return _data;
	}

	auto type_hash() const -> std::size_t
	{
		assert(_vtable != nullptr && _data != nullptr);
		return _vtable->type_hash();
	}

private:
	struct vtable
	{
		std::size_t (*type_hash)();
	};

	vtable const* _vtable = nullptr;
	void* _data = nullptr;

	template <typename T>
	static auto vtable_for() -> vtable const*
	{
		static vtable vtable = {[]() -> std::size_t {
			return typeid(T).hash_code();
		}};
		return &vtable;
	}
};

class data_list;

} // namespace clk