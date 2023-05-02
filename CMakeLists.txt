cmake_minimum_required(VERSION 3.16.0)
project(simple-audio-player VERSION 0.2 LANGUAGES CXX)

# Set C++ standard to C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# If on Unix, adding the DBus Qt Module.
if (UNIX)
	set(FIND_QT_DBUS_MODULE "DBus")
	set(FIND_QT_DBUS_LIBS "Qt6::DBus")

	set (DBUS_HEADERS 
		"include/dbus/SAPMPris.h")
	set (DBUS_SRC
		"src/dbus/SAPMPris.cpp")
elseif (WIN32)
    # Including only on Windows the ListenMediaKeys object to listen for media keys.
    set(LISTEN_MEDIA_KEYS_SRC
        "include/windows/ListenMediaKeys.h"
        "src/windows/ListenMediaKeys.cpp")

    # Option to use between keys hooks and System Media Transport Control interface for media control.
    option(USE_SMTC "Use the System Media Transport Control interface instead of a key hook to listen for media control" ON)
endif()

# Find Qt6
find_package(Qt6 6.3.0 COMPONENTS Core Gui Quick QuickControls2 Sql ${FIND_QT_DBUS_MODULE} Widgets Network REQUIRED)
find_package(Qt6Keychain REQUIRED)

# Include the include directory
include_directories(
    include/ 
    include/options 
    include/network 
    include/spotify 
    "${CMAKE_SOURCE_DIR}/dependencies/httplib"
    "${CMAKE_BINARY_DIR}/include")

