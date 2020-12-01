#pragma once

#include "rsp/base/Sentinel.hpp"
#include "rsp/util/Timestamp.hpp"

#include <algorithm>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_set>

namespace rsp
{
class Port
{
private:
	std::string name = "Unnamed";

protected:
	Port() = default;

public:
	Port(Port&&) = delete;
	Port(Port const&) = delete;
	auto operator=(Port&&) -> Port& = delete;
	auto operator=(Port const&) -> Port& = delete;
	virtual ~Port() = default;

	virtual auto getTimestamp() const noexcept -> Timestamp = 0;
	virtual auto getDataTypeHash() const noexcept -> std::size_t = 0;

	void setName(const std::string& name)
	{
		this->name = name;
	}

	void setName(std::string&& name) noexcept
	{
		this->name = std::move(name);
	}

	auto getName() const noexcept -> std::string const&
	{
		return name;
	}

	virtual auto isConnected() const noexcept -> bool = 0;
	virtual auto canConnectTo(Port const&) const noexcept -> bool = 0;
	virtual auto isConnectedTo(Port const&) const noexcept -> bool = 0;
	virtual void connectTo(Port&) = 0;
	virtual void disconnectFrom(Port&) = 0;
	virtual void disconnect() = 0;
	virtual auto getConnections() const -> std::unordered_set<Port const*> = 0;
	virtual void push(std::weak_ptr<rsp::Sentinel> const& sentinel = {}) noexcept = 0;
	virtual void pull(std::weak_ptr<rsp::Sentinel> const& sentinel = {}) noexcept = 0;
};

template <typename T>
class OutputPort;

template <typename T>
class InputPort final : public Port
{
	using CompatiblePort = OutputPort<T>;

private:
	CompatiblePort mutable* connection = nullptr;
	std::function<void(std::weak_ptr<rsp::Sentinel> const&)> pushCallback;

public:
	InputPort() = default;

	explicit InputPort(std::string name)
	{
		setName(std::move(name));
	}

	InputPort(InputPort const&) = delete;
	InputPort(InputPort&&) = delete;
	auto operator=(InputPort const&) -> InputPort& = delete;
	auto operator=(InputPort&&) -> InputPort& = delete;

	~InputPort() final
	{
		disconnect();
	}

	auto getDataTypeHash() const noexcept -> std::size_t final
	{
		static std::size_t hash = std::type_index(typeid(T)).hash_code();
		return hash;
	}

	auto getTimestamp() const noexcept -> Timestamp final
	{
		if(!connection)
			return {};
		return connection->getTimestamp();
	}

	auto isConnected() const noexcept -> bool final
	{
		return connection;
	}

	auto canConnectTo(Port const& other) const noexcept -> bool final
	{
		return dynamic_cast<CompatiblePort const*>(&other);
	}

	auto isConnectedTo(Port const& other) const noexcept -> bool final
	{
		if(!connection)
			return false;
		return connection == dynamic_cast<CompatiblePort const*>(&other);
	}

	void connectTo(CompatiblePort& other)
	{
		disconnect();
		connection = &other;
		if(!other.isConnectedTo(*this))
			other.connectTo(*this);
	}

	void connectTo(Port& other) final
	{
		connectTo(dynamic_cast<CompatiblePort&>(other));
	}

	void disconnect() final
	{
		if(connection)
		{
			auto oldConnection = connection;
			connection = nullptr;
			oldConnection->disconnectFrom(*this);
		}
	}

	void disconnectFrom(Port& other) final
	{
		if(isConnectedTo(other))
			disconnect();
	}

	auto getConnections() const -> std::unordered_set<Port const*> final
	{
		std::unordered_set<Port const*> connections;
		connections.insert(connection);
		return connections;
	}

	void push(std::weak_ptr<rsp::Sentinel> const& sentinel = {}) noexcept final
	{
		if(pushCallback)
			pushCallback(sentinel);
	}

	void pull(std::weak_ptr<rsp::Sentinel> const& sentinel = {}) noexcept final
	{
		if(connection)
			connection->pull(sentinel);
	}

	void setPushCallback(const std::function<void(std::weak_ptr<rsp::Sentinel> const&)>& callback)
	{
		pushCallback = callback;
	}

