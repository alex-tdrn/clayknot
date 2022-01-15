#pragma once

#include <functional>
#include <optional>
#include <stdexcept>
#include <variant>

namespace clk::gui
{
template <typename T>
class data_writer
{
public:
	data_writer() = default;
	// would be nice to just pass the variants directly here and have a single constructor
	//  but then the type cannot be deduced automatically, and I can't get deduction guides to do what I want
	data_writer(T* data_pointer);
	data_writer(T* data_pointer, std::function<void()> callback);
	data_writer(T* data_pointer, std::function<void(T*)> setter);
	data_writer(std::function<T*()> getter);
	data_writer(std::function<T*()> getter, std::function<void()> callback);
	data_writer(std::function<T*()> getter, std::function<void(T*)> setter);
	data_writer(data_writer const&) = default;
	data_writer(data_writer&&) noexcept = default;
	auto operator=(data_writer const&) -> data_writer& = default;
	auto operator=(data_writer&&) noexcept -> data_writer& = default;
	~data_writer() = default;

	auto read() const -> T*;
	void write(T* new_data) const;

private:
	std::variant<std::monostate, T*, std::function<T*()>> _in;
	std::variant<std::monostate, std::function<void()>, std::function<void(T*)>> _out;
};

template <typename T>
data_writer<T>::data_writer(T* data_pointer) : _in(std::move(data_pointer))
{
}

template <typename T>
data_writer<T>::data_writer(T* data_pointer, std::function<void()> callback)
	: _in(std::move(data_pointer)), _out(std::move(callback))
{
}

template <typename T>
data_writer<T>::data_writer(T* data_pointer, std::function<void(T*)> setter)
	: _in(std::move(data_pointer)), _out(std::move(setter))
{
}

template <typename T>
data_writer<T>::data_writer(std::function<T*()> getter) : _in(std::move(getter))
{
}

template <typename T>
data_writer<T>::data_writer(std::function<T*()> getter, std::function<void()> callback)
	: _in(std::move(getter)), _out(std::move(callback))
{
}

template <typename T>
data_writer<T>::data_writer(std::function<T*()> getter, std::function<void(T*)> setter)
	: _in(std::move(getter)), _out(std::move(setter))
{
}

template <typename T>
auto data_writer<T>::read() const -> T*
{
	if(std::holds_alternative<T*>(_in))
	{
		return std::get<T*>(_in);
	}
	else if(std::holds_alternative<std::function<T*()>>(_in))
	{
		auto getter = std::get<std::function<T*()>>(_in);
		if(getter)
		{
			return getter();
		}
		else
		{
			return nullptr;
		}
	}
	else
	{
		return nullptr;
	}
}

template <typename T>
void data_writer<T>::write(T* new_in) const
{
	if(std::holds_alternative<std::function<void()>>(_out))
	{
		auto callback = std::get<std::function<void()>>(_out);
		if(callback)
		{
			callback();
			return;
		}
	}
	else if(std::holds_alternative<std::function<void(T*)>>(_out))
	{
		auto setter = std::get<std::function<void(T*)>>(_out);
		if(setter)
		{
			setter(new_in);
			return;
		}
	}
}

} // namespace clk::gui