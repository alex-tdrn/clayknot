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
	void updateTimestamp() noexcept;
	virtual auto getTimestamp() const noexcept -> Timestamp;
	virtual auto getDataPointer() const noexcept -> void const* = 0;
	virtual auto getDataTypeHash() const noexcept -> std::size_t = 0;
	virtual auto isConnected() const noexcept -> bool = 0;
	virtual auto canConnectTo(Port const&) const noexcept -> bool = 0;
	virtual auto isConnectedTo(Port const&) const noexcept -> bool = 0;
	virtual void connectTo(Port&, bool notify = true) = 0;
	virtual void disconnectFrom(Port&, bool notify = true) = 0;
	virtual void disconnect(bool notify = true) = 0;
	virtual auto getConnectedPorts() const -> std::unordered_set<Port*> = 0;
	virtual void push(std::weak_ptr<Sentinel> const& sentinel = {}) noexcept = 0;
	virtual void pull(std::weak_ptr<Sentinel> const& sentinel = {}) noexcept = 0;
	virtual auto createCompatiblePort() const -> std::unique_ptr<Port> = 0;

protected:
	Port() = default;

private:
	std::string name = "Unnamed";
	Timestamp timestamp;
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

inline void Port::updateTimestamp() noexcept
{
	timestamp.update();
}

inline auto Port::getTimestamp() const noexcept -> Timestamp
{
	return timestamp;
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
	virtual auto getDefaultPort() const -> OutputPort& = 0;
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

	using Port::getDataPointer;
	virtual auto getDataPointer() noexcept -> void* = 0;
	using Port::connectTo;
	void connectTo(OutputPort&) = delete;
	virtual auto getConnectedInputPorts() const -> std::unordered_set<InputPort*> = 0;
	void setPullCallback(const std::function<void(std::weak_ptr<Sentinel> const&)>& callback);
	void setPullCallback(std::function<void(std::weak_ptr<Sentinel> const&)>&& callback) noexcept;
	void pull(std::weak_ptr<Sentinel> const& sentinel = {}) noexcept final;

protected:
	OutputPort() = default;

private:
	std::function<void(std::weak_ptr<Sentinel> const&)> pullCallback;
};

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
class InputPortOf;

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

	auto getDataPointer() const noexcept -> void const* final;
	auto getDataPointer() noexcept -> void* final;
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
	void connectTo(CompatiblePort& other, bool notify = true);
	void connectTo(Port& other, bool notify = true) final;
	void disconnectFrom(CompatiblePort& other, bool notify = true);
	void disconnectFrom(Port& other, bool notify = true) final;
	void disconnect(bool notify = true) final;
	auto getConnectedPorts() const -> std::unordered_set<Port*> final;
	auto getConnectedInputPorts() const -> std::unordered_set<InputPort*> final;
	void push(std::weak_ptr<Sentinel> const& sentinel = {}) noexcept final;
	auto createCompatiblePort() const -> std::unique_ptr<Port> final;

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

	return std::any_of(connections.begin(), connections.end(), [&concrete](auto const* port) {
		return port == concrete;
	});
}

template <typename T>
void OutputPortOf<T>::connectTo(CompatiblePort& other, bool notify)
{
	connections.insert(&other);
	if(!other.isConnectedTo(*this))
		other.connectTo(*this, notify);
}

template <typename T>
void OutputPortOf<T>::connectTo(Port& other, bool notify)
{
	connectTo(dynamic_cast<CompatiblePort&>(other), notify);
}

template <typename T>
void OutputPortOf<T>::disconnectFrom(CompatiblePort& other, bool notify)
{
	connections.erase(&other);
	if(other.isConnectedTo(*this))
		other.disconnect(notify);
}

template <typename T>
void OutputPortOf<T>::disconnectFrom(Port& other, bool notify)
{
	auto concrete = dynamic_cast<CompatiblePort*>(&other);
	if(concrete)
		disconnectFrom(*concrete, notify);
}

