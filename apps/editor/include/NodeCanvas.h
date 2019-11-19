#pragma once

#include "Window.h"
#include "Node.h"

#include <vector>
#include <memory>

namespace rshp::base
{
	class AbstractPipe;
}

class NodeCanvas final : public Window
{
private:
	ax::NodeEditor::EditorContext* context;
	std::vector<std::unique_ptr<rshp::base::AbstractPipe>>* store = nullptr;
	std::vector<Node> nodes;
	
public:
	NodeCanvas();
	NodeCanvas(NodeCanvas const&) = delete;
	NodeCanvas(NodeCanvas&&) = default;
	NodeCanvas& operator=(NodeCanvas const&) = delete;
	NodeCanvas& operator=(NodeCanvas&&) = default;
	~NodeCanvas();

private:
	void drawContents() override;

public:
	void setStore(std::vector<std::unique_ptr<rshp::base::AbstractPipe>>* store);

};