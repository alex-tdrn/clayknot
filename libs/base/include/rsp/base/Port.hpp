#pragma once

#include "rsp/base/Sentinel.hpp"
#include "rsp/util/Timestamp.hpp"

#include <algorithm>
#include <exception>
#include <functional>
#include <memory>
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
	Port& operator=(Port&&) = delete;
	Port& operator=(Port const&) = delete;
	virtual ~Port() = default;

public:
	virtual Timestamp getTimestamp() const noexcept = 0;
	virtual std::size_t getDataTypeHash() const noexcept = 0;

	void setName(const std::string& name)
	{
		this->name = name;
	}

	void setName(std::string&& name) noexcept
	{
		this->name = std::move(name);
	}

	std::string const& getName() const noexcept
	{
		return name;
	}

	virtual bool isConnected() const noexcept = 0;
	virtual bool canConnectTo(Port const&) const noexcept = 0;
	virtual bool isConnectedTo(Port const&) const noexcept = 0;
	virtual void connectTo(Port&) = 0;
	virtual void disconnectFrom(Port&) = 0;
	virtual void disconnect() = 0;
	virtual std::unordered_set<Port const*> getConnections() const = 0;
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

	InputPort(std::string name)
	{
		setName(std::move(name));
	}

	InputPort(InputPort const&) = delete;
	InputPort(InputPort&&) = delete;
	InputPort& operator=(InputPort const&) = delete;
	InputPort& operator=(InputPort&&) = delete;

	~InputPort()
	{
		disconnect();
	}

public:
	std::size_t getDataTypeHash() const noexcept override
	{
		static std::size_t hash = std::type_index(typeid(T)).hash_code();
		return hash;
	}

	Timestamp getTimestamp() const noexcept override
	{
		if(!connection)
			return {};
		return connection->getTimestamp();
	}

	bool isConnected() const noexcept override
	{
		return connection;
	}

	bool canConnectTo(Port const& other) const noexcept override
	{
		return dynamic_cast<CompatiblePort const*>(&other);
	}

	bool isConnectedTo(Port const& other) const noexcept override
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

	void connectTo(Port& other) override
	{
		connectTo(dynamic_cast<CompatiblePort&>(other));
	}

	void disconnect() override
	{
		if(connection)
		{
			auto oldConnection = connection;
			connection = nullptr;
			oldConnection->disconnectFrom(*this);
		}
	}

	void disconnectFrom(Port& other) override
	{
		if(isConnectedTo(other))
			disconnect();
	}

	std::unordered_set<Port const*> getConnections() const override
	{
		std::unordered_set<Port const*> connections;
		connections.insert(connection);
		return connections;
	}

	void push(std::weak_ptr<rsp::Sentinel> const& sentinel = {}) noexcept override
	{
		if(pushCallback)
			pushCallback(sentinel);
	}

	void pull(std::weak_ptr<rsp::Sentinel> const& sentinel = {}) noexcept override
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

	T const& get() const
	{
		if(!connection)
			throw "Port is not connected!";
		return connection->get();
	}

	T const& operator*() const
	{
		return get();
	}

	T const* operator->() const
	{
		if(!connection)
			throw "Port is not connected!";
		return connection->operator->();
	}
};

template <typename T>
class OutputPort final : public Port
{
	using CompatiblePort = InputPort<T>;

private:
	T data;
	Timestamp timestamp;
	std::unordered_set<CompatiblePort*> connections;
	std::function<void(std::weak_ptr<rsp::Sentinel> const&)> pullCallback;

public:
	OutputPort() = default;

	OutputPort(std::string name)
	{
		setName(std::move(name));
	}

	OutputPort(OutputPort const&) = delete;
	OutputPort(OutputPort&&) = delete;
	OutputPort& operator=(OutputPort const&) = delete;
	OutputPort& operator=(OutputPort&&) = delete;

	~OutputPort()
	{
		disconnect();
	}

public:
	std::size_t getDataTypeHash() const noexcept override
	{
		static std::size_t hash = std::type_index(typeid(T)).hash_code();
		return hash;
	}

	Timestamp getTimestamp() const noexcept override
	{
		return timestamp;
	}

	bool isConnected() const noexcept override
	{
		return !connections.empty();
	}

	bool canConnectTo(Port const& other) const noexcept override
	{
		return dynamic_cast<CompatiblePort const*>(&other);
	}

	bool isConnectedTo(Port const& other) const noexcept override
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

	void connectTo(Port& other) override
	{
		connectTo(dynamic_cast<CompatiblePort&>(other));
	}

	void disconnectFrom(CompatiblePort& other)
	{
		connections.erase(&other);
		if(other.isConnectedTo(*this))
			other.disconnect();
	}

	void disconnectFrom(Port& other) override
	{
		auto castedPort = dynamic_cast<CompatiblePort*>(&other);
		if(castedPort)
			disconnectFrom(*castedPort);
	}

	void disconnect() override
	{
		while(!connections.empty())
			disconnectFrom(**connections.begin());
	}

	std::unordered_set<Port const*> getConnections() const override
	{
		std::unordered_set<Port const*> connections;
		for(auto connection : this->connections)
			connections.insert(connection);
		return connections;
	}

	void push(std::weak_ptr<rsp::Sentinel> const& sentinel = {}) noexcept override
	{
		for(auto connection : connections)
			connection->push(sentinel);
	}

	void pull(std::weak_ptr<rsp::Sentinel> const& sentinel = {}) noexcept override
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

	T& get() noexcept
	{
		timestamp.update();
		return data;
	}

	T const& get() const noexcept
	{
		return data;
	}

	T& operator*() noexcept
	{
		timestamp.update();
		return get();
	}

	T const& operator*() const noexcept
	{
		return get();
	}

	T* operator->() noexcept
	{
		timestamp.update();
		return &data;
	}

	T const* operator->() const noexcept
	{
		return &data;
	}
};

} // namespace rsp