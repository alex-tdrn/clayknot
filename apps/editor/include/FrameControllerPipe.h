#pragma once
#include "rshp/base/Pipe.hpp"

#include <array>

class FrameControllerPipe final : public rshp::base::Pipe<FrameControllerPipe, 
	rshp::base::InputList<>, rshp::base::OutputList<>>
{
public:
	struct OutputEvents
	{
		enum
		{
			NewFrame = AbstractPipe::OutputEvents::UserEvents
		};
	};

public:
	static inline std::string const name = registerPipe<FrameControllerPipe>("Frame Controller Pipe");

protected:
	void registerOutputEvents() override;

public:
	void update() override;
	void newFrame();

};