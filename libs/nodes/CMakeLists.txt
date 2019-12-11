add_library(rendershop-nodes
	"src/ClearBackgroundSink.cpp"
	"include/rsp/nodes/ClearBackgroundSink.h"

	"src/DecomposeColor.cpp"
	"include/rsp/nodes/DecomposeColor.h"

	"src/GrayscaleColorNode.cpp"
	"include/rsp/nodes/GrayscaleColorNode.h"

	"src/MixColors.cpp"
	"include/rsp/nodes/MixColors.h"

	"src/RandomColorSource.cpp"
	"include/rsp/nodes/RandomColorSource.h"

	"src/Timer.cpp"
	"include/rsp/nodes/Timer.hpp"
	
	"src/ValueToColor.cpp"
	"include/rsp/nodes/ValueToColor.h"
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