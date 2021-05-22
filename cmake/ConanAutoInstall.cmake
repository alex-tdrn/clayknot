option(CONAN_AUTO_INSTALL "Automatically call conan install on configure" OFF)

if(CONAN_AUTO_INSTALL)
	find_program(CONAN_FOUND conan)
	if(CONAN_FOUND)
		if(WIN32 AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
			# force conan to use clang and Ninja as well, or else FindXXX files for some packages (e.g. range-v3)
			# contain msvc specific flags, such as \permissive-
			set(CONAN_COMPILER_VERSION ${CMAKE_CXX_COMPILER_VERSION})
			string(REPLACE "." ";" CONAN_COMPILER_VERSION_LIST ${CONAN_COMPILER_VERSION})
			list(GET CONAN_COMPILER_VERSION_LIST 0 CONAN_COMPILER_VERSION)

			execute_process(
				COMMAND
					conan install ${CMAKE_SOURCE_DIR} -if ${CMAKE_BINARY_DIR} --build missing -s
					build_type=${CMAKE_BUILD_TYPE} -s compiler=clang -s compiler.version=${CONAN_COMPILER_VERSION} -e
					CONAN_CMAKE_GENERATOR=${CMAKE_GENERATOR} -e CC=clang -e CXX=clang++
			)
		else()
			execute_process(
				COMMAND conan install ${CMAKE_SOURCE_DIR} -if ${CMAKE_BINARY_DIR} --build missing -s
						build_type=${CMAKE_BUILD_TYPE}
			)
		endif()
	else()
		message(WARNING "Could not find conan executable")
	endif()
endif()
