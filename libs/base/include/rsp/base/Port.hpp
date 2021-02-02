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
	virtual auto getConnectedPorts() const -> std::unordered_set<Port*> = 0;
	virtual void push(std::weak_ptr<Sentinel> const& sentinel = {}) noexcept = 0;
	virtual void pull(std::weak_ptr<Sentinel> const& sentinel = {}) noexcept = 0;

protected:
	Port() = default;

private:
	std::string name = "Unnamed";
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

class OutputPort;

class InputPort : public Port
{
public:
	InputPort(InputPort const&) = delete;
	InputPort(InputPort&&) = delete;
	auto operator=(InputPort const&) -> InputPort& = delete;
	auto operator=(InputPort&&) -> InputPort& = delete;
	~InputPort() override = default;

	using Port::connectTo;
	void connectTo(InputPort&) = delete;
	virtual auto getConnectedOutputPort() const -> OutputPort* = 0;
	void push(std::weak_ptr<Sentinel> const& sentinel = {}) noexcept final;
	void setPushCallback(const std::function<void(std::weak_ptr<Sentinel> const&)>& callback);
	void setPushCallback(std::function<void(std::weak_ptr<Sentinel> const&)>&& callback) noexcept;

protected:
	InputPort() = default;

private:
	std::function<void(std::weak_ptr<Sentinel> const&)> pushCallback;
};

inline void InputPort::setPushCallback(const std::function<void(std::weak_ptr<Sentinel> const&)>& callback)
{
	pushCallback = callback;
}

inline void InputPort::setPushCallback(std::function<void(std::weak_ptr<Sentinel> const&)>&& callback) noexcept
{
	pushCallback = std::move(callback);
}

inline void InputPort::push(std::weak_ptr<Sentinel> const& sentinel) noexcept
{
	if(pushCallback)
		pushCallback(sentinel);
}

class OutputPort : public Port
{
public:
	OutputPort(OutputPort const&) = delete;
	OutputPort(OutputPort&&) = delete;
	auto operator=(OutputPort const&) -> OutputPort& = delete;
	auto operator=(OutputPort&&) -> OutputPort& = delete;
	~OutputPort() override = default;

	using Port::connectTo;
	void connectTo(OutputPort&) = delete;
	virtual auto getConnectedInputPorts() const -> std::unordered_set<InputPort*> = 0;
	auto getTimestamp() const noexcept -> Timestamp final;
	void updateTimestamp() noexcept;
	void setPullCallback(const std::function<void(std::weak_ptr<Sentinel> const&)>& callback);
	void setPullCallback(std::function<void(std::weak_ptr<Sentinel> const&)>&& callback) noexcept;
	void pull(std::weak_ptr<Sentinel> const& sentinel = {}) noexcept final;

protected:
	OutputPort() = default;

private:
	Timestamp timestamp;
	std::function<void(std::weak_ptr<Sentinel> const&)> pullCallback;
};

inline auto OutputPort::getTimestamp() const noexcept -> Timestamp
{
	return timestamp;
}

inline void OutputPort::updateTimestamp() noexcept
{
	timestamp.update();
}

inline void OutputPort::pull(std::weak_ptr<Sentinel> const& sentinel) noexcept
{
	if(pullCallback)
		pullCallback(sentinel);
}

inline void OutputPort::setPullCallback(const std::function<void(std::weak_ptr<Sentinel> const&)>& callback)
{
	pullCallback = callback;
}

inline void OutputPort::setPullCallback(std::function<void(std::weak_ptr<Sentinel> const&)>&& callback) noexcept
{
	pullCallback = std::move(callback);
}

template <typename T>
class OutputPortOf;
template <typename T>
class InputPortOf final : public InputPort
{
public:
	using CompatiblePort = OutputPortOf<T>;

