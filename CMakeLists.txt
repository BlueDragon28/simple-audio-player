cmake_minimum_required(VERSION 3.16.0)
project(simple-audio-player VERSION 0.1 LANGUAGES CXX)

# Set C++ standard to C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6
find_package(Qt6 6.3.0 COMPONENTS Core Gui Quick QuickControls2 Sql REQUIRED)

if (UNIX)
	# Find simple-audio-library
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(SAL REQUIRED simple-audio-library_static)

	# include sal headers location
	include_directories(${SAL_INCLUDE_DIRS})

	# Find taglib library
	pkg_check_modules(TAGLIB REQUIRED taglib)
	
	# Add SAL library directory path.
	link_directories(${SAL_LIBRARY_DIRS})
elseif(WIN32)
	# Find simple-audio-library
	if ((EXISTS "${CMAKE_SOURCE_DIR}/dependencies/simple-audio-library/include/simple-audio-library/AudioPlayer.h") AND
		(EXISTS "${CMAKE_SOURCE_DIR}/dependencies/simple-audio-library/include/simple-audio-library/CallbackInterface.h"))

		# Check if SAL is static
		if (EXISTS "${CMAKE_SOURCE_DIR}/dependencies/simple-audio-library/lib/simple-audio-library.lib")
			# Set SAL library and include directory path.
			set(SAL_LIBRARIES ${CMAKE_SOURCE_DIR}/dependencies/simple-audio-library/lib/simple-audio-library.lib)
			include_directories(
				"${CMAKE_SOURCE_DIR}/dependencies/simple-audio-library/include/")

			# Find PortAudio
			if ((EXISTS "${CMAKE_SOURCE_DIR}/dependencies/portaudio/include/portaudio.h") AND
                                (EXISTS "${CMAKE_SOURCE_DIR}/dependencies/portaudio/lib/portaudio_x64.lib"))
				# Include directory of PortAudio
				include_directories(${CMAKE_SOURCE_DIR}/dependencies/portaudio/include)
                                set(SAL_LIBRARIES "${SAL_LIBRARIES}" "${CMAKE_SOURCE_DIR}/dependencies/portaudio/lib/portaudio_x64.lib")

				message(STATUS "PortAudio found")
			else()
				message(FATAL_ERROR "Cannot find portaudio")
			endif()

			# Find FLAC++
			if ((EXISTS "${CMAKE_SOURCE_DIR}/dependencies/flac/include/FLAC++/decoder.h") AND
				(EXISTS "${CMAKE_SOURCE_DIR}/dependencies/flac/lib/FLAC++.lib"))
				# Include directory of FLAC++ library
				include_directories("${CMAKE_SOURCE_DIR}/dependencies/flac/include/")
				# Set the FLAC++ library path
				set(SAL_LIBRARIES "${SAL_LIBRARIES}" "${CMAKE_SOURCE_DIR}/dependencies/flac/lib/FLAC++.lib")

				message(STATUS "FLAC++ found")
			else ()
				message(WARNING "Cannot find FLAC++ library")
			endif()

			# Find LIBSNDFILE
			if ((EXISTS "${CMAKE_SOURCE_DIR}/dependencies/libsndfile/include/sndfile.hh") AND
			    (EXISTS "${CMAKE_SOURCE_DIR}/dependencies/libsndfile/lib/sndfile.lib"))
				# Get informations from libsndfile using cmake find_package
				set(CMAKE_PREFIX_PATH 
					"${CMAKE_PREFIX_PATH}"
					"${CMAKE_SOURCE_DIR}/dependencies/libsndfile/cmake")
				find_package(SndFile REQUIRED)
				set(SAL_LIBRARIES "${SAL_LIBRARIES}" 
					${SndFile_LIBRARIES})

				message(STATUS "libsndfile found")
			elseif()
				message(WARNING "libsndfile not found")
			endif()
		endif()
	endif()
	
	# Find taglib
	if ((EXISTS "${CMAKE_SOURCE_DIR}/dependencies/taglib/include/taglib/tag.h") AND
	    (EXISTS "${CMAKE_SOURCE_DIR}/dependencies/taglib/include/taglib/fileref.h") AND
		(EXISTS "${CMAKE_SOURCE_DIR}/dependencies/taglib/include/taglib/tpropertymap.h") AND
		(EXISTS "${CMAKE_SOURCE_DIR}/dependencies/taglib/include/taglib/flacfile.h") AND
		(EXISTS "${CMAKE_SOURCE_DIR}/dependencies/taglib/include/taglib/flacpicture.h") AND
		(EXISTS "${CMAKE_SOURCE_DIR}/dependencies/taglib/include/taglib/attachedpictureframe.h") AND
		(EXISTS "${CMAKE_SOURCE_DIR}/dependencies/taglib/include/taglib/mpegfile.h") AND
		(EXISTS "${CMAKE_SOURCE_DIR}/dependencies/taglib/lib/tag.lib"))
			set(TAGLIB_LIBRARIES 
				"${CMAKE_SOURCE_DIR}/dependencies/taglib/lib/tag.lib")
			include_directories("${CMAKE_SOURCE_DIR}/dependencies/taglib/include/")
			message(STATUS "taglib found")
	endif()
endif()

# Search for every headers in sources file.
file(GLOB HEADERS include/*.h)
file(GLOB SOURCES src/*.cpp)

set(RESOURCES "simple-audio-player.qrc")

# Include the include directory
include_directories(include/)

# Create the executable.
qt_add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${RESOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Sql
    ${SAL_LIBRARIES}
    ${TAGLIB_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

qt_add_qml_module(${PROJECT_NAME} URI "SimpleAudioPlayer" VERSION 1.0)
