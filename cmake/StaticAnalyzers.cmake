option(STATIC_ANALYSIS_USE_CLANG_TIDY "Run clang tidy during builds" ON)
option(STATIC_ANALYSIS_USE_CPPCHECK "Run cppcheck during builds" ON)
option(STATIC_ANALYSIS_USE_IWYU "Run include-what-you-use during builds" ON)

if(STATIC_ANALYSIS_USE_CLANG_TIDY)
	find_program(CLANG_TIDY_EXE clang-tidy)
	if(NOT CLANG_TIDY_EXE)
		message(WARNING "Clang tidy not found")
	else()
		message("Clang tidy enabled")
		set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXE} -header-filter=.*)
	endif()
endif()

if(STATIC_ANALYSIS_USE_CPPCHECK)
	find_program(CPPCHECK_EXE cppcheck)
	if(NOT CPPCHECK_EXE)
		message(WARNING "CppCheck not found")
	else()
		message("CppCheck enabled")
		set(CMAKE_CXX_CPPCHECK ${CPPCHECK_EXE} --enable=all
								--suppressions-list=${CMAKE_SOURCE_DIR}/cppcheck-suppressions --template=gcc
		)
	endif()
endif()

if(STATIC_ANALYSIS_USE_IWYU)
	find_program(IWYU_EXE NAMES iwyu include-what-you-use)
	if(NOT IWYU_EXE)
		message(WARNING "include-what-you-use not found")
	else()
		message("include-what-you-use enabled")
		set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE
			${IWYU_EXE}
			-Xiwyu
			--mapping_file=${CMAKE_SOURCE_DIR}/iwyu.imp
			-Xiwyu
			--quoted_includes_first
			-Xiwyu
			--no_fwd_decls
		)
	endif()
endif()

macro(suppress_static_analysis)
	message("Static analysis suppressed for " + "${CMAKE_CURRENT_SOURCE_DIR}")
	unset(CMAKE_CXX_CLANG_TIDY)
	unset(CMAKE_CXX_CPPCHECK)
	unset(CMAKE_CXX_INCLUDE_WHAT_YOU_USE)
endmacro()

macro(enable_extra_compiler_warnings)
	message("Enabled extra warnings for " + "${CMAKE_CURRENT_SOURCE_DIR}")
	if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W4")
	endif()
endmacro()
