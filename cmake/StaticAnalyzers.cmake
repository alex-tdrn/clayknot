option(STATIC_ANALYSIS_ENABLED "Run static analyzers during build" OFF)

if(STATIC_ANALYSIS_ENABLED)
	option(STATIC_ANALYSIS_USE_CLANG_TIDY "Run clang tidy during builds" ON)
	option(STATIC_ANALYSIS_USE_CPPCHECK "Run cppcheck during builds" ON)
	option(STATIC_ANALYSIS_USE_IWYU "Run include-what-you-use during builds" ON)

	if(STATIC_ANALYSIS_USE_CLANG_TIDY)
		find_program(CLANG_TIDY_EXE clang-tidy)
		if(NOT CLANG_TIDY_EXE)
			message(NOTICE "Clang tidy not found")
		else()
			if(CMAKE_CXX_COMPILER STREQUAL "cl" AND CMAKE_GENERATOR STREQUAL "Ninja")
				message(NOTICE "CMake clang tidy integration seems to break builds when using cl with Ninja")
			else()
				message("Clang tidy enabled")
				set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXE} -header-filter=.*)
			endif()
		endif()
	endif()

	if(STATIC_ANALYSIS_USE_CPPCHECK)
		find_program(CPPCHECK_EXE cppcheck)
		if(NOT CPPCHECK_EXE)
			message(NOTICE "CppCheck not found")
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
			message(NOTICE "include-what-you-use not found")
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

endif()

macro(suppress_static_analysis)
	message("Static analysis suppressed for " + "${CMAKE_CURRENT_SOURCE_DIR}")
	unset(CMAKE_CXX_CLANG_TIDY)
	unset(CMAKE_CXX_CPPCHECK)
	unset(CMAKE_CXX_INCLUDE_WHAT_YOU_USE)
endmacro()

macro(disable_all_warnings)
	message("Disable all warnings for " + "${CMAKE_CURRENT_SOURCE_DIR}")
	if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		add_compile_options(-w)
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		add_compile_options(/w)
	endif()
endmacro()

macro(enable_extra_compiler_warnings)
	message("Enabled extra warnings for " + "${CMAKE_CURRENT_SOURCE_DIR}")
	if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		add_compile_options(-Wall -Wextra -Wpedantic)
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		add_compile_options(/W4)
	endif()
endmacro()
