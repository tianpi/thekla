# Installation on Windows

You need Visual Studio C++ 2005 and build for Win32.

## ACE

1. Get ACE from [here](https://download.dre.vanderbilt.edu/) (recommended version: 5.5.1)

1. Set environment variables:

    ```
    ACEROOT (eg c:/libs/ACE_wrappers)
    ```

1.  Add to `PATH`:
    ```
    ACEROOT/lib
    ```

1. See Instructions on 
http://www.cs.wustl.edu/~schmidt/ACE_wrappers/ACE-INSTALL.html#msvc 

1. Create ace/config.h similar to this one
    ```
    <quote> 
    #define ACE_HAS_STANDARD_CPP_LIBRARY 1 
    #include "ace/config-win32.h"
    </quote> 
    ```

1. Open solution: `ACEROOT/ACE_vc8.sln`

1. Build project "ACE" in "Debug" and "Release" mode (`Ace.lib` and `Aced.lib`)


## Muddleware

Also see the Muddleware documentation for install instructions.

1. Checkout from SVN

    * https://svn.icg.tu-graz.ac.at/svn/Muddleware to `Muddleware/trunk`

    * https://svn.icg.tu-graz.ac.at/svn/TinyXMLMod to `Libs/TinyXML_Mod` (without trunk!)

        **NOTE**: Put `TinyXMLMod` exactly in this directory relative to Muddleware (`../../Libs/TinyXML_Mod`), otherwise the Muddleware project files will not work  out of the box.

        **NOTE2**: Most propably you'll have to change the ACE library source directory in the project settings from 

        - `ACEROOT/lib/win32` to `ACEROOT/lib` 

            *OR* 

        - copy the contents of `ACEROOT/lib` to `ACEROOT/lib/win32`


1. Set environment variables:

    ```
    MUDDLEWAREROOT
    TINYXMLMODROOT 
    ```

1. Add to PATH:
    ```
    TINYXMLMODROOT/bin/win32
    MUDDLEWAREROOT/bin/win32 
    ```

1. Compile the following Solutions/Projects in "Debug" and "Release" mode:
    ```
    TinyXML_Mod 
    XMLServer
    XMLServerWin
    XMLClient 
    XMLClientWin 
    ```

    **NOTE**: Per default: libs can be found in
    ```
    TINYXMLMODROOT/lib
    MUDDLEWAREROOT/lib
    ```

1. Test Muddleware Installation: Try to start server and client (see Muddleware DOCs for an HOWTO).

    ```
    Server: MUDDLEWAREROOT/exec/start_xmlserver.cmd
    Client: MUDDLEWAREROOT/bin/XMLClientWin.exe
    ```

1.  Don't forget to [prepare the Muddleware database contents](README.md#muddleware_database_preparation).


##  Qt

Get Qt version 4 (in the following: Qt 4.1.3)

1. Install Qt (`qt-win-commercial-4.1.3-vs2005.exe`)

    Setup:

    - Installation directory: `C:\libs\Qt\4.1.3`
    - Automatically set EnvVars: YES 
    - Register Ui files w/ QtDesigner:  YES 

    Results:

        ```
        QTDIR = C:\libs\Qt\4.1.3 
        PATH += C:\libs\Qt\4.1.3\bin 
        QMAKESPEC = win32-msvc2005 
        ```

1. Install Visual Studio plugin (`qt-vsintegration-1.1.1.exe`)
    
    Setup:

    - Register Ui files w/ VisualStudio: NO 


## Coin2

1. Download Windows binaries, no installer (VC8)
    - http://ftp.coin3d.org/coin/bin/win32/all/Coin-2.4.5-bin-msvc8.zip 
    - http://www.coin3d.org/lib/coin/releases

1. Unpack archive

1. Set environment variables:
    ```
    COINROOT (eg C:\XXX\Coin-2.4.5)
    ```

1. Add to PATH:

    ```
    COINROOT/bin
    ```


## log4cplus 

You find the source package in `extras/log4cplus`

1. Set environment variables:
    ```
    LOG4CPLUSROOT (eg THEKLAROOT/extras/log4cplus)
    ```

1. Add to PATH:
    ```
    LOG4CPLUSROOT/bin
    ```

1. Create `config.h`

    Copy `LOG4CPLUSROOT/include/log4cplus/config.h.default` to `LOG4CPLUSROOT/include/log4cplus/config.h`

1. Open VC2005 solution: `LOG4CPLUSROOT/vc.2005/log4cplus.sln`

1. Compile the `log4cplus_dll` project without UNICODE support (!) in "Debug" and "Release" mode.

    **NOTE**: Per default the libs go into: `LOG4CPLUSROOT/lib`


## (optional) CxxTest

This package is needed to compile the THEKLA unit tests.

You find the source package in `extras/cxxtest`

**TODO**: Currently not applied under Win32 (!)

For those who want to set it up: A good starting point are propably the instructions of the
CxxTest manual: http://cxxtest.sourceforge.net/guide.html#TOC32

Set environment variables:
```
CXXTESTROOT (eg THEKLAROOT/extras/cxxtest)
```

## (optional) SoQt w/ Qt4

This package is needed to compile the `soqt4-viewer` example.

Set environment variables:
```
SOQT4ROOT
```

## Thekla

1. Set environment variables:
    ```
    THEKLAROOT
    ```

1. Add to PATH:
    ```
    THEKLAROOT/bin
    ```

    The solution/project files are located in `build/VC8.XP`

1. Compile the following VC++ solutions:

    1. "Thekla" : The Thekla library 
    
        Results in:

        ```
        THEKLAROOT/lib/Thekla(d).lib
        THEKLAROOT/lib/Thekla(d).dll
        ```

    1. `TheklaQDPlugin`: The Qt Designer Plugin (dependent from `Thekla(d).dll`)

        Results in

        ```
        QTDIR/plugins/designer/TheklaQDPlugin(d).dll
        ```

    **NOTE**: The plugin DLL is only recognized by the Qt Designer if it is compiled with the same mode ("Debug" or "Release") as the Qt Designer itself (which is commonly "Release").


### (Optional) Other Build Options Overview

Generate Qt adapters and compile (static) Qt adapters library from scratch.

1. Edit the adapter/QObjectClassList.raw according to your needs (add any QObject classes for which Qt adapters should be generated and build for).

2. Generate Qt adapter classes (see `QxyAdapter.h files) and compile (static) library that contains all generated Qt adapters.

    * Build the following solutions (after you have built the `Thekla.dll`!):

        ```
        default-adapter-input-file-generator.sln
        default-adapter-generator.sln
        ```

        results in

        ```
        THEKLAROOT/src/adapter/default-adapter-input-file-generator.exe
        THEKLAROOT/src/adapter/default-adapter-generator.exe
        ```

    * Generate the QtAdapter headers by executing from command-line

        ```
        THEKLAROOT/src/adapter $> ./default-adapter-input-file-generator.exe
        THEKLAROOT/src/adapter $> ./default-adapter-generator.exe
        ```

    * Re-create the Thekla library.

        Re-build the `Thekla.sln` solution.
