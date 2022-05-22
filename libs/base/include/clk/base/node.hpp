#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace clk
{
class sentinel;
class port;
class input;
class output;

class node
{
public:
	node() = default;
	node(node const&) = delete;
	node(node&&) noexcept = delete;
	auto operator=(node const&) -> node& = delete;
	auto operator=(node&&) noexcept -> node& = delete;
	virtual ~node() = default;

	virtual auto name() const -> std::string_view = 0;
	auto all_ports() const -> std::vector<clk::port*> const&;
	auto inputs() const -> std::vector<clk::input*> const&;
	auto outputs() const -> std::vector<clk::output*> const&;
	void pull(std::weak_ptr<clk::sentinel> const& sentinel = {});
	void push(std::weak_ptr<clk::sentinel> const& sentinel = {});
	auto has_inputs() const -> bool;
	auto has_outputs() const -> bool;
	auto error() const -> std::string const&;

protected:
	void clear_error();
	void set_error(std::string_view error_message);
	void register_port(clk::input* input);
	void register_port(clk::output* output);
	void unregister_port(clk::input* input);
	void unregister_port(clk::output* output);
	virtual auto update_possible() const -> bool;
	virtual void update();

private:
	std::string _last_error_message;
	std::vector<clk::port*> _ports;
	std::vector<clk::input*> _inputs;
	std::vector<clk::output*> _outputs;
	std::weak_ptr<clk::sentinel> _sentinel;

	void pull_inputs(std::weak_ptr<clk::sentinel> const& sentinel);
	void push_outputs(std::weak_ptr<clk::sentinel> const& sentinel);
	auto sentinel_present() const -> bool;
	auto update_needed() const -> bool;
	void try_update();
};

} // namespace clk