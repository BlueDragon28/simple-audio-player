# simple-audio-player

**simple-audio-player** is a basic audio player wrote in C++17 using the **simple-audio-library** has audio back-end. It's supporting a large variety of file format and support also parsing music collection.

## Features

**simple-audio-player** support playback of many audio files. You can choose between the file system and the music collection to play sound. For fast album cover loading, create an image and save it named **large_cover.png** or **small_cover.png** inside the album folder.

## Compilation

The simple-audio-player use the **CMake** build generator, it needs Qt (Quick and Sql), the **simple-audio-library**. If you compiled the **SAL** statically, you will also need **PortAudio**, **FLAC++** and **libsndfile**.

### Windows

To compile on Windows, you must include the dependencies in the dependencies' folder. Each of the dependencies must be in the appropriate folder.

- SAL -> dependencies/simple-audio-library/
- PortAudio -> dependencies/portaudio/
- FLAC++ -> dependencies/flac/
- libsndfile -> dependencies/libsndfile/

### Linux

Install the dependencies' library using your package manager, they are available is most distributions.

## License

The library is licensed under the **MIT** license. Check the [LICENSE](LICENSE) file.