template <typename T>
void OutputPortOf<T>::disconnect(bool notify)
{
	while(!connections.empty())
		disconnectFrom(**connections.begin(), notify);
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
auto OutputPortOf<T>::createCompatiblePort() const -> std::unique_ptr<Port>
{
	auto port = std::make_unique<CompatiblePort>(getName());
	if constexpr(std::is_copy_assignable_v<T>)
		port->getDefaultPort().get() = this->get();
	return port;
}

template <typename T>
auto OutputPortOf<T>::getDataPointer() const noexcept -> void const*
{
	return &data;
}

template <typename T>
auto OutputPortOf<T>::getDataPointer() noexcept -> void*
{
	return &data;
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

template <typename T>
class InputPortOf final : public InputPort
{
public:
	using CompatiblePort = OutputPortOf<T>;

	InputPortOf();
	explicit InputPortOf(std::string name);
	InputPortOf(InputPortOf const&) = delete;
	InputPortOf(InputPortOf&&) = delete;
	auto operator=(InputPortOf const&) -> InputPortOf& = delete;
	auto operator=(InputPortOf&&) -> InputPortOf& = delete;
	~InputPortOf() final;

	auto getDataPointer() const noexcept -> void const* final;
	auto get() const noexcept -> T const&;
	auto operator*() const noexcept -> T const&;
	auto operator->() const noexcept -> T const*;
	auto getDataTypeHash() const noexcept -> std::size_t final;
	auto getTimestamp() const noexcept -> Timestamp final;
	auto isConnected() const noexcept -> bool final;
	auto canConnectTo(Port const& other) const noexcept -> bool final;
	auto isConnectedTo(Port const& other) const noexcept -> bool final;
	void connectTo(CompatiblePort& other, bool notify = true);
	void connectTo(Port& other, bool notify = true) final;
	void disconnect(bool notify = true) final;
	void disconnectFrom(Port& other, bool notify = true) final;
	auto getConnectedPorts() const -> std::unordered_set<Port*> final;
	auto getConnectedOutputPort() const -> OutputPort* final;
	auto getDefaultPort() const -> CompatiblePort& final;
	void pull(std::weak_ptr<Sentinel> const& sentinel = {}) noexcept final;
	auto createCompatiblePort() const -> std::unique_ptr<Port> final;

private:
	CompatiblePort mutable defaultPort = CompatiblePort("Default port");
	CompatiblePort mutable* connection = nullptr;
};

template <typename T>
InputPortOf<T>::InputPortOf()
{
	defaultPort.connectTo(*this, false);
}

template <typename T>
InputPortOf<T>::InputPortOf(std::string name)
{
	setName(std::move(name));
	defaultPort.connectTo(*this, false);
}

template <typename T>
InputPortOf<T>::~InputPortOf()
{
	disconnect(false);
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
		return std::max(defaultPort.getTimestamp(), Port::getTimestamp());
	return std::max(connection->getTimestamp(), Port::getTimestamp());
}

template <typename T>
auto InputPortOf<T>::isConnected() const noexcept -> bool
{
	return connection != nullptr;
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
void InputPortOf<T>::connectTo(CompatiblePort& other, bool notify)
{
	if(&other == &defaultPort)
		return;
	disconnect(false);
	connection = &other;
	if(!other.isConnectedTo(*this))
		other.connectTo(*this, false);
	updateTimestamp();
	if(notify)
		push();
}

template <typename T>
void InputPortOf<T>::connectTo(Port& other, bool notify)
{
	if(&other != &defaultPort)
		connectTo(dynamic_cast<CompatiblePort&>(other), notify);
}

template <typename T>
void InputPortOf<T>::disconnect(bool notify)
{
	if(connection)
	{
		auto oldConnection = connection;
		connection = nullptr;
		oldConnection->disconnectFrom(*this, false);
		updateTimestamp();
		if(notify)
			push();
	}
}

template <typename T>
void InputPortOf<T>::disconnectFrom(Port& other, bool notify)
{
	if(isConnectedTo(other))
		disconnect(notify);
}

template <typename T>
auto InputPortOf<T>::getConnectedPorts() const -> std::unordered_set<Port*>
{
	std::unordered_set<Port*> connections;
	if(connection != nullptr)
		connections.insert(connection);
	return connections;
}

template <typename T>
auto InputPortOf<T>::getConnectedOutputPort() const -> OutputPort*
{
	return connection;
}

template <typename T>
auto InputPortOf<T>::getDefaultPort() const -> CompatiblePort&
{
	return defaultPort;
}

template <typename T>
void InputPortOf<T>::pull(std::weak_ptr<Sentinel> const& sentinel) noexcept
{
	if(connection)
		connection->pull(sentinel);
	else
		defaultPort.pull(sentinel);
}

template <typename T>
auto InputPortOf<T>::createCompatiblePort() const -> std::unique_ptr<Port>
{
	auto port = std::make_unique<CompatiblePort>(getName());
	if constexpr(std::is_copy_assignable_v<T>)
		port->get() = this->get();
	return port;
}

template <typename T>
auto InputPortOf<T>::getDataPointer() const noexcept -> void const*
{
	return &get();
}

template <typename T>
auto InputPortOf<T>::get() const noexcept -> T const&
{
	if(connection)
		return connection->get();
	return defaultPort.get();
}

template <typename T>
auto InputPortOf<T>::operator*() const noexcept -> T const&
{
	return get();
}

template <typename T>
auto InputPortOf<T>::operator->() const noexcept -> T const*
{
	if(connection)
		return connection->operator->();
	return defaultPort.operator->();
}

} // namespace rsp