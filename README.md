# Jukebox
## An OpenAL modular Jukebox written in C++
### Design
The idea for this jukebox is to abstract away most of the OpenAL ends of audio 
streaming by using class inheritance and overloaded methods. In order to add 
new audio formats to this system, simply inherit from the `source` class, 
overload the method `long read(ALuint buffer)` to load the correct frequency,
format, and data into a buffer, and that's it.

### Usage
To use the jukebox, run the following (where `$` is a shell input)
```
$ ./bin/main /path/to/audio/file
```

### Building
In order to build this project, the following libraries are necessary:
* OpenAL Soft (alhough Hard will work as well)
* Ogg
* Vorbis file

After installing the above libraries, run the following to build the jukebox.

```
$ make
```

Enjoy.

### To-Dos
* Automatic source selection by file format & suffix
* Better thread management
* More File formats