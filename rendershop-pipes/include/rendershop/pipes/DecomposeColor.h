#pragma once
#include "rendershop/base/Pipe.hpp"

#include <glm/glm.hpp>
#include <array>

class DecomposeColor : public Pipe<DecomposeColor, InputList<glm::vec3>, OutputList<float, float, float>>
{
public:
	struct InputPorts
	{
		static inline std::array names = {
			"Color"
		};
		enum {
			Color
		};

	};

	struct OutputPorts
	{
		static inline std::array names = {
			"R",
			"G",
			"B"
		};
		enum {
			R,
			G,
			B
		};
	};

public:
	static inline std::string const name = registerPipe<DecomposeColor>("Decompose Colors");

public:
	void update() override;

};

