#include "clk/gui/widgets/graph_viewer.hpp"

#include "clk/base/node.hpp"
#include "clk/base/port.hpp"
#include "node_viewers.hpp"
#include "port_viewers.hpp"
#include "selection_manager.hpp"
#include "widget_cache.hpp"

namespace clk::gui
{
graph_viewer::graph_viewer(clk::graph const* data, std::string const& data_name)
	: viewer_of<clk::graph>(data, data_name)
{
	_port_cache = std::make_unique<impl::widget_cache<clk::port const, impl::port_viewer>>(&impl::create_port_viewer);

	_node_cache =
		std::make_unique<impl::widget_cache<clk::node const, impl::node_viewer>>([&](node const* node, int id) {
			return impl::create_node_viewer(node, id, _port_cache.get());
		});

	_selection_manager = std::make_unique<impl::selection_manager<true>>(_node_cache.get(), _port_cache.get());

	disable_title();
	_context = imnodes::EditorContextCreate();
	imnodes::EditorContextSet(_context);

	imnodes::EditorContextSet(nullptr);
}

graph_viewer::~graph_viewer()
{
	imnodes::EditorContextFree(_context);
}

auto graph_viewer::clone() const -> std::unique_ptr<widget>
{
	return std::make_unique<graph_viewer>(get_data(), get_data_name());
}

void graph_viewer::draw_contents() const
{
	imnodes::EditorContextSet(_context);

	draw_graph();
	_selection_manager->update();

	imnodes::EditorContextSet(nullptr);
}

void graph_viewer::draw_graph() const
{
	_connections.clear();

	imnodes::BeginNodeEditor();

	for(auto const& node : *get_data())
	{
		_node_cache->get_widget(node.get()).draw();
		for(auto* output : node->get_output_ports())
			for(auto* input : output->get_connected_input_ports())
				if(_port_cache->has_widget(input))
					_connections.emplace_back(std::make_pair(input, output));
	}

	{
		int linkID = 0;
		for(auto& connection : _connections)
		{
			auto color = color_rgba(color_rgb::create_random(connection.first->get_data_type_hash()), 1.0f).packed();
			imnodes::PushColorStyle(imnodes::ColorStyle_Link, color);
			imnodes::Link(linkID++, _port_cache->get_widget(connection.first).get_id(),
				_port_cache->get_widget(connection.second).get_id());
			imnodes::PopColorStyle();
		}
	}

	imnodes::EndNodeEditor();
}

} // namespace clk::gui