	void setPushCallback(std::function<void(std::weak_ptr<rsp::Sentinel> const&)>&& callback) noexcept
	{
		pushCallback = std::move(callback);
	}

	auto get() const -> T const&
	{
		if(!connection)
			throw std::runtime_error("Port is not connected!");
		return connection->get();
	}

	auto operator*() const -> T const&
	{
		return get();
	}

	auto operator->() const -> T const*
	{
		if(!connection)
			throw std::runtime_error("Port is not connected!");
		return connection->operator->();
	}
};

template <typename T>
class OutputPort final : public Port
{
	using CompatiblePort = InputPort<T>;

private:
	T data = {};
	Timestamp timestamp;
	std::unordered_set<CompatiblePort*> connections;
	std::function<void(std::weak_ptr<rsp::Sentinel> const&)> pullCallback;

public:
	OutputPort() = default;

	explicit OutputPort(std::string name)
	{
		setName(std::move(name));
	}

	OutputPort(OutputPort const&) = delete;
	OutputPort(OutputPort&&) = delete;
	auto operator=(OutputPort const&) -> OutputPort& = delete;
	auto operator=(OutputPort&&) -> OutputPort& = delete;

	~OutputPort() final
	{
		disconnect();
	}

	auto getDataTypeHash() const noexcept -> std::size_t final
	{
		static std::size_t hash = std::type_index(typeid(T)).hash_code();
		return hash;
	}

	auto getTimestamp() const noexcept -> Timestamp final
	{
		return timestamp;
	}

	auto isConnected() const noexcept -> bool final
	{
		return !connections.empty();
	}

	auto canConnectTo(Port const& other) const noexcept -> bool final
	{
		return dynamic_cast<CompatiblePort const*>(&other);
	}

	auto isConnectedTo(Port const& other) const noexcept -> bool final
	{
		for(auto const port : connections)
			if(port == dynamic_cast<CompatiblePort const*>(&other))
				return true;
		return false;
	}

	void connectTo(CompatiblePort& other)
	{
		connections.insert(&other);
		if(!other.isConnectedTo(*this))
			other.connectTo(*this);
	}

	void connectTo(Port& other) final
	{
		connectTo(dynamic_cast<CompatiblePort&>(other));
	}

	void disconnectFrom(CompatiblePort& other)
	{
		connections.erase(&other);
		if(other.isConnectedTo(*this))
			other.disconnect();
	}

	void disconnectFrom(Port& other) final
	{
		auto castedPort = dynamic_cast<CompatiblePort*>(&other);
		if(castedPort)
			disconnectFrom(*castedPort);
	}

	void disconnect() final
	{
		while(!connections.empty())
			disconnectFrom(**connections.begin());
	}

	auto getConnections() const -> std::unordered_set<Port const*> final
	{
		std::unordered_set<Port const*> connections;
		for(auto connection : this->connections)
			connections.insert(connection);
		return connections;
	}

	void push(std::weak_ptr<rsp::Sentinel> const& sentinel = {}) noexcept final
	{
		for(auto connection : connections)
			connection->push(sentinel);
	}

	void pull(std::weak_ptr<rsp::Sentinel> const& sentinel = {}) noexcept final
	{
		if(pullCallback)
			pullCallback(sentinel);
	}

	void setPullCallback(const std::function<void(std::weak_ptr<rsp::Sentinel> const&)>& callback)
	{
		this->pullCallback = callback;
	}

	void setPullCallback(std::function<void(std::weak_ptr<rsp::Sentinel> const&)>&& callback) noexcept
	{
		this->pullCallback = std::move(callback);
	}

	auto get() noexcept -> T&
	{
		timestamp.update();
		return data;
	}

	auto get() const noexcept -> T const&
	{
		return data;
	}

	auto operator*() noexcept -> T&
	{
		timestamp.update();
		return get();
	}

	auto operator*() const noexcept -> T const&
	{
		return get();
	}

	auto operator->() noexcept -> T*
	{
		timestamp.update();
		return &data;
	}

	auto operator->() const noexcept -> T const*
	{
		return &data;
	}
};

} // namespace rsp