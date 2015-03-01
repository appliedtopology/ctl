# The Build System

CTL’s build system relies on [CMake](http://cmake.org) in order to manage a large number of configuration options in a platform-independent manner; it can be easily configured to build on Linux and Unix environments (including Darwin), and, at least in theory, various versions of Microsoft Windows. 

**A relatively up-to-date C++11 compiler (e.g., g++ >= 4.7)** is required in all cases.

CTL’s main dependencies are:
- [CMake](http://www.cmake.org)
- [Boost](http://www.boost.org)
- [GTest](https://code.google.com/p/googletest/)
- [TBB](https://www.threadingbuildingblocks.org/)

CTL also depends upon:
- [METIS](http://glaros.dtc.umn.edu/gkhome/metis/metis/overview)
- [ANN](http://www.cs.umd.edu/~mount/ANN/)
- [Doxygen](http://www.stack.nl/~dimitri/doxygen/)

## Dependencies

[Recent](http://pkgs.org/) versions of Unix/Linux have sufficiently up-to-date versions of all dependencies. So for those running a fairly sane system with root access one of the following things should work:

### Quick Install (OS/X)
	brew tap homebrew/science
	brew install cmake tbb metis ann
	brew install boost ---build-from-source --with-c++11 --with-mpi --with-program_options --with-clang --without-single
    
### Quick Install (Red Hat)
	yum install cmake metis-devel ann-devel tbb-devel boost-devel
    
### Quick Install (Debian)
	apt-get install libann-dev libboost1.54-all-dev libtbb-dev libmetis-dev

We have had trouble with these sorts of instructions on Ubuntu 12.x and other older Linux distributions. Please see ctl/dependencies/ci_linux.sh for our hacked solution on Ubuntu 12.x

If this succeeds proceed below to [Building](##Building)

If the instructions so far have not helped you to escape dependency hell. Do not fear. We have tried to make such non-simple installs as _simple as possible_.  

If you are having trouble please do file an issue

## Local Dependencies
So you dont have root access, or, you don't want to do a system wide installation, or, you want to try and compile against some special version of a dependency. No problem! 

The [### Installing Local Dependencies](next sub section) has instructions on _installing_ local copies of software.

And then the [### Pointing CMake at the local dependencies](final sub section) explains how to easily point CMake at those local copies.


### Installing Local Dependencies
1. If the system does not have CMake, then follow the instructions below to install [### CMake (From Source)](CMake from source) 
2. Within /path/to/ctl:
		mkdir build
		cd build
		cmake ..
If you see: 
> The compiler /usr/bin/c++ has no C++11 support. Please use a different C++ compiler.      

Then first install an appropriate compiler then return here.

3. Observe the messages such as
> Boost not found. Install locally by typing 'make boost'
> METIS not found. Install locally by typing 'make metis'
OR
> Found Doxygen: /usr/bin/doxygen (found version "1.6.1") 

Now you may use our premade build scripts to download, build, and install the missing dependencies!

### Pointing CMake at the local dependencies
In order to 
1. Open /path/to/ctl/build_dependencies.txt
2. Uncomment the `set(...)` lines and change `XXX_FOUND 0` to `XXX_FOUND 1`
3. Fill in the `INCLUDE_DIR` and `LIBRARY_DIR` with the relavent data.

### CMake (From Source)
CTL uses several new CMake modules, so it is important to ensure that version 2.8.8 or later is installed. 

Thankfully the [installation process](http://www.cmake.org/cmake/help/install.html) is extremely straightforward: either [download](http://www.cmake.org/cmake/resources/software.html) a platform-specific binary from the downloads page, or instead grab the most recent stable tarball and have CMake bootstrap itself. In the simplest case, the bootstrap process is as simple as running the following commands:

For Local Installations:

	./bootstrap --prefix=~
	make
	make install

For System Wide Installations (as root):

	./bootstrap
	make
	make install

Ensure that your system can find CMake by adding:

	$PATH:$HOME/bin:$PATH 

to your terminals .profile (or equivalent filename)

Some highly optimizing compilers will not correctly build CMake, but the GNU compilers nearly always work. 

### Specifying Compilers With CMake

There are two ways to [specify compilers](http://www.cmake.org/Wiki/CMake_FAQ#How_do_I_use_a_different_compiler.3F) to CMake.
We summarize them below:

Method 1:

	CC=gcc-4.2 CXX=/usr/bin/g++-4.2 cmake -G "Your Generator" path/to/your/source

Method 2:

	cmake -G "Your Generator" -D CMAKE_C_COMPILER=gcc-4.2 -D CMAKE_CXX_COMPILER=g++-4.2 path/to/your/source


## Building
Though many configuration utilities, like [autoconf](http://www.gnu.org/software/autoconf/), are designed such that the user need only invoke ./configure && make && make install from the top-level source directory, CMake targets out-of-source builds, which is to say that the build process occurs away from the source code. The out-of-source build approach is ideal for projects that offer several different build modes, as each version of the project can be built in a separate folder.

A common approach is to create a folder named build in the top-level of the source directory and to invoke CMake from within it:

	mkdir build
	cd build
	cmake ..

# Citations
Most of the content of this page has been adapted from our friends at:
[libelemental](http://www.libelemental.org).