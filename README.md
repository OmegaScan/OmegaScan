## Prerequisites
    * cmake
    * g++

## Build
### quick satrt
To set up your build directory: 

	$ mkdir -p <path/to/omegascan>/build
	$ cd <path/to/omegascan>/build
	$ cmake ..

**Note:** all further commands listed below should be run from the `build` dir.

To build:

    $ make

You can use the `-j` switch to build in parallel, e.g.,

    $ make -j$(nproc)

### more options
To use other ui instead of ctl:
    
    $ cmake .. -DFTXUI

**Note:** src of FTXUI will be downloaded from https://github.com/ArthurSonzogni/ftxui and stored in <path/to/omegascan>/build/_deps/
