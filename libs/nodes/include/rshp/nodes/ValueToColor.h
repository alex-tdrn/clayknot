#pragma once
#include "rshp/base/node/Node.hpp"

#include <glm/glm.hpp>
#include <array>

namespace rshp::nodes
{

	class ValueToColor : public rshp::base::Node<ValueToColor, 
		rshp::base::InputList<float>, rshp::base::OutputList<glm::vec3>>
	{
	public:
		struct InputPorts
		{
			static inline std::array names = {
				"Value"
			};
			enum {
				Value
			};

		};
		struct OutputPorts
		{
			static inline std::array names = {
				"Color"
			};
			enum {
				Color
			};
		};

	public:
		static inline std::string const name = registerNode<ValueToColor>("Value To Color");

	public:
		void update() override;

	};

}