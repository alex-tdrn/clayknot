#pragma once

#include "rsp/base/port/InputPort.hpp"
#include "rsp/base/port/OutputEventPort.h"

#include <memory>

namespace rsp
{
namespace detail
{
class TypeErasedCallable
{
public:
	virtual void call() const = 0;
};

template <typename F>
class Callable final : public TypeErasedCallable
{
private:
	F callable;

public:
	Callable(F&& callable) : callable(callable)
	{
	}
	void call() const final override
	{
		callable();
	}
};
} // namespace detail

class OutputEventPort;

class InputEventPort : public InputPort<OutputEventPort>
{
private:
	std::unique_ptr<detail::TypeErasedCallable> f;
	mutable int timesTriggered = 0;

public:
	template <typename F>
	InputEventPort(F&& callable) : f(std::make_unique<detail::Callable<F>>(std::forward<F>(callable)))
	{
	}
	InputEventPort(InputEventPort const&) = delete;
	InputEventPort(InputEventPort&&) = default;
	InputEventPort& operator=(InputEventPort const& that) = delete;
	InputEventPort& operator=(InputEventPort&&) = default;
	virtual ~InputEventPort() = default;

public:
	int getTimesTriggered() const;
	void trigger() const;
	void operator()() const;
};

} // namespace rsp