	InputPortOf() = default;
	explicit InputPortOf(std::string name);
	InputPortOf(InputPortOf const&) = delete;
	InputPortOf(InputPortOf&&) = delete;
	auto operator=(InputPortOf const&) -> InputPortOf& = delete;
	auto operator=(InputPortOf&&) -> InputPortOf& = delete;
	~InputPortOf() final;

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
	auto getConnectedPorts() const -> std::unordered_set<Port*> final;
	auto getConnectedOutputPort() const -> OutputPort* final;
	void pull(std::weak_ptr<Sentinel> const& sentinel = {}) noexcept final;

private:
	CompatiblePort mutable* connection = nullptr;
};

template <typename T>
InputPortOf<T>::InputPortOf(std::string name)
{
	setName(std::move(name));
}

template <typename T>
InputPortOf<T>::~InputPortOf()
{
	disconnect();
}

template <typename T>
auto InputPortOf<T>::getDataTypeHash() const noexcept -> std::size_t
{
	static std::size_t hash = std::type_index(typeid(T)).hash_code();
	return hash;
}

template <typename T>
auto InputPortOf<T>::getTimestamp() const noexcept -> Timestamp
{
	if(!connection)
		return {};
	return connection->getTimestamp();
}

template <typename T>
auto InputPortOf<T>::isConnected() const noexcept -> bool
{
	return connection;
}

template <typename T>
auto InputPortOf<T>::canConnectTo(Port const& other) const noexcept -> bool
{
	return dynamic_cast<CompatiblePort const*>(&other);
}

template <typename T>
auto InputPortOf<T>::isConnectedTo(Port const& other) const noexcept -> bool
{
	if(!connection)
		return false;
	return connection == dynamic_cast<CompatiblePort const*>(&other);
}

template <typename T>
void InputPortOf<T>::connectTo(CompatiblePort& other)
{
	disconnect();
	connection = &other;
	if(!other.isConnectedTo(*this))
		other.connectTo(*this);
}

template <typename T>
void InputPortOf<T>::connectTo(Port& other)
{
	connectTo(dynamic_cast<CompatiblePort&>(other));
}

template <typename T>
void InputPortOf<T>::disconnect()
{
	if(connection)
	{
		auto oldConnection = connection;
		connection = nullptr;
		oldConnection->disconnectFrom(*this);
	}
}

template <typename T>
void InputPortOf<T>::disconnectFrom(Port& other)
{
	if(isConnectedTo(other))
		disconnect();
}

template <typename T>
auto InputPortOf<T>::getConnectedPorts() const -> std::unordered_set<Port*>
{
	std::unordered_set<Port*> connections;
	connections.insert(connection);
	return connections;
}

template <typename T>
auto InputPortOf<T>::getConnectedOutputPort() const -> OutputPort*
{
	return connection;
}

template <typename T>
void InputPortOf<T>::pull(std::weak_ptr<Sentinel> const& sentinel) noexcept
{
	if(connection)
		connection->pull(sentinel);
}

template <typename T>
auto InputPortOf<T>::get() const -> T const&
{
	if(!connection)
		throw std::runtime_error("Port is not connected!");
	return connection->get();
}

template <typename T>
auto InputPortOf<T>::operator*() const -> T const&
{
	return get();
}

template <typename T>
auto InputPortOf<T>::operator->() const -> T const*
{
	if(!connection)
		throw std::runtime_error("Port is not connected!");
	return connection->operator->();
}

template <typename T>
class OutputPortOf final : public OutputPort
{
public:
	using CompatiblePort = InputPortOf<T>;

	OutputPortOf() = default;
	explicit OutputPortOf(std::string name);
	OutputPortOf(OutputPortOf const&) = delete;
	OutputPortOf(OutputPortOf&&) = delete;
	auto operator=(OutputPortOf const&) -> OutputPortOf& = delete;
	auto operator=(OutputPortOf&&) -> OutputPortOf& = delete;
	~OutputPortOf() final;

