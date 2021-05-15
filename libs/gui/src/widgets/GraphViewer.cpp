#include "rsp/gui/widgets/GraphViewer.h"

#include "NodeViewers.hpp"
#include "PortViewers.hpp"
#include "SelectionManager.hpp"
#include "WidgetCache.hpp"
#include "rsp/base/Node.hpp"
#include "rsp/base/Port.hpp"

namespace rsp::gui
{
GraphViewer::GraphViewer(Graph const* data, std::string const& dataName) : ViewerOf<Graph>(data, dataName)
{
	portCache = std::make_unique<impl::WidgetCache<Port const, impl::PortViewer>>(&impl::createPortViewer);

	nodeCache = std::make_unique<impl::WidgetCache<Node const, impl::NodeViewer>>([&](Node const* node, int id) {
		return impl::createNodeViewer(node, id, portCache.get());
	});

	selectionManager = std::make_unique<impl::SelectionManager<true>>(nodeCache.get(), portCache.get());

	disableTitle();
	context = imnodes::EditorContextCreate();
	imnodes::EditorContextSet(context);

	imnodes::EditorContextSet(nullptr);
}

GraphViewer::~GraphViewer()
{
	imnodes::EditorContextFree(context);
}

auto GraphViewer::clone() const -> std::unique_ptr<Widget>
{
	return std::make_unique<GraphViewer>(getData(), getDataName());
}

void GraphViewer::drawContents() const
{
	imnodes::EditorContextSet(context);

	drawGraph();
	selectionManager->update();

	imnodes::EditorContextSet(nullptr);
}

void GraphViewer::drawGraph() const
{
	connections.clear();

	imnodes::BeginNodeEditor();

	for(auto const& node : *getData())
	{
		nodeCache->getWidget(node.get()).draw();
		for(auto* output : node->getOutputPorts())
			for(auto* input : output->getConnectedInputPorts())
				if(portCache->hasWidget(input))
					connections.emplace_back(std::make_pair(input, output));
	}

	{
		int linkID = 0;
		for(auto& connection : connections)
		{
			auto color = ColorRGBA(ColorRGB::createRandom(connection.first->getDataTypeHash()), 1.0f).packed();
			imnodes::PushColorStyle(imnodes::ColorStyle_Link, color);
			imnodes::Link(linkID++, portCache->getWidget(connection.first).getID(),
				portCache->getWidget(connection.second).getID());
			imnodes::PopColorStyle();
		}
	}

	imnodes::EndNodeEditor();
}

} // namespace rsp::gui