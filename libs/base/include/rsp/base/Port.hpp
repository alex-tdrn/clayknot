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
public:
	Port(Port&&) = delete;
	Port(Port const&) = delete;
	auto operator=(Port&&) -> Port& = delete;
	auto operator=(Port const&) -> Port& = delete;
	virtual ~Port() = default;

	void setName(const std::string& name);
	void setName(std::string&& name) noexcept;
	auto getName() const noexcept -> std::string const&;
	virtual auto getTimestamp() const noexcept -> Timestamp = 0;
	virtual auto getDataTypeHash() const noexcept -> std::size_t = 0;
	virtual auto isConnected() const noexcept -> bool = 0;
	virtual auto canConnectTo(Port const&) const noexcept -> bool = 0;
	virtual auto isConnectedTo(Port const&) const noexcept -> bool = 0;
	virtual void connectTo(Port&) = 0;
	virtual void disconnectFrom(Port&) = 0;
	virtual void disconnect() = 0;
	virtual auto getConnections() const -> std::unordered_set<Port const*> = 0;
	virtual void push(std::weak_ptr<rsp::Sentinel> const& sentinel = {}) noexcept = 0;
	virtual void pull(std::weak_ptr<rsp::Sentinel> const& sentinel = {}) noexcept = 0;

protected:
	Port() = default;

private:
	std::string name = "Unnamed";
};

template <typename T>
class OutputPort;

template <typename T>
class InputPort final : public Port
{
public:
	using CompatiblePort = OutputPort<T>;

	InputPort() = default;
	explicit InputPort(std::string name);
	InputPort(InputPort const&) = delete;
	InputPort(InputPort&&) = delete;
	auto operator=(InputPort const&) -> InputPort& = delete;
	auto operator=(InputPort&&) -> InputPort& = delete;
	~InputPort() final;

	auto get() const -> T const&;
	auto operator*() const -> T const&;
	auto operator->() const -> T const*;
	auto getDataTypeHash() const noexcept -> std::size_t final;
	auto getTimestamp() const noexcept -> Timestamp final;
	auto isConnected() const noexcept -> bool final;
	auto canConnectTo(Port const& other) const noexcept -> bool final;
	auto isConnectedTo(Port const& other) const noexcept -> bool final;
	void connectTo(CompatiblePort& other);
	void connectTo(Port& other) final;
	void disconnect() final;
	void disconnectFrom(Port& other) final;
	auto getConnections() const -> std::unordered_set<Port const*> final;
	void setPushCallback(const std::function<void(std::weak_ptr<rsp::Sentinel> const&)>& callback);
	void setPushCallback(std::function<void(std::weak_ptr<rsp::Sentinel> const&)>&& callback) noexcept;
	void push(std::weak_ptr<rsp::Sentinel> const& sentinel = {}) noexcept final;
	void pull(std::weak_ptr<rsp::Sentinel> const& sentinel = {}) noexcept final;

private:
	CompatiblePort mutable* connection = nullptr;
	std::function<void(std::weak_ptr<rsp::Sentinel> const&)> pushCallback;
};

template <typename T>
class OutputPort final : public Port
{
public:
	using CompatiblePort = InputPort<T>;

	OutputPort() = default;
	explicit OutputPort(std::string name);
	OutputPort(OutputPort const&) = delete;
	OutputPort(OutputPort&&) = delete;
	auto operator=(OutputPort const&) -> OutputPort& = delete;
	auto operator=(OutputPort&&) -> OutputPort& = delete;
	~OutputPort() final;

