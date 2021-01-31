add_library(
	rendershop-algorithms
	"src/DecomposeColor.cpp"
	"include/rsp/algorithms/DecomposeColor.h"
	"src/GrayscaleColorNode.cpp"
	"include/rsp/algorithms/GrayscaleColorNode.h"
	"src/MixColors.cpp"
	"include/rsp/algorithms/MixColors.h"
	"src/RandomColorSource.cpp"
	"include/rsp/algorithms/RandomColorSource.h"
	"src/ValueToColor.cpp"
	"include/rsp/algorithms/ValueToColor.h"
)

target_include_directories(rendershop-algorithms PUBLIC "include")

target_link_libraries(rendershop-algorithms PUBLIC rendershop-base glm::glm)
