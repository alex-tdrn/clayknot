#pragma once

#include "rsp/base/Graph.hpp"
#include "rsp/gui/widgets/Editor.hpp"
#include "rsp/gui/widgets/Viewer.hpp"
#include "rsp/util/ColorRGBA.hpp"

#include <functional>
#include <imnodes.h>
#include <optional>
#include <unordered_map>
#include <unordered_set>

namespace rsp::gui::impl
{
template <typename Data, typename Widget>
class WidgetCache;
class NodeEditor;
class PortEditor;

template <bool Const>
class SelectionManager;
} // namespace rsp::gui::impl

namespace rsp::gui
{
class GraphEditor final : public EditorOf<Graph>
{
public:
	GraphEditor() = delete;
	GraphEditor(
		Graph* data, std::string const& dataName, std::optional<std::function<void()>> modifiedCallback = std::nullopt);
	GraphEditor(GraphEditor const&) = delete;
	GraphEditor(GraphEditor&&) = delete;
	auto operator=(GraphEditor const&) = delete;
	auto operator=(GraphEditor&&) = delete;
	~GraphEditor() final;

	auto clone() const -> std::unique_ptr<Widget> final;
	void drawContents() const final;

private:
	struct ConnectionChange
	{
		Port& startingPort;
		Port* endingPort = nullptr;
		std::optional<std::pair<Port*, Port*>> droppedConnection = std::nullopt;
	};

	imnodes::EditorContext* context;
	std::unique_ptr<impl::WidgetCache<Node, impl::NodeEditor>> nodeCache;
	std::unique_ptr<impl::WidgetCache<Port, impl::PortEditor>> portCache;
	mutable std::vector<std::pair<InputPort*, OutputPort*>> connections;
	std::unique_ptr<impl::SelectionManager<false>> selectionManager;
	mutable std::optional<ConnectionChange> newConnectionInProgress = std::nullopt;
	mutable std::optional<std::function<bool()>> modificationCallback = std::nullopt; // this is smelly af

	void drawGraph() const;
	void drawMenus() const;
	void updateConnections() const;
	void handleMouseInteractions() const;
	void restoreDroppedConnection() const;
};
} // namespace rsp::gui