	auto get() noexcept -> T&;
	auto get() const noexcept -> T const&;
	auto operator*() noexcept -> T&;
	auto operator*() const noexcept -> T const&;
	auto operator->() noexcept -> T*;
	auto operator->() const noexcept -> T const*;
	auto getDataTypeHash() const noexcept -> std::size_t final;
	auto getTimestamp() const noexcept -> Timestamp final;
	auto isConnected() const noexcept -> bool final;
	auto canConnectTo(Port const& other) const noexcept -> bool final;
	auto isConnectedTo(Port const& other) const noexcept -> bool final;
	void connectTo(CompatiblePort& other);
	void connectTo(Port& other) final;
	void disconnectFrom(CompatiblePort& other);
	void disconnectFrom(Port& other) final;
	void disconnect() final;
	auto getConnections() const -> std::unordered_set<Port const*> final;
	void setPullCallback(const std::function<void(std::weak_ptr<rsp::Sentinel> const&)>& callback);
	void setPullCallback(std::function<void(std::weak_ptr<rsp::Sentinel> const&)>&& callback) noexcept;
	void push(std::weak_ptr<rsp::Sentinel> const& sentinel = {}) noexcept final;
	void pull(std::weak_ptr<rsp::Sentinel> const& sentinel = {}) noexcept final;

private:
	T data = {};
	Timestamp timestamp;
	std::unordered_set<CompatiblePort*> connections;
	std::function<void(std::weak_ptr<rsp::Sentinel> const&)> pullCallback;
};

inline void Port::setName(const std::string& name)
{
	this->name = name;
}

inline void Port::setName(std::string&& name) noexcept
{
	this->name = std::move(name);
}

inline auto Port::getName() const noexcept -> std::string const&
{
	return name;
}

template <typename T>
InputPort<T>::InputPort(std::string name)
{
	setName(std::move(name));
}

template <typename T>
InputPort<T>::~InputPort()
{
	disconnect();
}

template <typename T>
auto InputPort<T>::getDataTypeHash() const noexcept -> std::size_t
{
	static std::size_t hash = std::type_index(typeid(T)).hash_code();
	return hash;
}

template <typename T>
auto InputPort<T>::getTimestamp() const noexcept -> Timestamp
{
	if(!connection)
		return {};
	return connection->getTimestamp();
}

template <typename T>
auto InputPort<T>::isConnected() const noexcept -> bool
{
	return connection;
}

template <typename T>
auto InputPort<T>::canConnectTo(Port const& other) const noexcept -> bool
{
	return dynamic_cast<CompatiblePort const*>(&other);
}

template <typename T>
auto InputPort<T>::isConnectedTo(Port const& other) const noexcept -> bool
{
	if(!connection)
		return false;
	return connection == dynamic_cast<CompatiblePort const*>(&other);
}

template <typename T>
void InputPort<T>::connectTo(CompatiblePort& other)
{
	disconnect();
	connection = &other;
	if(!other.isConnectedTo(*this))
		other.connectTo(*this);
}

template <typename T>
void InputPort<T>::connectTo(Port& other)
{
	connectTo(dynamic_cast<CompatiblePort&>(other));
}

template <typename T>
void InputPort<T>::disconnect()
{
	if(connection)
	{
		auto oldConnection = connection;
		connection = nullptr;
		oldConnection->disconnectFrom(*this);
	}
}

template <typename T>
void InputPort<T>::disconnectFrom(Port& other)
{
	if(isConnectedTo(other))
		disconnect();
}

template <typename T>
auto InputPort<T>::getConnections() const -> std::unordered_set<Port const*>
{
	std::unordered_set<Port const*> connections;
	connections.insert(connection);
	return connections;
}

template <typename T>
void InputPort<T>::push(std::weak_ptr<rsp::Sentinel> const& sentinel) noexcept
{
	if(pushCallback)
		pushCallback(sentinel);
}

template <typename T>
void InputPort<T>::pull(std::weak_ptr<rsp::Sentinel> const& sentinel) noexcept
{
	if(connection)
		connection->pull(sentinel);
}

template <typename T>
void InputPort<T>::setPushCallback(const std::function<void(std::weak_ptr<rsp::Sentinel> const&)>& callback)
{
	pushCallback = callback;
}

template <typename T>
void InputPort<T>::setPushCallback(std::function<void(std::weak_ptr<rsp::Sentinel> const&)>&& callback) noexcept
{
	pushCallback = std::move(callback);
}

template <typename T>
auto InputPort<T>::get() const -> T const&
{
	if(!connection)
		throw std::runtime_error("Port is not connected!");
	return connection->get();
}

