# simple-audio-player

**simple-audio-player** is a basic audio player writen in C++17 using the **simple-audio-library** has audio back-end. It's supporting a large variety of file format and support also parsing music collection.

## Features

**simple-audio-player** support playback of many audio files. You can choose between the file system and the music collection to play sound. For fast album cover loading, create an image and save it named **large_cover.png** or **small_cover.png** inside the album folder.

## Compilation

The simple-audio-player use the [**CMake**](https://cmake.org/) build generator, it needs Qt (Quick and Sql), the [**simple-audio-library**](https://github.com/BlueDragon28/simple-audio-library). If you compiled the **SAL** statically, you will also need [**PortAudio**](https://github.com/PortAudio/portaudio), [**FLAC++**](https://github.com/xiph/flac), [**libsndfile**](https://github.com/libsndfile/libsndfile) and [**taglib**](https://github.com/taglib/taglib).

### Windows

To compile on Windows, you must include the dependencies in the dependencies' folder. Each of the dependencies must be in the appropriate folder.

- [SAL](https://github.com/BlueDragon28/simple-audio-library) -> dependencies/simple-audio-library/
- [PortAudio](https://github.com/PortAudio/portaudio) -> dependencies/portaudio/
- [FLAC++](https://github.com/xiph/flac) -> dependencies/flac/
- [libsndfile](https://github.com/libsndfile/libsndfile) -> dependencies/libsndfile/
- [taglib](https://github.com/taglib/taglib) -> dependencies/taglib/
- [simple-audio-player-smtc](https://github.com/BlueDragon28/simple-audio-player-smtc) -> Interface with the Windows Runtime System Media Transport Control API. You need to compile it using the Visual Studio solution to the same compilation type and architecture of this project, and copy this files into dependencies/simple-audio-player-smtc (buildOutput must be changed with the real binary output) :
   - simple-audio-player-smtc/SAP_SMTC.h
   - *buildOutput*/simple-audio-player-smtc.lib
   - *buildOutput*/simple-audio-player-smtc.dll  
  
  If you do not want or can't compile simple-audio-player-smtc, you can disable it with the cmake option **USE_SMTC** `-DUSE_SMTC=off`. You will still be able to control playback with the media keys.

### Linux

Install the dependencies' library using your package manager, they are available in most distributions.

## License

The library is licensed under the **MIT** license. Check the [LICENSE](LICENSE) file.

Some of the images are from <a href="https://www.flaticon.com/free-icons/search" title="search icons">Search icons created by Chanut - Flaticon</a>.