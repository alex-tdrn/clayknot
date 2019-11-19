#pragma once
#include "rshp/base/Pipe.hpp"

#include <glm/glm.hpp>
#include <array>

namespace rshp::pipes
{

	class GrayscaleColorPipe : public rshp::base::Pipe<GrayscaleColorPipe, 
		rshp::base::InputList<glm::vec3>, rshp::base::OutputList<glm::vec3>>
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

		struct OutputPorts
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
		static inline std::string const name = registerPipe<GrayscaleColorPipe>("Grayscale Color Pipe");

	public:
		void update() override;

	};

}