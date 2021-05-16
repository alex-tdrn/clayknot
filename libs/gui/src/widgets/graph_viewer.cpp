#include "rsp/gui/widgets/graph_viewer.h"

#include "node_viewers.hpp"
#include "port_viewers.hpp"
#include "rsp/base/node.hpp"
#include "rsp/base/port.hpp"
#include "selection_manager.hpp"
#include "widget_cache.hpp"

namespace rsp::gui
{
graph_viewer::graph_viewer(rsp::graph const* data, std::string const& data_name)
	: viewer_of<rsp::graph>(data, data_name)
{
	port_cache = std::make_unique<impl::widget_cache<rsp::port const, impl::port_viewer>>(&impl::create_port_viewer);

	node_cache =
		std::make_unique<impl::widget_cache<rsp::node const, impl::node_viewer>>([&](node const* node, int id) {
			return impl::create_node_viewer(node, id, port_cache.get());
		});

	selection_manager = std::make_unique<impl::selection_manager<true>>(node_cache.get(), port_cache.get());

	disable_title();
	context = imnodes::EditorContextCreate();
	imnodes::EditorContextSet(context);

	imnodes::EditorContextSet(nullptr);
}

graph_viewer::~graph_viewer()
{
	imnodes::EditorContextFree(context);
}

auto graph_viewer::clone() const -> std::unique_ptr<widget>
{
	return std::make_unique<graph_viewer>(get_data(), get_data_name());
}

void graph_viewer::draw_contents() const
{
	imnodes::EditorContextSet(context);

	draw_graph();
	selection_manager->update();

	imnodes::EditorContextSet(nullptr);
}

void graph_viewer::draw_graph() const
{
	connections.clear();

	imnodes::BeginNodeEditor();

	for(auto const& node : *get_data())
	{
		node_cache->get_widget(node.get()).draw();
		for(auto* output : node->get_output_ports())
			for(auto* input : output->get_connected_input_ports())
				if(port_cache->has_widget(input))
					connections.emplace_back(std::make_pair(input, output));
	}

	{
		int linkID = 0;
		for(auto& connection : connections)
		{
			auto color = color_rgba(color_rgb::create_random(connection.first->get_data_type_hash()), 1.0f).packed();
			imnodes::PushColorStyle(imnodes::ColorStyle_Link, color);
			imnodes::Link(linkID++, port_cache->get_widget(connection.first).get_id(),
				port_cache->get_widget(connection.second).get_id());
			imnodes::PopColorStyle();
		}
	}

	imnodes::EndNodeEditor();
}

} // namespace rsp::gui