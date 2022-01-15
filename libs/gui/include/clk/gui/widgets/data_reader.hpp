#pragma once

#include <functional>
#include <stdexcept>
#include <variant>

namespace clk::gui
{
template <typename T>
class data_reader
{
public:
	data_reader() = default;
	explicit data_reader(T const* data_pointer);
	explicit data_reader(std::function<T const*()> getter);
	data_reader(data_reader const&) = default;
	data_reader(data_reader&&) noexcept = default;
	auto operator=(data_reader const&) -> data_reader& = default;
	auto operator=(data_reader&&) noexcept -> data_reader& = default;
	~data_reader() = default;

	auto read() const -> T const*;

private:
	std::variant<T const*, std::function<T const*()>> _in = nullptr;
};

template <typename T>
data_reader<T>::data_reader(T const* data_pointer) : _in(data_pointer)
{
}

template <typename T>
data_reader<T>::data_reader(std::function<T const*()> getter) : _in(std::move(getter))
{
}

template <typename T>
auto data_reader<T>::read() const -> T const*
{
	if(std::holds_alternative<T const*>(_in))
	{
		return std::get<T const*>(_in);
	}
	else if(std::holds_alternative<std::function<T const*()>>(_in))
	{
		auto getter = std::get<std::function<T const*()>>(_in);
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

} // namespace clk::gui