#pragma once
#include "rshp/base/Sink.hpp"

#include <glm/glm.hpp>
#include <array>

namespace rshp::pipes
{

	class ClearBackgroundSink : public rshp::base::Sink<ClearBackgroundSink, rshp::base::InputList<glm::vec3>>
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

}