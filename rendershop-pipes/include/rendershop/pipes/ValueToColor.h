#pragma once
#include "rendershop/base/Pipe.hpp"

#include <glm/glm.hpp>
#include <array>

class ValueToColor : public Pipe<ValueToColor, InputList<float>, OutputList<glm::vec3>>
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
	static inline std::string const name = registerPipe<ValueToColor>("Value To Color");

public:
	void update() override;

};

