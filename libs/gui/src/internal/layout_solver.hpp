#pragma once
#include "clk/base/graph.hpp"

#include "clk/gui/widgets/action_widget.hpp"
#include "clk/gui/widgets/widget_factory.hpp"
#include "clk/gui/widgets/widget_tree.hpp"

#include "clk/util/profiler.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <imgui.h>
#include <imnodes.h>
#include <vector>

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
	void update_cache(clk::graph const& graph, T& node_cache, U& port_cache);
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
	glm::vec2 _mouse_position;
	std::vector<node_representation> _nodes;
	std::vector<port_representation> _ports;
	float _mouse_influence_radius = 500.0f;
	float _time_multiplier = 1.0f;
	float _repulsion_intensity_multiplier = 1.0f;
	float _attraction_intensity_multiplier = 1.0f;
	bool _queue_gather = false;

	constexpr static auto calculate_force(float ideal_distance, float distance) -> float;
	void update_nodes_from_gui();
	void calculate_repulsion();
	void calculate_attraction();
	void integrate(float seconds_elapsed);
	void write_out_results() const;
};

inline layout_solver::layout_solver()
{
	for(auto& profiler : _profilers)
		profiler.set_active(false);
}

inline void layout_solver::register_settings(widget_tree& settings, widget_factory const& f)
{
	settings.add(std::make_unique<action_widget>(
		[&]() {
			_queue_gather = true;
		},
		"Gather nodes"));

	{
		auto& parameters = settings.get_subtree("Parameter");
		parameters.add(f.create(_mouse_influence_radius, "Mouse influence radius"));
		parameters.add(f.create(_time_multiplier, "Time multiplier"));
		parameters.add(f.create(_repulsion_intensity_multiplier, "Repulsion intensity"));
		parameters.add(f.create(_attraction_intensity_multiplier, "Attraction intensity"));
	}
	{
		auto& profilers = settings.get_subtree("Profilers");
		profilers.add(f.create(_profilers[profiler_update_cache], "Update cache"));
		profilers.add(f.create(_profilers[profiler_step], "Step"));
		profilers.add(f.create(_profilers[profiler_read_from_gui], "Read positions from GUI"));
		profilers.add(f.create(_profilers[profiler_repulsion], "Repulsion"));
		profilers.add(f.create(_profilers[profiler_attraction], "Attraction"));
		profilers.add(f.create(_profilers[profiler_integration], "Integration"));
		profilers.add(f.create(_profilers[profiler_write_to_gui], "Write positions to GUI"));
	}
}

template <typename T, typename U>
void layout_solver::update_cache(clk::graph const& graph, T& node_cache, U& port_cache)
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

inline void layout_solver::step()
{
	_profilers[profiler_step].record_sample_start();

	auto current_time = chrono::now();
	if(_last_step_execution == chrono::time_point::min())
	{
		_last_step_execution = current_time;
		_profilers[profiler_step].record_sample_end();
		return;
	}

	auto current_timestep = current_time - _last_step_execution;
	if(current_timestep < _minimum_timestep)
	{
		_profilers[profiler_step].record_sample_end();
		return;
	}

	if(current_timestep > _maximum_timestep)
	{
		current_timestep = _maximum_timestep;
	}

	_last_step_execution = current_time;

	update_nodes_from_gui();
	calculate_repulsion();
	calculate_attraction();
	integrate(std::chrono::duration_cast<std::chrono::duration<float>>(current_timestep).count());
	write_out_results();
	_profilers[profiler_step].record_sample_end();
}

inline constexpr auto layout_solver::calculate_force(float ideal_distance, float distance) -> float
{
	float force = ideal_distance - distance;
	return (force < 0 ? -1.0f : 1.0f) * force * force;
}

