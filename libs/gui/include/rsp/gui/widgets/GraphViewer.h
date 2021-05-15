#pragma once

#include "rsp/base/Graph.hpp"
#include "rsp/gui/widgets/Viewer.hpp"

#include <imnodes.h>

namespace rsp::gui::impl
{
template <typename Data, typename Widget>
class WidgetCache;
class NodeViewer;
class PortViewer;

template <bool Const>
class SelectionManager;
} // namespace rsp::gui::impl

namespace rsp::gui
{
class GraphViewer final : public ViewerOf<Graph>
{
public:
	GraphViewer() = delete;
	GraphViewer(Graph const* data, std::string const& dataName);
	GraphViewer(GraphViewer const&) = delete;
	GraphViewer(GraphViewer&&) = delete;
	auto operator=(GraphViewer const&) -> GraphViewer& = delete;
	auto operator=(GraphViewer&&) -> GraphViewer& = delete;
	~GraphViewer() final;

	auto clone() const -> std::unique_ptr<Widget> final;
	void drawContents() const final;

private:
	imnodes::EditorContext* context;
	std::unique_ptr<impl::WidgetCache<Node const, impl::NodeViewer>> nodeCache;
	std::unique_ptr<impl::WidgetCache<Port const, impl::PortViewer>> portCache;
	mutable std::vector<std::pair<InputPort const*, OutputPort const*>> connections;
	std::unique_ptr<impl::SelectionManager<true>> selectionManager;

	void drawGraph() const;
};

} // namespace rsp::gui