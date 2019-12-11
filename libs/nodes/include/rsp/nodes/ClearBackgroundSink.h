#pragma once

#include "rsp/base/ColorRGB.hpp"
#include "rsp/base/node/Sink.hpp"

#include <array>

namespace rsp::nodes
{
class ClearBackgroundSink : public rsp::Sink<ClearBackgroundSink, rsp::InputList<rsp::ColorRGB>>
{
public:
	struct InputPorts
	{
		static inline std::array names = {"Color"};
		enum
		{
			Color
		};
	};

public:
	static inline std::string const name = registerNode<ClearBackgroundSink>("Clear Background Sink");

protected:
	void update() override;
};

} // namespace rsp::nodes