inline void layout_solver::update_nodes_from_gui()
{
	_profilers[profiler_read_from_gui].record_sample_start();
	_mouse_position = ImGui::GetMousePos();
	_mouse_position -= glm::vec2(ImGui::GetCursorStartPos());
	_mouse_position -= glm::vec2(ImNodes::EditorContextGetPanning());

	for(auto& node : _nodes)
	{
		glm::vec2 dim = ImNodes::GetNodeDimensions(node.id);
		glm::vec2 pos = ImNodes::GetNodeGridSpacePos(node.id);
		pos += dim / 2.0f;
		if(_queue_gather)
		{
			pos = {0, 0};
		}
		node.position = pos;

		const int long_axis = dim.x > dim.y ? 0 : 1;
		const int short_axis = dim.x < dim.y ? 0 : 1;
		node.collision_sphere_centres[0] = pos;
		node.collision_sphere_centres[1] = pos;
		node.collision_sphere_radius = std::max(dim[long_axis] / 4.0f, dim[short_axis] / 2.0f);
		node.collision_sphere_centres[0][long_axis] -= dim[long_axis] / 4.0f;
		node.collision_sphere_centres[1][long_axis] += dim[long_axis] / 4.0f;

		node.mass = dim.x * dim.y / 100.0f;
		node.velocity = glm::vec2{0.0f};
	}
	_queue_gather = false;
	_profilers[profiler_read_from_gui].record_sample_end();
}

inline void layout_solver::calculate_repulsion()
{
	_profilers[profiler_repulsion].record_sample_start();
	for(auto node1 = _nodes.begin(); node1 != _nodes.end(); ++node1)
	{
		for(auto node2 = node1 + 1; node2 != _nodes.end(); ++node2)
		{
			for(auto& collision_sphere_node1 : node1->collision_sphere_centres)
			{
				for(auto& collision_sphere_node2 : node2->collision_sphere_centres)
				{
					auto dir = collision_sphere_node2 - collision_sphere_node1;
					float distance = glm::length(dir);
					const float ideal_distance = (node1->collision_sphere_radius + node2->collision_sphere_radius) * 2;
					if(distance < ideal_distance)
					{
						dir /= distance;
						float force = calculate_force(ideal_distance, distance) * _repulsion_intensity_multiplier;

						node1->velocity -= dir * force / node1->mass;
						node2->velocity += dir * force / node2->mass;
					}
				}
			}
		}
	}
	_profilers[profiler_repulsion].record_sample_end();
}

inline void layout_solver::calculate_attraction()
{
	_profilers[profiler_attraction].record_sample_start();
	for(auto const& port1 : _ports)
	{
		auto& node1 = _nodes[port1.parent_node_index];
		for(std::size_t port2_index : port1.connected_port_indices)
		{
			auto const& port2 = _ports[port2_index];
			auto& node2 = _nodes[port2.parent_node_index];

			auto port1_to_port2 = *port2.position - *port1.position;
			float distance = glm::length(port1_to_port2);
			const float ideal_distance = 0;
			port1_to_port2 /= distance;
			float force = calculate_force(ideal_distance, distance) * _attraction_intensity_multiplier;

			node1.velocity -= port1_to_port2 * force / node1.mass;
			node2.velocity += port1_to_port2 * force / node2.mass;
		}
	}
	_profilers[profiler_attraction].record_sample_end();
}

inline void layout_solver::integrate(float seconds_elapsed)
{
	_profilers[profiler_integration].record_sample_start();
	seconds_elapsed *= _time_multiplier;
	for(auto& node : _nodes)
	{
		auto position_difference = node.velocity * seconds_elapsed;

		if(auto distance_to_mouse = std::max(std::min(glm::length(node.collision_sphere_centres[0] - _mouse_position),
												 glm::length(node.collision_sphere_centres[1] - _mouse_position)) -
												 node.collision_sphere_radius,
			   0.0f);
			distance_to_mouse <= _mouse_influence_radius)
		{
			float time_influence = distance_to_mouse / _mouse_influence_radius;
			time_influence *= time_influence;
			position_difference *= time_influence;
		}

		if(glm::length(position_difference) > 0.1f)
			node.position += position_difference;
	}
	_profilers[profiler_integration].record_sample_end();
}

inline void layout_solver::write_out_results() const
{
	_profilers[profiler_write_to_gui].record_sample_start();
	for(auto const& node : _nodes)
	{
		glm::vec2 dim = ImNodes::GetNodeDimensions(node.id);
		ImNodes::SetNodeGridSpacePos(node.id, node.position - dim / 2.0f);
	}
	_profilers[profiler_write_to_gui].record_sample_end();
}

} // namespace clk::gui::impl