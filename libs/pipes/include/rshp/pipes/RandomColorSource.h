#pragma once
#include "rshp/base/Source.hpp"

#include <glm/glm.hpp>
#include <array>


class RandomColorSource : public Source<RandomColorSource, OutputList<glm::vec3>>
{
public:
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
	static inline std::string const name = registerPipe<RandomColorSource>("Random Color Source");

protected:
	void update() override;

};