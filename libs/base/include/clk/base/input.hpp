#pragma once

#include "clk/base/output.hpp"
#include "clk/base/port.hpp"
#include "clk/util/timestamp.hpp"

#include <cassert>
#include <cstddef>
#include <functional>
#include <memory>
#include <string_view>
#include <typeindex>
#include <utility>

namespace clk
{
class output;
class sentinel;

class input : public port
{
public:
    using port::port;

    input(input const&) = delete;
    input(input&&) = delete;
    auto operator=(input const&) -> input& = delete;
    auto operator=(input&&) -> input& = delete;
    ~input() override;

    auto timestamp() const noexcept -> clk::timestamp final;
    auto is_faulty() const noexcept -> bool final;

    auto can_connect_to(port const& other_port) const noexcept -> bool override;

    void connect_to(input& other_port) = delete;
    void connect_to(output& other_port, bool notify = true);
    void connect_to(port& other_port, bool notify = true) final;

    void disconnect_from(port& other_port, bool notify = true) final;

    void disconnect(bool notify = true) final;

    auto connected_ports() const -> std::vector<port*> const& final;

    auto connected_output() const -> output*;

    void push(std::weak_ptr<clk::sentinel> const& sentinel = {}) noexcept final;
    void pull(std::weak_ptr<clk::sentinel> const& sentinel = {}) noexcept final;
    void set_push_callback(std::function<void(std::weak_ptr<clk::sentinel> const&)> callback) noexcept;

    virtual auto default_port() const -> output& = 0;

private:
    output* _connection = nullptr;
    std::vector<port*> _cached_connected_ports = {};
    std::function<void(std::weak_ptr<clk::sentinel> const&)> _push_callback;
};

template <typename T>
class output_of;

template <typename T>
class input_of final : public input
{
public:
    input_of()
    {
        _default_port.connect_to(*this, false);
    }

    explicit input_of(std::string_view name) : input(name)
    {
        _default_port.connect_to(*this, false);
    }

    input_of(input_of const&) = delete;
    input_of(input_of&&) = delete;
    auto operator=(input_of const&) -> input_of& = delete;
    auto operator=(input_of&&) -> input_of& = delete;
    ~input_of() override = default;

    auto data_pointer() const noexcept -> void const* final
    {
        return &data();
    }

    auto data() const noexcept -> T const&
    {
        if(connected_output() == nullptr)
        {
            return _default_port.data();
        }
        else
        {
            assert(connected_output()->data_type_hash() == data_type_hash());
            return *(static_cast<T const*>(std::as_const(*connected_output()).data_pointer()));
        }
    }

    auto operator*() const noexcept -> T const&
    {
        return data();
    }

    auto operator->() const noexcept -> T const*
    {
        if(connected_output() == nullptr)
        {
            return _default_port.operator->();
        }
        else
        {
            assert(connected_output()->data_type_hash() == data_type_hash());
            return static_cast<T const*>(std::as_const(*connected_output()).data_pointer());
        }
    }

    auto data_type_hash() const noexcept -> std::size_t final
    {
        static std::size_t hash = std::type_index(typeid(T)).hash_code();
        return hash;
    }

    auto default_port() const -> output_of<T>& final
    {
        return _default_port;
    }

    auto create_compatible_port() const -> std::unique_ptr<port> final
    {
        auto port = std::make_unique<output_of<T>>(name());
        if constexpr(std::is_copy_assignable_v<T>)
            port->data() = this->data();
        return port;
    }

private:
    output_of<T> mutable _default_port = output_of<T>("Default port");
};

} // namespace clk
