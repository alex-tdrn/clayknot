#pragma once
#include "clk/base/graph.hpp"
#include "clk/base/node.hpp"
#include "clk/base/port.hpp"
#include "clk/util/profiler.hpp"
#include "clk/util/timestamp.hpp"

#include <array>
#include <chrono>
#include <cstddef>
#include <glm/glm.hpp>
#include <memory>
#include <range/v3/functional/bind_back.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/view/adaptor.hpp>
#include <range/v3/view/any_view.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/view.hpp>
#include <unordered_map>
#include <utility>
#include <vector>

namespace clk::gui
{
class widget_factory;
class widget_tree;
} // namespace clk::gui

namespace clk::gui::impl
{
class layout_solver
{
public:
	layout_solver();
	layout_solver(layout_solver const&) = delete;
	layout_solver(layout_solver&&) = delete;
	auto operator=(layout_solver const&) -> layout_solver& = delete;
	auto operator=(layout_solver&&) -> layout_solver& = delete;
	~layout_solver() = default;

	void register_settings(widget_tree& settings, widget_factory const& f);

	template <typename T, typename U>
	void update_cache(clk::graph const& graph, T& node_cache, U& port_cache)
	{
		if(_cached_graph_timestamp == graph.timestamp().time_point())
			return;

		_profilers[profiler_update_cache].record_sample_start();
		_cached_graph_timestamp = graph.timestamp().time_point();
		_nodes.clear();
		_ports.clear();
		std::unordered_map<int, std::size_t> port_id_to_index;
		for(auto const& node : graph.nodes())
		{
			auto id = node_cache.widget_for(node.get()).id();

			node_representation n;
			n.id = id;

			_nodes.push_back(n);
			for(auto const& port : node->all_ports() | ranges::views::filter(&clk::port::is_connected))
			{
				port_representation p;
				auto const& widget = port_cache.widget_for(port);
				port_id_to_index[widget.id()] = _ports.size();
				p.position = &widget.position();
				p.parent_node_index = _nodes.size() - 1;
				for(auto const& connected_port : port->connected_ports())
				{
					int connected_id = port_cache.widget_for(connected_port).id();
					// only add the connection if the index comes before the current port
					if(auto it = port_id_to_index.find(connected_id); it != port_id_to_index.end())
						p.connected_port_indices.push_back(it->second);
				}

				_ports.push_back(p);
			}
		}
		_profilers[profiler_update_cache].record_sample_end();
	}

	void step();

private:
	struct node_representation
	{
		int id = -1;
		glm::vec2 position = {0.0f, 0.0f};
		std::array<glm::vec2, 2> collision_sphere_centres;
		float collision_sphere_radius;
		float mass = 1.0f;
		glm::vec2 velocity = {0.0f, 0.0f};
	};
	struct port_representation
	{
		glm::vec2 const* position;
		std::size_t parent_node_index = 0;
		// does not contain all indices, only indices less than the index of the current port
		std::vector<std::size_t> connected_port_indices;
	};
	enum profiler_category
	{
		profiler_update_cache,
		profiler_step,
		profiler_read_from_gui,
		profiler_repulsion,
		profiler_attraction,
		profiler_integration,
		profiler_write_to_gui,
		profiler_N
	};

	mutable std::array<profiler, profiler_N> _profilers;
	using chrono = std::chrono::high_resolution_clock;
	chrono::time_point _last_step_execution = chrono::time_point::min();
	std::chrono::steady_clock::time_point _cached_graph_timestamp = std::chrono::steady_clock::time_point::min();
	std::chrono::milliseconds _minimum_timestep{10};
	std::chrono::milliseconds _maximum_timestep{100};
	glm::vec2 _mouse_position = {0.0f, 0.0f};
	std::vector<node_representation> _nodes;
	std::vector<port_representation> _ports;
	float _mouse_influence_radius = 500.0f;
	float _time_multiplier = 1.0f;
	float _repulsion_intensity_multiplier = 1.0f;
	float _attraction_intensity_multiplier = 1.0f;
	bool _queue_gather = false;

	static auto calculate_force(float ideal_distance, float distance) -> float;
	void update_nodes_from_gui();
	void calculate_repulsion();
	void calculate_attraction();
	void integrate(float seconds_elapsed);
	void write_out_results() const;
};

} // namespace clk::gui::impl