	auto get() noexcept -> T&;
	auto get() const noexcept -> T const&;
	auto operator*() noexcept -> T&;
	auto operator*() const noexcept -> T const&;
	auto operator->() noexcept -> T*;
	auto operator->() const noexcept -> T const*;
	auto getDataTypeHash() const noexcept -> std::size_t final;
	auto isConnected() const noexcept -> bool final;
	auto canConnectTo(Port const& other) const noexcept -> bool final;
	auto isConnectedTo(Port const& other) const noexcept -> bool final;
	void connectTo(CompatiblePort& other);
	void connectTo(Port& other) final;
	void disconnectFrom(CompatiblePort& other);
	void disconnectFrom(Port& other) final;
	void disconnect() final;
	auto getConnectedPorts() const -> std::unordered_set<Port*> final;
	auto getConnectedInputPorts() const -> std::unordered_set<InputPort*> final;
	void push(std::weak_ptr<Sentinel> const& sentinel = {}) noexcept final;

private:
	T data = {};
	std::unordered_set<CompatiblePort*> connections;
};

template <typename T>
OutputPortOf<T>::OutputPortOf(std::string name)
{
	setName(std::move(name));
}

template <typename T>
OutputPortOf<T>::~OutputPortOf()
{
	disconnect();
}

template <typename T>
auto OutputPortOf<T>::getDataTypeHash() const noexcept -> std::size_t
{
	static std::size_t hash = std::type_index(typeid(T)).hash_code();
	return hash;
}

template <typename T>
auto OutputPortOf<T>::isConnected() const noexcept -> bool
{
	return !connections.empty();
}

template <typename T>
auto OutputPortOf<T>::canConnectTo(Port const& other) const noexcept -> bool
{
	return dynamic_cast<CompatiblePort const*>(&other);
}

template <typename T>
auto OutputPortOf<T>::isConnectedTo(Port const& other) const noexcept -> bool
{
	auto concrete = dynamic_cast<CompatiblePort const*>(&other);
	if(concrete == nullptr)
		return false;

	return std::any_of(
		connections.begin(), connections.end(), [&concrete](auto const* port) { return port == concrete; });
}

template <typename T>
void OutputPortOf<T>::connectTo(CompatiblePort& other)
{
	connections.insert(&other);
	if(!other.isConnectedTo(*this))
		other.connectTo(*this);
}

template <typename T>
void OutputPortOf<T>::connectTo(Port& other)
{
	connectTo(dynamic_cast<CompatiblePort&>(other));
}

template <typename T>
void OutputPortOf<T>::disconnectFrom(CompatiblePort& other)
{
	connections.erase(&other);
	if(other.isConnectedTo(*this))
		other.disconnect();
}

template <typename T>
void OutputPortOf<T>::disconnectFrom(Port& other)
{
	auto concrete = dynamic_cast<CompatiblePort*>(&other);
	if(concrete)
		disconnectFrom(*concrete);
}

template <typename T>
void OutputPortOf<T>::disconnect()
{
	while(!connections.empty())
		disconnectFrom(**connections.begin());
}

template <typename T>
auto OutputPortOf<T>::getConnectedPorts() const -> std::unordered_set<Port*>
{
	std::unordered_set<Port*> connections;
	for(auto connection : this->connections)
		connections.insert(connection);
	return connections;
}

template <typename T>
auto OutputPortOf<T>::getConnectedInputPorts() const -> std::unordered_set<InputPort*>
{
	std::unordered_set<InputPort*> connections;
	for(auto connection : this->connections)
		connections.insert(connection);
	return connections;
}

template <typename T>
void OutputPortOf<T>::push(std::weak_ptr<Sentinel> const& sentinel) noexcept
{
	for(auto connection : connections)
		connection->push(sentinel);
}

template <typename T>
auto OutputPortOf<T>::get() noexcept -> T&
{
	updateTimestamp();
	return data;
}

template <typename T>
auto OutputPortOf<T>::get() const noexcept -> T const&
{
	return data;
}

template <typename T>
auto OutputPortOf<T>::operator*() noexcept -> T&
{
	updateTimestamp();
	return get();
}

template <typename T>
auto OutputPortOf<T>::operator*() const noexcept -> T const&
{
	return get();
}

template <typename T>
auto OutputPortOf<T>::operator->() noexcept -> T*
{
	updateTimestamp();
	return &data;
}

template <typename T>
auto OutputPortOf<T>::operator->() const noexcept -> T const*
{
	return &data;
}

} // namespace rsp