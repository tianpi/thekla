# Installation on Linux

## ACE

1. Get ACE from [here](https://download.dre.vanderbilt.edu/) (recommended version: 5.5.1)

1. Set environment variables:
    ```
    ACEROOT (eg /usr/local)
    ```

1. Add to LD_LIBRARY_PATH:
    ```
    ACEROOT/lib
    ```

1. See [ACE Install Instructions](http://www.cs.wustl.edu/~schmidt/ACE_wrappers/ACE-INSTALLhtml#unix_traditional)

1. Install [TProactor](http://www.terabit.com.au/solutions.php)

    On Unix systems the default ACE ProActor implementation is not working as expected. Please overwrite the corresponding source files with the alternative TProactor. See the "install.txt" in the TProactor package root for an HOWTO covering this task.


## Muddleware

Also see the Muddleware documentation for install instructions.

1. Checkout from SVN
    * https://svn.icg.tu-graz.ac.at/svn/Muddleware 
    * https://svn.icg.tu-graz.ac.at/svn/TinyXMLMod 

1. Set environment variables:
    ```
    MUDDLEWAREROOT
    TINYXMLMODROOT 
    WXWINDOWSROOT
    ```

1. Add to LD_LIBRARY_PATH:
    ```
    TINYXMLMODROOT/lib
    MUDDLEWAREROOT/lib
    ```

1. Compile TinyXMLMod:

    ```
    TINYXMLMODROOT/build/Linux $> scons install
    ```

1. Compile Muddleware:

    ```
    MUDDLEWAREROOT $> scons install
    ```

1. Test Muddleware Installation

    Try to start server and client (see Muddleware DOCs for an HOWTO).

    - Server: `MUDDLEWAREROOT/exec $> ./start_xmlserver.sh`
    - Client: `MUDDLEWAREROOT/bin $> ./XMLClientCL`

1. Don't forget to [prepare the Muddleware database contents](README.md#muddleware_database_preparation).



## Qt

Get Qt version 4.

1. Install from deb (Debian, Ubuntu) or rpm (SuSE, RedHat) package or source package.

    Download page: http://www.trolltech.com/developer/downloads/qt/x11

1. If you installed Qt to a non-standard path, set the following paths:

    1. Set environment variables: `QTDIR (eg /usr/share/qt4)`

    1. Add to LD_LIBRARY_PATH: location of libraries eg `/usr/local/lib`


## Coin2

1. Install from deb (Debian, Ubuntu) or rpm (SuSE, RedHat) package or source package.

    Download page: http://www.coin3d.org/lib/coin/releases

1. If you installed Coin to a non-standard path, set the following paths:

    1. Set environment variables: `COINROOT (eg /usr/local/include)`

    1. Add to LD_LIBRARY_PATH: location of libraries eg `/usr/local/lib`


## log4cplus 

You find the source package in `extras/log4cplus`

1. Set environment variables: 
    ```
    LOG4CPLUSROOT (eg THEKLAROOT/extras/log4cplus)
    ```

1. Add to LD_LIBRARY_PATH: 

    ```
    LOG4CPLUSROOT/src/.libs
    ```

1. Create `config.h`:

    Copy 
    
    `LOG4CPLUSROOT/include/log4cplus/config.h.default` 
    
    to 
    
    `LOG4CPLUSROOT/include/log4cplus/config.h`

1. Compile

    ```
    LOG4CPLUSROOT $> ./configure
    LOG4CPLUSROOT $> make
    ```

## (optional) CxxText

This package is needed to compile the THEKLA unit tests.

You find the source package in `extras/cxxtest`


1. Set environment variables:
    ```
    CXXTESTROOT (eg THEKLAROOT/extras/cxxtest)
    ```


## (optional) SoQt w/ Qt4

This package is needed to compile the "soqt4-viewer" example.

1. Download the source package from [here](http://www.coin3d.org/lib/soqt/releases)

1. Unpack to directory, configure and compile

    To use Qt4 (assumed that `QT4DIR` is `/usr/share/qt4`) and generate a `SoQt4` shared object (`SoQt` is default naming, adding suffix to avoid conflicts with `SoQt` library compiled with `Qt3`):

    ```
    $> ./configure --with-qt=/usr/share/qt4 --with-suffix="4"
    $> make
    ```

1. Set environment variables:
    ```
    SOQT4ROOT
    ```


## Thekla

1. Set environment variables:
    ```
    THEKLAROOT
    ```

1. Add to `LD_LIBRARY_PATH`:
    ```
    THEKLAROOT/lib
    ```

1. Compile the libraries

    ```
    THEKLAROOT $> make
    ```

    - (static) default Qt adapter library (libTheklaQtAdapters.a)
    - Thekla library (libThekla.so)
    - Qt Designer Plugin (libTheklaQDPlugin.so)


1. Install everything: Qt Designer Plugin + Thekla library (as root)

    ```
    THEKLAROOT $> sudo make install
    ```


### Optional Compile Targets

1. Install the Qt Designer Plugin (as root)

    ```
    THEKLAROOT $> make plugin-install
    ```

1. Other Build Options Overview

    Generate Qt adapters and compile (static) Qt adapters library from scratch

    - Edit the adapter/QObjectClassList.raw according to your needs (add any QObject classes for which Qt adapters should be generated and build for).

    - Generate Qt adapter classes (see QxyAdapter.h files) and compile (static) library that contains all generated Qt adapters.
        ```
        THEKLAROOT $> make adapter-gen
        ```

    - Re-create the Thekla library.

        ```
        THEKLAROOT $> make
        ```

1. Compile test cases (see `CxxTest`)

    Create tests:
    ```
    THEKLAROOT $> make test
    ```

    Execute tests (after starting Muddleware server):
    ```
    THEKLAROOT $> make test-run [ 2> /dev/null ]
    ```

1. Compile examples

    ```
    THEKLAROOT $> make examples
    ```

1. Generate or update doxygen documentation

    ```
    THEKLAROOT $> make docs
    ```


1. Clean compilation

    - Thekla library only:
        ```
        THEKLAROOT $> make thekla-clean`
        ```
    - Thekla Qt designer plugin only:
        ```
        THEKLAROOT $> make plugin-clean
        ```
    - Tests only:
        ```
        THEKLAROOT $> make test-clean
        ```
    - Examples only:
        ```
        THEKLAROOT $> make examples-clean
        ```
    - Doxygen docs only:
        ```
        THEKLAROOT $> make docs-clean
        ```
    - All:
        ```
        THEKLAROOT $> make clean
        ```
