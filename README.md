# Thekla - The UI Connection Spider

## Getting started

[Installation on Linux](INSTALL-Linux.md)

[Installation on Windows](INSTALL-Windows.md)

## Dependencies

- [Muddleware](http://studierstube.icg.tu-graz.ac.at/handheld_ar/muddleware/)

- [TinyXMLMod](https://svn.icg.tu-graz.ac.at/svn/TinyXMLMod/)

- [ACE](http://www.cs.wustl.edu/~schmidt/ACE.html)

- [TProactor](http://www.terabit.com.au/solutions.php)

- [WxWidgets](http://www.wxwidgets.org/)

- [Qt](http://www.trolltech.com/products/qt ) features a dual-licensing model. For Linux and Windows/Cygwin versions under the GNU GPL are available. The Windows/MSVC version is commercial.

- [Coin 2](http://www.coin3d.org/lib/coin)

- [log4cplus](http://log4cplus.sourceforge.net/) is available in `extras/log4cplus`.

- (optional) [CxxTest](http://cxxtest.sourceforge.net/) in available in  `extras/cxxtest`.

- (optional) [SoQt](http://www.coin3d.org/lib/soqt)


## Muddleware Database Preparation[](#muddleware_database_preparation)

The Muddleware database contents must be customized in order to use it with Thekla. The startup content is specified by the file `test.xml` (name can be changed within the Muddleware config file) located in the `exec/data` directory relative from the Muddleware root.

Per default, Thekla puts the UI data elements as children of the XML element `THEKLA`. Thus the `MUDDLEWAREROOT/exec/data/test.xml` contents must be edited to the following:

```xml
<?xml version="1.0" standalone="no" ?>
<!-- Using Thekla -->
<THEKLA>
    <!-- TODO -->
</THEKLA>
```

It is, however, possible to use a different XML element as shell for the Thekla UI data. The XML element is identified by some XPath prefix which can be entered in the global settings dialog of the Qt Plugin (the default UI data XPath prefix is `/THEKLA`) and the Thekla Input- and OutputClient API.

## Documentation

Generate HTML documentation with doxygen by executing:

```
doxygen Doxyfile
```

The HTML documentation will be located in `doc/html`.
