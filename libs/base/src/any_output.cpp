#include "clk/base/any_output.hpp"
#include "clk/base/any_input.hpp"
#include "clk/base/data.hpp"
#include "clk/base/input.hpp"

#include <cassert>
#include <memory>

namespace clk
{

any_output::~any_output()
{
	disconnect();
}

void any_output::set_data(const_data data)
{
	update_timestamp();
	if(data.pointer() == _data.pointer())
	{
		return;
	}

	_data = {};
	for(auto* connected : connected_inputs())
	{
		if(connected->data_type_hash() != data.type_hash() && connected->data_type_hash() != 0)
		{
			disconnect_from(*connected);
		}
	}
	_data = data;

	connection_changed();
}

void any_output::clear_data()
{
	set_data({});
}

auto any_output::data_type_hash() const noexcept -> std::size_t
{
	if(_data.is_empty())
	{
		return 0;
	}
	else
	{
		return _data.type_hash();
	}
}

auto any_output::abstract_data() const noexcept -> const_data
{
	return _data;
}

auto any_output::abstract_data() noexcept -> mutable_data
{
	return mutable_data{};
}

auto any_output::create_compatible_port() const -> std::unique_ptr<port>
{
	return std::make_unique<any_input>();
}

} // namespace clk