template <typename T>
auto InputPort<T>::operator*() const -> T const&
{
	return get();
}

template <typename T>
auto InputPort<T>::operator->() const -> T const*
{
	if(!connection)
		throw std::runtime_error("Port is not connected!");
	return connection->operator->();
}

template <typename T>
OutputPort<T>::OutputPort(std::string name)
{
	setName(std::move(name));
}

template <typename T>
OutputPort<T>::~OutputPort()
{
	disconnect();
}

template <typename T>
auto OutputPort<T>::getDataTypeHash() const noexcept -> std::size_t
{
	static std::size_t hash = std::type_index(typeid(T)).hash_code();
	return hash;
}

template <typename T>
auto OutputPort<T>::getTimestamp() const noexcept -> Timestamp
{
	return timestamp;
}

template <typename T>
auto OutputPort<T>::isConnected() const noexcept -> bool
{
	return !connections.empty();
}

template <typename T>
auto OutputPort<T>::canConnectTo(Port const& other) const noexcept -> bool
{
	return dynamic_cast<CompatiblePort const*>(&other);
}

template <typename T>
auto OutputPort<T>::isConnectedTo(Port const& other) const noexcept -> bool
{
	return std::any_of(connections.begin(), connections.end(),
		[&other](auto const* port) { return port == dynamic_cast<CompatiblePort const*>(&other); });
}

template <typename T>
void OutputPort<T>::connectTo(CompatiblePort& other)
{
	connections.insert(&other);
	if(!other.isConnectedTo(*this))
		other.connectTo(*this);
}

template <typename T>
void OutputPort<T>::connectTo(Port& other)
{
	connectTo(dynamic_cast<CompatiblePort&>(other));
}

template <typename T>
void OutputPort<T>::disconnectFrom(CompatiblePort& other)
{
	connections.erase(&other);
	if(other.isConnectedTo(*this))
		other.disconnect();
}

template <typename T>
void OutputPort<T>::disconnectFrom(Port& other)
{
	auto castedPort = dynamic_cast<CompatiblePort*>(&other);
	if(castedPort)
		disconnectFrom(*castedPort);
}

template <typename T>
void OutputPort<T>::disconnect()
{
	while(!connections.empty())
		disconnectFrom(**connections.begin());
}

template <typename T>
auto OutputPort<T>::getConnections() const -> std::unordered_set<Port const*>
{
	std::unordered_set<Port const*> connections;
	for(auto connection : this->connections)
		connections.insert(connection);
	return connections;
}

template <typename T>
void OutputPort<T>::push(std::weak_ptr<rsp::Sentinel> const& sentinel) noexcept
{
	for(auto connection : connections)
		connection->push(sentinel);
}

template <typename T>
void OutputPort<T>::pull(std::weak_ptr<rsp::Sentinel> const& sentinel) noexcept
{
	if(pullCallback)
		pullCallback(sentinel);
}

template <typename T>
void OutputPort<T>::setPullCallback(const std::function<void(std::weak_ptr<rsp::Sentinel> const&)>& callback)
{
	this->pullCallback = callback;
}

template <typename T>
void OutputPort<T>::setPullCallback(std::function<void(std::weak_ptr<rsp::Sentinel> const&)>&& callback) noexcept
{
	this->pullCallback = std::move(callback);
}

template <typename T>
auto OutputPort<T>::get() noexcept -> T&
{
	timestamp.update();
	return data;
}

template <typename T>
auto OutputPort<T>::get() const noexcept -> T const&
{
	return data;
}

template <typename T>
auto OutputPort<T>::operator*() noexcept -> T&
{
	timestamp.update();
	return get();
}

template <typename T>
auto OutputPort<T>::operator*() const noexcept -> T const&
{
	return get();
}

template <typename T>
auto OutputPort<T>::operator->() noexcept -> T*
{
	timestamp.update();
	return &data;
}

template <typename T>
auto OutputPort<T>::operator->() const noexcept -> T const*
{
	return &data;
}

} // namespace rsp