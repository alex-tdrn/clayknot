set(availableBuildTypes Debug Release)

list(APPEND availableBuildTypes ASan)
set(CMAKE_CXX_FLAGS_ASAN
	"-fsanitize=address -g -O0"
	CACHE STRING ""
)

list(APPEND availableBuildTypes MSan)
set(CMAKE_CXX_FLAGS_MSAN
	"-fsanitize=memory -O0"
	CACHE STRING ""
)

list(APPEND availableBuildTypes UBSan)
set(CMAKE_CXX_FLAGS_UBSAN
	"-fsanitize=undefined -O0"
	CACHE STRING ""
)

list(APPEND availableBuildTypes TSan)
set(CMAKE_CXX_FLAGS_TSAN
	"-fsanitize=thread -O0"
	CACHE STRING ""
)

get_property(generatorIsMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(generatorIsMultiConfig)
	set(CMAKE_CONFIGURATION_TYPES availableBuildTypes)
else()
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "${availableBuildTypes}")
	if(NOT CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE
			Debug
			CACHE STRING "" FORCE
		)
	elseif(NOT CMAKE_BUILD_TYPE IN_LIST availableBuildTypes)
		message(FATAL_ERROR "Invalid build type: ${CMAKE_BUILD_TYPE}\nAvailable build types: ${availableBuildTypes}")
	endif()
endif()
