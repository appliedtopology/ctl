# The Build System

CTL’s build system relies on [CMake](http://cmake.org) in order to manage a large number of configuration options in a platform-independent manner; it can be easily configured to build on Linux and Unix environments (including Darwin), and, at least in theory, various versions of Microsoft Windows. 

**A relatively up-to-date C++11 compiler (e.g., gcc >= 4.7)** is required in all cases.

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

If you are trying to do a local installation, this requires a bit more work.
To make escaping dependency hell as simple as possible, we have taken steps to make the installation process as _simple_ as possible. If you are having trouble
please do write to [ctl@appliedtopology.org](mailto:ctl@appliedtopology.org)

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

There are two ways to specify compilers to CMake.


You can specify which compilers to use by setting the environment variables CC and CXX to the full paths to your preferred C and C++ compilers before running the bootstrap script.

## Building
Though many configuration utilities, like [autoconf](http://www.gnu.org/software/autoconf/), are designed such that the user need only invoke ./configure && make && make install from the top-level source directory, CMake targets out-of-source builds, which is to say that the build process occurs away from the source code. The out-of-source build approach is ideal for projects that offer several different build modes, as each version of the project can be built in a separate folder.

A common approach is to create a folder named build in the top-level of the source directory and to invoke CMake from within it:

	mkdir build
	cd build
	cmake ..
    
# Citations
Most of the content of this page has been adapted from our friends at:
[libelemental](http://www.libelemental.org).