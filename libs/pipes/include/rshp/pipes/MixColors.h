#pragma once
#include "rshp/base/Pipe.hpp"

#include <glm/glm.hpp>
#include <array>

namespace rshp::pipes
{

	class MixColors : public rshp::base::Pipe<MixColors, 
		rshp::base::InputList<float, glm::vec3, glm::vec3>, rshp::base::OutputList<glm::vec3>>
	{
	public:
		struct InputPorts
		{
			static inline std::array names = {
				"Factor",
				"Color A",
				"Color B"
			};
			enum {
				Factor,
				ColorA,
				ColorB
			};
		};

		struct OutputPorts
		{
			static inline std::array names = {
				"Mixed Color"
			};
			enum {
				MixedColor
			};

		};

	public:
		static inline std::string const name = registerPipe<MixColors>("Mix Colors");

	public:
		void update() override;

	};

}