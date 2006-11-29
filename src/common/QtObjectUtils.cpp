
#include "QtObjectUtils.h"
#include "Log.h"
#include "Exception.h"

#include <QDataStream>
#include <QVariant>
#include <QByteArray>
#include <QIODevice>

//--------------------------------------------------------------------------------
Log QtObjectUtils::cLog (Log::getClassLog("QtObjectUtils"));

//--------------------------------------------------------------------------------
bool QtObjectUtils::isSerializable(const QMetaMethod & method)
{
    Log log(Log::getMethodLog(cLog, "isSerializable(QMetaMethod)"));

    if (method.methodType() != QMetaMethod::Signal)
        return false;

    QList<QByteArray> paramTypes = method.parameterTypes();
    QList<QByteArray> paramNames = method.parameterNames();

    for (int j = 0; j < paramTypes.size(); j++) {

        QByteArray a = paramTypes.at(j);
        std::string paramType(a.data(), a.size());


        QByteArray type = paramTypes.at(j);
        QByteArray name = paramNames.at(j);
        THEKLA_DEBUG(log, "paramType = [" << type.data() << "] ,"
                    << "paramName = [" << name.data() << "]");

        // Ignore pointers generally
        if (paramType.find('*') != std::string::npos)
            return false;

        // Ignore those which are not recognized by QVariant
        if (QVariant::nameToType(paramType.c_str()) == QVariant::Invalid)
            return false;

    }

    return true;
}

//--------------------------------------------------------------------------------
bool QtObjectUtils::isConnectable(const QMetaMethod & method)
{
    if (method.methodType() != QMetaMethod::Slot)
        return false;

    QList<QByteArray> paramTypes = method.parameterTypes();
    for (int j = 0; j < paramTypes.size(); j++) {
        QByteArray a = paramTypes.at(j);
        std::string paramType(a.data(), a.size());

        // Ignore pointers generally
        if (paramType.find('*') != std::string::npos)
            return false;

    }

    return true;
}

//--------------------------------------------------------------------------------
bool QtObjectUtils::isSerializable(const QMetaProperty & property)
{
    // ignore those which are not even readable (always true for properties?)
    if (!property.isReadable())
        return false;

    // ignore those which are not recognized by QVariant
    if (property.type() == QVariant::Invalid)
        return false;

    return true;
}

//--------------------------------------------------------------------------------
bool QtObjectUtils::isConnectable(const QMetaProperty & property)
{
    QString type(property.typeName());

    // ignore pointers generally
    if (type.contains('*'))
        return false;

    // ignore those you cannot write
    if (!property.isWritable())
        return false;

    return true;

}

//--------------------------------------------------------------------------------
QByteArray QtObjectUtils::toStream(const QVariant & qVariant)
{
    QList<QVariant> qVariantList;
    qVariantList.push_back(qVariant);
    return toStream(qVariantList);
}

//--------------------------------------------------------------------------------
QByteArray QtObjectUtils::toStream(const QList<QVariant> & qVariantList)
{
    QByteArray ret;
    QDataStream s(&ret, QIODevice::WriteOnly);

    for (int i = 0; i < qVariantList.size(); i++) {
        s << qVariantList[i];
    }

    return ret;
}

//--------------------------------------------------------------------------------
QList<QVariant> QtObjectUtils::fromStream(const QByteArray & byteStream)
{
    QDataStream s(&const_cast<QByteArray&>(byteStream), QIODevice::ReadOnly);
    QList<QVariant> qVariantList;
    while (!s.atEnd()) {
        QVariant qVariant;
        s >> qVariant;
        qVariantList.push_back(qVariant);
    }
    return qVariantList;
}



//--------------------------------------------------------------------------------
QStringList QtObjectUtils::getParamTypes(const QString & signature)
{
    QStringList paramTypeList;
    int from = signature.indexOf('(') + 1;
    int to = signature.indexOf(')');

    // empty parameter space
    if (from == to) {
        return paramTypeList;
    }

    QString params = signature.mid(from, to - from);
    paramTypeList = params.split(',');
    return paramTypeList;
}

//--------------------------------------------------------------------------------
QString QtObjectUtils::normalizeParamType(const QString & paramType)
{
    QString ret(paramType);
    ret.remove('&');
    ret.remove('*');
    return ret;
}


//--------------------------------------------------------------------------------
QMetaMethod QtObjectUtils::getMetaMethod(const QObject & qObj, const char * signature)
{
    const QMetaObject * meta = qObj.metaObject();
    int index = meta->indexOfMethod(signature);
    if (index == -1) {
        THROW_THEKLA_EXCEPTION("QObject (className = " << qObj.metaObject()->className() << "): "
                              << "given signature (" << signature << ") invalid.")
    }
    return meta->method(index);
}

//--------------------------------------------------------------------------------
QMetaProperty QtObjectUtils::getMetaProperty(const QObject & qObj, const char * name)
{
    const QMetaObject * meta = qObj.metaObject();
    int index = meta->indexOfProperty(name);
    if (index == -1) {
        THROW_THEKLA_EXCEPTION("QObject (className = " << qObj.metaObject()->className() << "): "
                              << "given property name (" << name << ") invalid.")
    }
    return meta->property(index);
}

