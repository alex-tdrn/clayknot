#pragma once
#include "rshp/base/Sink.hpp"

#include <glm/glm.hpp>
#include <array>

class ClearBackgroundSink : public Sink<ClearBackgroundSink, InputList<glm::vec3>>
{
public:
	struct InputPorts
	{
		static inline std::array names = {
			"Color"
		};
		enum
		{
			Color
		};
	};

public:
	static inline std::string const name = registerPipe<ClearBackgroundSink>("Clear Background Sink");

protected:
	void update() override;

};

