#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "rsp/base/node/EventNode.hpp"

namespace rsp
{
class AbstractNode : public virtual EventNode
{
public:
	struct InputEvents
	{
		enum
		{
			SourceEvents = 8,
			SinkEvents = 16,
			NodeEvents = 24,
			UserEvents = 32
		};
	};

	struct OutputEvents
	{
		enum
		{
			Updated,
			SourceEvents = 8,
			SinkEvents = 16,
			NodeEvents = 24,
			UserEvents = 32
		};
	};

public:
	AbstractNode() = default;
	AbstractNode(AbstractNode const&) = delete;
	AbstractNode(AbstractNode&&) = default;
	AbstractNode& operator=(AbstractNode const&) = delete;
	AbstractNode& operator=(AbstractNode&&) = default;
	virtual ~AbstractNode() = default;

private:
	static std::unordered_map<std::string, std::unique_ptr<AbstractNode> (*)()>& nodeMap()
	{
		static std::unordered_map<std::string, std::unique_ptr<AbstractNode> (*)()> allNodes;
		return allNodes;
	}

protected:
	virtual void update() = 0;

	template <typename ConcreteNode>
	static std::string registerNode(std::string name)
	{
		nodeMap()[name] = []() {
			std::unique_ptr<AbstractNode> ptr = std::make_unique<ConcreteNode>();
			return ptr;
		};
		return name;
	}

public:
	virtual void run() = 0;

	void registerInputEvents() override
	{
	}

	void registerOutputEvents() override
	{
		registerOutputEvent(OutputEvents::Updated, "Updated");
	}

	static std::unordered_map<std::string, std::unique_ptr<AbstractNode> (*)()> const& getNodeMap()
	{
		return nodeMap();
	}

	static std::unique_ptr<AbstractNode> createNode(std::string const name)
	{
		assert(nodeMap().find(name) != nodeMap().end());

		return nodeMap()[name]();
	}

	virtual std::string const& getName() const = 0;
};

} // namespace rsp