add_library(rendershop-nodes
	"src/ClearBackgroundSink.cpp"
	"include/rshp/nodes/ClearBackgroundSink.h"

	"src/DecomposeColor.cpp"
	"include/rshp/nodes/DecomposeColor.h"

	"src/GrayscaleColorNode.cpp"
	"include/rshp/nodes/GrayscaleColorNode.h"

	"src/MixColors.cpp"
	"include/rshp/nodes/MixColors.h"

	"src/RandomColorSource.cpp"
	"include/rshp/nodes/RandomColorSource.h"

	"src/Timer.cpp"
	"include/rshp/nodes/Timer.hpp"
	
	"src/ValueToColor.cpp"
	"include/rshp/nodes/ValueToColor.h"
)

target_include_directories(rendershop-nodes 
	PUBLIC
	"include"
	"../../external/glm/include"
)

target_link_libraries(rendershop-nodes
	PRIVATE
	rendershop-base
	glad
)