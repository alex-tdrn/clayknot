#pragma once

#include "clk/base/sentinel.hpp"
#include "clk/util/timestamp.hpp"

#include <memory>
#include <range/v3/view.hpp>
#include <string>

namespace clk
{
template <typename T>
using port_range = ranges::any_view<T, ranges::category::bidirectional | ranges::category::sized>;

class port
{
public:
	port(port&&) = delete;
	port(port const&) = delete;
	auto operator=(port&&) -> port& = delete;
	auto operator=(port const&) -> port& = delete;
	virtual ~port() = default;

	void set_name(const std::string& name);
	void set_name(std::string&& name) noexcept;
	auto get_name() const noexcept -> std::string const&;
	void update_timestamp() noexcept;
	virtual auto get_timestamp() const noexcept -> clk::timestamp;
	virtual auto get_data_pointer() const noexcept -> void const* = 0;
	virtual auto get_data_type_hash() const noexcept -> std::size_t = 0;
	virtual auto is_connected() const noexcept -> bool = 0;
	virtual auto can_connect_to(port const& other_port) const noexcept -> bool = 0;
	virtual auto is_connected_to(port const& other_port) const noexcept -> bool = 0;
	virtual void connect_to(port& other_port, bool notify = true) = 0;
	virtual void disconnect_from(port& other_port, bool notify = true) = 0;
	virtual void disconnect(bool notify = true) = 0;
	virtual auto get_connected_ports() const -> port_range<port*> = 0;
	virtual void push(std::weak_ptr<clk::sentinel> const& sentinel = {}) noexcept = 0;
	virtual void pull(std::weak_ptr<clk::sentinel> const& sentinel = {}) noexcept = 0;
	virtual auto create_compatible_port() const -> std::unique_ptr<port> = 0;

protected:
	port() = default;

private:
	std::string _name = "Unnamed";
	clk::timestamp _timestamp;
};

inline void port::set_name(const std::string& name)
{
	_name = name;
}

inline void port::set_name(std::string&& name) noexcept
{
	_name = std::move(name);
}

inline auto port::get_name() const noexcept -> std::string const&
{
	return _name;
}

inline void port::update_timestamp() noexcept
{
	_timestamp.update();
}

inline auto port::get_timestamp() const noexcept -> clk::timestamp
{
	return _timestamp;
}

} // namespace clk