
## import SETTINGS

include $(THEKLAROOT)/tests/Makefile.common

CXXFLAGS += -DQT_PLUGIN -DQDESIGNER_EXPORT_WIDGETS -I/usr/include/qt4/QtDesigner

LIBS += -lTheklaQDPlugin

### SOUCES (TESTS)

TESTSUITES = \
	../TestInit.h \
	TestUiFileObserver.h \
	TestFormManager.h \
	TestFormWindowManager.h \
	TestQtObjectSettingsDialog.h \
	TestGlobalSettingsDialog.h

## import RULES

include $(THEKLAROOT)/tests/Makefile.rules
