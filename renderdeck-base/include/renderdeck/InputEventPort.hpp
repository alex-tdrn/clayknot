#pragma once

#include "renderdeck/InputPort.hpp"
#include "renderdeck/OutputEventPort.hpp"

class OutputEventPort;

class InputEventPort : public InputPort<OutputEventPort>
{
public:
	InputEventPort() = default;
	InputEventPort(InputEventPort const&) = delete;
	InputEventPort(InputEventPort&&) = delete;
	InputEventPort& operator=(InputEventPort const& that) = delete;
	InputEventPort& operator=(InputEventPort&&) = delete;
	virtual ~InputEventPort() = default;

public:
	virtual void operator()() const = 0;
	virtual void trigger() const = 0;

};


template <typename F>
class InputEventPortImpl final : public InputEventPort
{
private:
	F callable;

public:
	InputEventPortImpl(F callable)
		: callable(callable)
	{

	}
	InputEventPortImpl(InputEventPortImpl const&) = delete;
	InputEventPortImpl(InputEventPortImpl&&) = default;
	InputEventPortImpl& operator=(InputEventPortImpl const& that) = delete;
	InputEventPortImpl& operator=(InputEventPortImpl&&) = default;
	~InputEventPortImpl() = default;

public:
	void trigger() const final override
	{
		callable();
	}

	void operator()() const final override
	{
		trigger();
	}

};
