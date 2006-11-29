
#include "DefaultAdapterGenerator.h"

#include <codegen/QtAdapterGenerator.h>
#include <codegen/CodeGenerator.h>
#include <common/QtAdapterUtils.h>
#include <common/Log.h>
#include <common/Exception.h>

#include <QObject>
#include <QFile>


//--------------------------------------------------------------------------------
Log DefaultAdapterGenerator::cLog(Log::getClassLog("DefaultAdapterGenerator"));

// Contains the QObject classes include statements
#include QOBJECTCLASSLIST_INCLUDEFILE

//--------------------------------------------------------------------------------
void DefaultAdapterGenerator::init()
{
    Log log(Log::getMethodLog(cLog, "init()"));
    THEKLA_DEBUG(log, "-- START.");

    THEKLA_INFO(coutLog, "Reading input file: " << QOBJECTCLASSLIST_INCLUDEFILE);
    THEKLA_INFO(coutLog, "Reading input file: " << QOBJECTCLASSLIST_MACROFILE);

    // create meta objects of QObject classes
#define CREATE_QMETAOBJECT(ClassName)                 \
    {                                                 \
        QObject * obj = new ClassName();              \
        const QMetaObject * meta = obj->metaObject(); \
        input_.insert(meta->className(), meta);       \
    }
#include QOBJECTCLASSLIST_MACROFILE
#undef CREATE_QMETAOBJECT

    THEKLA_DEBUG(log, "-- END. input_.count() = " << input_.count());
}

//--------------------------------------------------------------------------------
void DefaultAdapterGenerator::enqueueAbstractClasses()
{
    Log log(Log::getMethodLog(cLog, "enqueueAbstractClasses()"));
    THEKLA_DEBUG(log, "-- START. input_.count() = " << input_.count());

    const QMetaObject * meta = NULL;

    ////////// Add meta objects of abstract QObject classes

    QClassNameToMetaObjMap::ConstIterator it = input_.begin();
    for ( ; it != input_.end(); ) {

        meta = (*it);
        const QMetaObject * super = meta->superClass();

        // check superclass meta object of contained class
        if (super == NULL || input_.contains(super->className())) {
            it++;
            continue;
        }

        // superclass is not a member of list -> abstract class: add meta object
        THEKLA_DEBUG(log, "Adding item. className = " << super->className());
        input_.insert(super->className(), super);
        it = input_.begin();
    }

    THEKLA_DEBUG(log, "-- END. input_.count() = " << input_.count());
}

//--------------------------------------------------------------------------------
void DefaultAdapterGenerator::generateAdapters()
{
    Log log(Log::getMethodLog(cLog, "generateAdapters()"));
    THEKLA_DEBUG(log, "-- START.");

    const QMetaObject * meta = NULL;

    // root class QObject MUST be contained in list
    if (!input_.contains("QObject"))
    {
        THROW_THEKLA_EXCEPTION("Given QObject classlist does not contain QObject.");
    }
    // process the root class "QObject"
    meta = input_.value("QObject");

    // (!) Generate QObject Adapter
    generateAndSaveAdapter(meta);

    THEKLA_DEBUG(log, "Before processing items."
                << " input_.count() = " << input_.count()
                << " output_.count() = " << output_.count());

    ////////// process rest of meta object list

    QClassNameToMetaObjMap::ConstIterator it = input_.begin();
    for ( ; input_.count() != 0; ) {

        meta = (*it);

        // pre-condition: superclass was already processed
        const QMetaObject * super = meta->superClass();
        if (output_.contains(super->className())) {

            // (!) Generate Adapter Class
            generateAndSaveAdapter(meta);

            it = input_.begin();
            continue;
        }

        it++;
    }

    THEKLA_DEBUG(log, "After processing items."
                << " input_.count() = " << input_.count()
                << " output_.count() = " << output_.count());
    THEKLA_DEBUG(log, "-- END.");
}

//--------------------------------------------------------------------------------
void DefaultAdapterGenerator::generateAndSaveAdapter(const QMetaObject * qMetaObj)
{
    THEKLA_INFO(coutLog, "++ Generating QtAdapter for QObject class: " << qMetaObj->className());

    // generate source code
    QtAdapterGenerator adapterGenerator(qMetaObj);
    adapterGenerator.exec();

    // construct target file path
    QString adapterClassname = adapterGenerator.getClassName();
    QString adapterFilePath;
    adapterFilePath += adapterClassesTargetPath_;
    adapterFilePath += QtAdapterUtils::getAdapterHeaderFileName(adapterClassname);

    // save adapter as file
    CodeGenerator::saveToFile(adapterFilePath, adapterGenerator.getCode());
    THEKLA_INFO(coutLog, "Generated QtAdapter: " << adapterFilePath.toStdString() << " .. OK");

    // enqueue in output list, dequeue from input list
    output_.insert(qMetaObj->className(), qMetaObj);
    input_.remove(qMetaObj->className());
}

//--------------------------------------------------------------------------------
void DefaultAdapterGenerator::generateAdapterClassListFiles()
{
    QList<QString> qObjectClassList = output_.keys();

    QStringList include, macro, qmake;

    qmake += "HEADERS += \\";

    int adapterCount = qObjectClassList.size();
    for (int i=0; i < adapterCount; i++)
    {
        QString qtObjClassName = qObjectClassList.value(i);
        QString adapterClassName = QtAdapterUtils::getAdapterClassName(output_.value(qtObjClassName));
        QString adapterHeaderFileName = QtAdapterUtils::getAdapterHeaderFileName(adapterClassName);

        include << "#include \"" + adapterHeaderFileName + "\"";
        macro << "CREATE_QTADAPTERFACTORY_ENTRY(" + qtObjClassName + "," + adapterClassName + "::create)";
        qmake << "\t" + adapterHeaderFileName + (((i+1) < adapterCount) ? " \\" : "");
    }

    include << "";
    macro << "";
    qmake << "";

    THEKLA_INFO(coutLog, "++ Generating QtAdapter include file: " << qtAdapterIncludeFilePath_.toStdString());
    CodeGenerator::saveToFile(qtAdapterIncludeFilePath_, include.join("\n"));
    THEKLA_INFO(coutLog, "++ Generating QtAdapter macro file: " << qtAdapterIncludeFilePath_.toStdString());
    CodeGenerator::saveToFile(qtAdapterMacroFilePath_, macro.join("\n"));
    THEKLA_INFO(coutLog, "++ Generating QtAdapter qmake project file: " << qtAdapterIncludeFilePath_.toStdString());
    CodeGenerator::saveToFile(qtAdapterClassListFilePath_, qmake.join("\n"));
}

//--------------------------------------------------------------------------------
void DefaultAdapterGenerator::exec()
{
    init();
    THEKLA_INFO(coutLog, "Input QObject class count (from input files): " << input_.count());
    enqueueAbstractClasses();
    THEKLA_INFO(coutLog, "Input QObject class count (+ added nested classes): " << input_.count());

    generateAdapters();
    THEKLA_INFO(coutLog, "--> Generated QtAdapter count : " << output_.count());
    generateAdapterClassListFiles();
}

