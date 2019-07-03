#pragma once
#include <memory>

class AbstractDataPort;

class OutputDataPinDelegate
{
public:
	static std::unique_ptr<OutputDataPinDelegate> create(AbstractDataPort* port);

	virtual void draw() const = 0;
};