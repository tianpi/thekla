
#ifndef THEKLA_DEFAULTADAPTERGENERATOR_H
#define THEKLA_DEFAULTADAPTERGENERATOR_H

#include "QtAdapterFactory.h"

#include <QMap>
#include <QString>
#include <QMetaObject>

#define QOBJECTCLASSLIST_INCLUDEFILE "QObjectClassList.h"
#define QOBJECTCLASSLIST_MACROFILE   "QObjectClassList.cpp"

class Log;

//--------------------------------------------------------------------------------
/**
 * At compile time (!) depends on the following input files:
 *
 * @li QObject Classes HEADER file: Contains entries of the following form
 *
 * @verbatim
 #include <QObject>@endverbatim
 *
 * @li QObject Classes MACRO file: Contains entries of the following form
 *
 * @verbatim
 CREATE_QMETAOBJECT(QObject)@endverbatim
 */
class DefaultAdapterGenerator
{
private:
    /**
     * Maps QObject class names to corresponding QMetaObject objects.
     */
    typedef QMap<QString, const QMetaObject *> QClassNameToMetaObjMap;

private:
    static Log cLog;

private:
    /**
     * Adapter target file prefix (some directory).
     */
    QString adapterClassesTargetPath_;
    QString qtAdapterClassListFilePath_;
    QString qtAdapterIncludeFilePath_;
    QString qtAdapterMacroFilePath_;
    /**
     * Contains not-yet-processed meta objects of QObject classes.
     */
    QClassNameToMetaObjMap input_;
    /**
     * Contains already-processed meta objects of QObject classes.
     */
    QClassNameToMetaObjMap output_;

public:
    DefaultAdapterGenerator(QString adapterClassesTargetPath = "",
                            QString qtAdapterClassListFilePath = QTADAPTERCLASSLIST_QMAKEPRJFILE,
                            QString qtAdapterIncludeFilePath = QTADAPTERCLASSLIST_INCLUDEFILE,
                            QString qtAdapterMacroFilePath = QTADAPTERCLASSLIST_MACROFILE)
        : adapterClassesTargetPath_(((adapterClassesTargetPath == "") ? "" : (adapterClassesTargetPath + "/"))),
          qtAdapterClassListFilePath_(adapterClassesTargetPath_ + qtAdapterClassListFilePath),
          qtAdapterIncludeFilePath_(adapterClassesTargetPath_ + qtAdapterIncludeFilePath),
          qtAdapterMacroFilePath_(adapterClassesTargetPath_ + qtAdapterMacroFilePath),
          input_(),
          output_()
        {};
    ~DefaultAdapterGenerator() {};

private:
    /**
     * Fills the input map with QMetaObject instances derived from QObject objects which
     * have been created with the help of the MACRO file (pre-condition: the QObject class
     * headers must have been included beforehand).
     */
    void init();
    /**
     * Bases on an input map which contains all NON-ABSTRACT (thus all LEAF classes :=
     * classes which does not have any parent class).
     *
     * Adds QMetaObject instances of all ABSTRACT classes which are derived from the meta
     * objects of their parent class meta objects.
     */
    void enqueueAbstractClasses();
    /**
     * Iterates over the list containing ALL QObject class meta objects according to the
     * object hierarchy: starts with root class QObject and subsequently processes Classes
     * whose superclasses have already been processed. Moves processed classes from the
     * input to the output map.
     */
    void generateAdapters();
    /**
     * Generates the adapter source code and stores the header file (using the given path
     * prefix).
     */
    void generateAndSaveAdapter(const QMetaObject * qMetaObj);
    /**
     * Generates the auxiliary files for the AdapterFactory and the qmake project include
     * file.
     */
    void generateAdapterClassListFiles();

public:
    /**
     * Starts the workflow.
     */
    void exec();
};

#endif // THEKLA_DEFAULTADAPTERGENERATOR_H
