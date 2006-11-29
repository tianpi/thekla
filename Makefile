
#### GENERAL RULES

## COMPILE

## NOTE: this has to be the FIRST ('default' doesn't mean anything?!)
default: build

all: adapter thekla plugin test examples

build: adapter thekla plugin

## CLEAN

clean: thekla-clean plugin-clean test-clean examples-clean

## INSTALL

install: build thekla-install plugin-install

uninstall: thekla-uninstall plugin-uninstall

#### PROJECTS

## Adapter

adapter:
	(cd src/adapter && qmake-qt4 default-adapter-lib.pro && make)

adapter-make:
	(cd src/adapter && ./make.sh)

adapter-gen:
	(cd src/adapter && ./generate.sh)

adapter-clean:
	(cd src/adapter && ./clean.sh)

## Library (Thekla)

thekla:
	(cd src && qmake-qt4 lib.pro && make)

thekla-clean:
	(cd src && qmake-qt4 lib.pro && make distclean)
	(cd lib && rm -f libThekla*.so*)

thekla-install:
	(cd src && qmake-qt4 lib.pro && make install)

thekla-uninstall:
	(cd src && qmake-qt4 lib.pro && make uninstall)

## QDesigner Plugin (TheklaPlugin)

plugin:
	(cd src/plugin && qmake-qt4 plugin.pro && make)

plugin-clean:
	(cd src/plugin && qmake-qt4 plugin.pro && make distclean)
	(cd lib && rm -f libTheklaQDPlugin*.so*)

plugin-install:
	(cd src/plugin && qmake-qt4 plugin.pro && make install)

plugin-uninstall:
	(cd src/plugin && qmake-qt4 plugin.pro && make uninstall)

## Tests

test:
	(cd tests/lib && make)
	(cd tests/client && make)
	(cd tests/plugin && make)

test-run:
	(cd tests/lib && ./runner)
	(cd tests/client && ./runner)
	(cd tests/plugin && ./runner)

test-clean:
	(cd tests/lib && make clean)
	(cd tests/client && make clean)
	(cd tests/plugin && make clean)
	(find tests -name "*.log" -exec rm -f "{}" ";")

## Examples

examples: qt4-dialog soqt4-viewer

qt4-dialog:
	(cd examples/qt4-dialog && qmake-qt4 input.pro && make)
	(cd examples/qt4-dialog && qmake-qt4 output.pro && make)

soqt4-viewer:
	(cd examples/soqt4-viewer && qmake-qt4 input.pro && make)
	(cd examples/soqt4-viewer && qmake-qt4 output.pro && make)

examples-clean:
	(cd examples/qt4-dialog && qmake-qt4 input.pro && make distclean)
	(cd examples/qt4-dialog && qmake-qt4 output.pro && make distclean)
	(cd examples/soqt4-viewer && qmake-qt4 input.pro && make distclean)
	(cd examples/soqt4-viewer && qmake-qt4 output.pro && make distclean)
	(find examples -name "*.log" -exec rm -f "{}" ";")

## Documentation

docs:
	(cd doc && doxygen Doxyfile)

docs-clean:
	(rm -rf doc/html)
