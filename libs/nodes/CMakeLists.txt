add_library(rendershop-nodes
	"src/ClearBackgroundSink.cpp"
	"src/DecomposeColor.cpp"
	"src/GrayscaleColorNode.cpp"
	"src/MixColors.cpp"
	"src/RandomColorSource.cpp"
	"src/Timer.cpp"
	"src/ValueToColor.cpp"
	"include/rshp/nodes/ClearBackgroundSink.h"
	"include/rshp/nodes/DecomposeColor.h"
	"include/rshp/nodes/GrayscaleColorNode.h"
	"include/rshp/nodes/MixColors.h"
	"include/rshp/nodes/RandomColorSource.h"
	"include/rshp/nodes/Timer.hpp"
	"include/rshp/nodes/ValueToColor.h"
)

target_include_directories(rendershop-nodes 
	PUBLIC
	"include"
	"../../external/glm/include"
)

target_link_libraries(rendershop-nodes
	PUBLIC
	rendershop-base
	PRIVATE
	glad
)

target_compile_features(rendershop-nodes PUBLIC cxx_std_17)