#pragma once

#include "clk/base/data.hpp"
#include "clk/base/graph.hpp"
#include "clk/util/timestamp.hpp"

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace clk
{
class sentinel;

class port
{
public:
	port() = default;
	explicit port(std::string_view name);
	port(port&&) = delete;
	port(port const&) = delete;
	auto operator=(port&&) -> port& = delete;
	auto operator=(port const&) -> port& = delete;
	virtual ~port() = default;

	void set_name(std::string_view name);
	auto name() const noexcept -> std::string_view;
	void update_timestamp() noexcept;
	virtual auto timestamp() const noexcept -> clk::timestamp;

	void mark_as_faulty() const noexcept;
	void mark_as_working() const noexcept;
	virtual auto is_faulty() const noexcept -> bool;

	virtual auto abstract_data() const noexcept -> const_data = 0;
	virtual auto data_type_hash() const noexcept -> std::size_t = 0;

	virtual auto can_connect_to(port const& other_port) const noexcept -> bool = 0;
	virtual void connect_to(port& other_port, bool notify = true) = 0;
	virtual void disconnect_from(port& other_port, bool notify = true) = 0;
	virtual void disconnect(bool notify = true) = 0;
	virtual auto connected_ports() const -> std::vector<port*> const& = 0;
	auto is_connected() const noexcept -> bool;
	auto is_connected_to(port const& other_port) const noexcept -> bool;

	virtual void push(std::weak_ptr<clk::sentinel> const& sentinel = {}) noexcept = 0;
	virtual void pull(std::weak_ptr<clk::sentinel> const& sentinel = {}) noexcept = 0;

	virtual auto create_compatible_port() const -> std::unique_ptr<port> = 0;

protected:
	void connection_changed();

private:
	friend class graph;

	std::string _name = "Unnamed";
	clk::timestamp _timestamp;
	mutable bool _faulty = false;
	std::function<void()> _connection_changed_callback;

	void set_connection_changed_callback(std::function<void()> const& callback);
};

} // namespace clk