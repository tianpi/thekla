
#ifndef THEKLA_QTADAPTERUTILS_H
#define THEKLA_QTADAPTERUTILS_H

#include <QString>
#include <QMetaMethod>

//--------------------------------------------------------------------------------
class QtAdapterUtils
{

public:

    static QString getAdapterSuperBaseClassName()
        {
            return "QtAdapter";
        };

    static QString getAdapterOwnerClassName()
        {
            return "QtObject";
        };

    static QString getAdapterClassName(const QMetaObject * qMetaObj)
        {
            return QString(qMetaObj->className()) + "Adapter";
        };

    static QString getAdapterBaseClassName(const QMetaObject * qMetaObj)
        {
            const QMetaObject * qMetaObjSuper = qMetaObj->superClass();
            if (qMetaObjSuper == NULL)
                return getAdapterSuperBaseClassName();
            return getAdapterClassName(qMetaObjSuper);
        };
    static QString getAdapterHeaderFileName(const QString & qtAdapterClassName)
        {
            return qtAdapterClassName + ".h";
        };

    static QString getSignalName(const QString & signature)
        {
            QString name = signature.left(signature.indexOf('('));
            return name + "Signal";
        };

    static QString getSignalName(const QMetaMethod & method)
        {
            QString sig(method.signature());
            return getSignalName(sig);
        };

    static QString getSlotName(const QString & signature)
        {
            QString name = signature.left(signature.indexOf('('));
            return name + "Slot";
        };

    static QString getSlotName(const QMetaMethod & method)
        {
            QString sig(method.signature());
            return getSlotName(sig);
        };

    static QString getSignalSignature(const QString & signature)
        {
            QString ret = getSignalName(signature);
            ret += signature.right(signature.length() - signature.indexOf('('));
            return ret;
        };

    static QString getSlotSignature(const QString & signature)
        {
            QString ret = getSlotName(signature);
            ret += signature.right(signature.length() - signature.indexOf('('));
            return ret;
        };

private:
    QtAdapterUtils();
    ~QtAdapterUtils();
};

#endif // THEKLA_QTADAPTERUTILS_H
