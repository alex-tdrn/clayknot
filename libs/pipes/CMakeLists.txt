add_library(rendershop-pipes
	"src/ClearBackgroundSink.cpp"
	"src/DecomposeColor.cpp"
	"src/GrayscaleColorPipe.cpp"
	"src/MixColors.cpp"
	"src/RandomColorSource.cpp"
	"src/Timer.cpp"
	"src/ValueToColor.cpp"
	"include/rshp/pipes/ClearBackgroundSink.h"
	"include/rshp/pipes/DecomposeColor.h"
	"include/rshp/pipes/GrayscaleColorPipe.h"
	"include/rshp/pipes/MixColors.h"
	"include/rshp/pipes/RandomColorSource.h"
	"include/rshp/pipes/Timer.hpp"
	"include/rshp/pipes/ValueToColor.h"
)

target_include_directories(rendershop-pipes 
	PUBLIC
	"include"
	"../../external/glm/include"
)

target_link_libraries(rendershop-pipes
	PUBLIC
	rendershop-base
	PRIVATE
	glad
)

target_compile_features(rendershop-pipes PUBLIC cxx_std_17)