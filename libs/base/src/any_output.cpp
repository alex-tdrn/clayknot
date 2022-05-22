#include "clk/base/any_output.hpp"
#include "clk/base/any_input.hpp"
#include "clk/base/input.hpp"

#include <cassert>
#include <memory>

namespace clk
{

any_output::~any_output()
{
	disconnect();
}

void any_output::set_data(void* data_pointer, std::size_t data_type_hash)
{
	if(data_pointer == nullptr || data_type_hash != 0)
	{
		assert(data_pointer != nullptr);
		assert(data_type_hash == 0);
		disconnect();
		// TODO only disconnect if not any_input
	}

	_data_pointer = data_pointer;
	_data_type_hash = data_type_hash;
}

void any_output::clear_data()
{
	set_data(nullptr, 0);
}

auto any_output::data_type_hash() const noexcept -> std::size_t
{
	return _data_type_hash;
}

auto any_output::data_pointer() const noexcept -> void const*
{
	return _data_pointer;
}

auto any_output::data_pointer() noexcept -> void*
{
	return _data_pointer;
}

auto any_output::create_compatible_port() const -> std::unique_ptr<port>
{
	return std::make_unique<any_input>();
}

} // namespace clk