if (UNIX)
	# Find simple-audio-library
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(SAL REQUIRED simple-audio-library)

	# include sal headers location
	include_directories(${SAL_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

	# Find taglib library
	pkg_check_modules(TAGLIB REQUIRED taglib)
	
	# Add SAL library directory path.
	link_directories(${SAL_LIBRARY_DIRS})

	# Generate dbus mpris adaptor
	qt6_add_dbus_adaptor(DBUS_SRC "include/dbus/org.mpris.MediaPlayer2.xml" "dbus/SAPMPris.h" SAPMPris dbus/mprisRoot MPrisRoot)
	qt6_add_dbus_adaptor(DBUS_SRC "include/dbus/org.mpris.MediaPlayer2.Player.xml" "dbus/SAPMPris.h" SAPMPris dbus/mprisPlayer MPrisPlayer)
elseif(WIN32)
	# Find simple-audio-library
	if ((EXISTS "${CMAKE_SOURCE_DIR}/dependencies/simple-audio-library/include/simple-audio-library/AudioPlayer.h") AND
		(EXISTS "${CMAKE_SOURCE_DIR}/dependencies/simple-audio-library/include/simple-audio-library/CallbackInterface.h"))

                # Check for the SAL library file.
                if (EXISTS "${CMAKE_SOURCE_DIR}/dependencies/simple-audio-library/lib/simple-audio-library.lib")
			# Set SAL library and include directory path.
			set(SAL_LIBRARIES ${CMAKE_SOURCE_DIR}/dependencies/simple-audio-library/lib/simple-audio-library.lib)
			include_directories(
				"${CMAKE_SOURCE_DIR}/dependencies/simple-audio-library/include/")


                        # Find PortAudio
                        if ((EXISTS "${CMAKE_SOURCE_DIR}/dependencies/portaudio/include/portaudio.h") AND
			    ((EXISTS "${CMAKE_SOURCE_DIR}/dependencies/portaudio/lib/portaudio_x64.lib") OR
			     (EXISTS "${CMAKE_SOURCE_DIR}/dependencies/portaudio/lib/portaudio.lib")))
                                # Include directory of PortAudio
                                include_directories(${CMAKE_SOURCE_DIR}/dependencies/portaudio/include)
				if (EXISTS "${CMAKE_SOURCE_DIR}/dependencies/portaudio/lib/portaudio_x64.lib")
				    set(SAL_LIBRARIES "${SAL_LIBRARIES}" "${CMAKE_SOURCE_DIR}/dependencies/portaudio/lib/portaudio_x64.lib")
				else()
				    set(SAL_LIBRARIES "${SAL_LIBRARIES}" "${CMAKE_SOURCE_DIR}/dependencies/portaudio/lib/portaudio.lib")
				endif()

                                message(STATUS "PortAudio found")
                        else()
                                message(FATAL_ERROR "Cannot find portaudio")
                        endif()

                        # Include only the dependencies only if SAL is compiled statically
                        if (NOT EXISTS "${CMAKE_SOURCE_DIR}/dependencies/simple-audio-library/bin/simple-audio-library.dll")
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
                            else()
                                    message(WARNING "libsndfile not found")
                            endif()
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
        else()
            message(FATAL_ERROR "taglib not found")
	endif()

        if (USE_SMTC)
            # Find simple-audio-player-smtc
            if ((EXISTS "${CMAKE_SOURCE_DIR}/dependencies/simple-audio-player-smtc/simple-audio-player-smtc.lib") AND
                    (EXISTS "${CMAKE_SOURCE_DIR}/dependencies/simple-audio-player-smtc/SAP_SMTC.h"))

                # Store the path to the lib into a variable
                set(SAP_SMTC_LIB "${CMAKE_SOURCE_DIR}/dependencies/simple-audio-player-smtc/simple-audio-player-smtc.lib")
                message(STATUS "simple-audio-library-smtc found")

                # Include the header directory.
                include_directories("${CMAKE_SOURCE_DIR}/dependencies/simple-audio-player-smtc/")

                # Include the SMTCInterface object.
                set(SMTC_INTERFACE_SRC
                    "include/windows/SMTCInterface.h"
                    "src/windows/SMTCInterface.cpp")
            endif()
        endif()
		
    set (WIN32_EXECUTABLE WIN32)
endif()

# Set the headers and sources files.
set(HEADERS
    "include/AlbumCollectionModel.h"
    "include/AlbumTrackListModel.h"
    "include/AppConfig.h"
    "include/BaseCollectionModel.h"
    "include/CoverArtTag.h"
    "include/CoverImageProvider.h"
    "include/FileSize.h"
    "include/FileSystemModel.h"
    "include/MusicCollectionList.h"
    "include/Player.h"
    "include/PlayingList.h"
    "include/SectionListModel.h"
    "include/SelectionModel.h"
    "include/SystemMediaControl.h"
    "include/TrackTag.h"
    "include/PlaylistModel.h"
    "include/AddToPlaylistInterface.h"
    "include/SystemFileDialog.h"
	"include/options/SAPOptions.h"
	"include/options/AboutDialog.h"
	"include/options/LicenseDialog.h"
    "include/network/NetworkManager.h"
    "include/spotify/SpotifyAPI.h"
    "include/spotify/SpotifyAuthorizationPKCE.h"
    "include/spotify/SpotifyHttpCodeListener.h"
    "include/spotify/SpotifyUserInfo.h"
    "include/spotify/SpotifyTokenSaver.h"
    "dependencies/httplib/httplib.h")

set(SOURCES
    "src/main.cpp"
    "src/AlbumCollectionModel.cpp"
    "src/AlbumTrackListModel.cpp"
    "src/AppConfig.cpp"
    "src/BaseCollectionModel.cpp"
    "src/CoverArtTag.cpp"
    "src/CoverImageProvider.cpp"
    "src/FileSize.cpp"
    "src/FileSystemModel.cpp"
    "src/MusicCollectionList.cpp"
    "src/Player.cpp"
    "src/PlayingList.cpp"
    "src/SectionListModel.cpp"
    "src/SelectionModel.cpp"
    "src/SystemMediaControl.cpp"
    "src/TrackTag.cpp"
    "src/PlaylistModel.cpp"
    "src/AddToPlaylistInterface.cpp"
    "src/SystemFileDialog.cpp"
	"src/options/SAPOptions.cpp"
	"src/options/AboutDialog.cpp"
	"src/options/LicenseDialog.cpp"
    "src/network/NetworkManager.cpp"
    "src/spotify/SpotifyAPI.cpp"
    "src/spotify/SpotifyAuthorizationPKCE.cpp"
    "src/spotify/SpotifyHttpCodeListener.cpp"
    "src/spotify/SpotifyUserInfo.cpp"
    "src/spotify/SpotifyTokenSaver.cpp")

set(RESOURCES "simple-audio-player.qrc")

# Configure the config.h.in file.
configure_file(include/config.h.in "${CMAKE_BINARY_DIR}/include/config.h")

# Create the executable.
qt_add_executable(${PROJECT_NAME} ${WIN32_EXECUTABLE} ${HEADERS} ${SOURCES} ${RESOURCES} ${DBUS_HEADERS} ${DBUS_SRC} ${LISTEN_MEDIA_KEYS_SRC} ${SMTC_INTERFACE_SRC})
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Sql
	Qt6::Widgets
    Qt6::Network
    ${QTKEYCHAIN_LIBRARIES}
    ${FIND_QT_DBUS_LIBS}
    ${SAL_LIBRARIES}
    ${TAGLIB_LIBRARIES}
    "${SAP_SMTC_LIB}")
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

qt_add_qml_module(${PROJECT_NAME} URI "SimpleAudioPlayer" VERSION 1.0)
