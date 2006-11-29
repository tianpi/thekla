
#ifndef THEKLA_QTOBJECTUTILS_H
#define THEKLA_QTOBJECTUTILS_H

#include <common/config-win32.h>

#include <time.h>

#include <QString>
#include <QMetaMethod>
#include <QMetaProperty>
#include <QStringList>
#include <QTime>

class Log;

//--------------------------------------------------------------------------------
/**
 * @b SERIALIZABLE :=
 *
 * It is possible to create an XML-representation (string) of the entity (signal,
 * property). Basically the entity must be READABLE (see properties).
 *
 * Non-insertable signals must not be part of the ADAPTER (resulted in a compilation
 * error!).
 *
 * Non-insertable signals ARE shown in the PLUGIN. BUT - the BIND checkbox MUST NOT be
 * show, whereas the PROP-TRIGGER checkbox is available for ALL signals (because they are
 * only used as trigger on the input client side - no Xml-representation must be
 * created). Therefore, the question if signals are omitted if they don't fullfill some
 * criteria is answered: NO! ALL SIGNALS ARE CONVERTED TO XML-ELEMENTS. The check
 *
 * @b CONNECTABLE (implies INSERTABLE) :=
 *
 * It is possible to create an object from the XML-representation and pass it savely to
 * the Observer (TEMPORARY - NO HEAP DATA. Thus POINTERS are not allowed). The observer
 * entity must be writable.
 *
 * The result is dependent from the Observable AND the Observer. It is possible to create
 * a Xml-representation from a Pointer, but it is NOT POSSIBLE to PASS a Pointer. ONLY
 * CALL BY VALUE (or REFERENCE) is possible.
 *
 * TODO: What concerns connections from Qt-Signals to Oiv-Fields: that is completely different,
 * because the major criteria for that is the existance of a valid TYPE CONVERSTION (Qt
 * <-> OIV). That is not specified yet.
 */
class QtObjectUtils
{
private:
    static Log cLog;

public:

    static QString normalizeSignature(const QString & signature)
        {
            return QMetaObject::normalizedSignature(signature.toAscii());
        };

    static QString normalizeParamType(const QString & paramType);


    static QString removeSignalPrefix(const QString & signature)
        {
            QString ret(signature);
            if (ret.startsWith(SIGNAL()))
                ret.remove(0, strlen(SIGNAL()));
            return ret;
        };
    static QString removeSlotPrefix(const QString & signature)
        {
            QString ret(signature);
            if (ret.startsWith(SLOT()))
                ret.remove(0, strlen(SLOT()));
            return ret;
        };

    static QByteArray addSignalPrefix(const QString & signature)
        {
            QString ret(signature);
            ret.prepend(SIGNAL());
            return QByteArray(ret.toAscii());
        };

    static QByteArray addSlotPrefix(const QString & signature)
        {
            QString ret(signature);
            ret.prepend(SLOT());
            return QByteArray(ret.toAscii());
        };

    /**
     * Returns true if the TYPES (all parameter types of the method) are (DE)SERIALIZABLE.
     *
     * Returns true if ALL signal parameter types can be serialized and deserialized to
     * ByteArray, thus the types possess stream operators << and >>.
     *
     * @li The major criteria (run-time) is that the type is a valid QVariant wrappee.
     *
     * @li The second criteria is a compilation failure of the statements (stream << type,
     * stream >> type).
     */
    static bool isSerializable(const QMetaMethod & method);
    /**
     * Returns true if property TYPE is (DE)SERIALIZABLE.
     *
     * The same issues as with methods (signals).
     */
    static bool isSerializable(const QMetaProperty & property);

    static bool isConnectable(const QMetaMethod & method);
    static bool isConnectable(const QMetaProperty & property);

    static QStringList getParamTypes(const QString & signature);

    static QByteArray toStream(const QVariant & qVariant);
    static QByteArray toStream(const QList<QVariant> & qVariantList);

    static QList<QVariant> fromStream(const QByteArray & byteStream);

    static QMetaMethod getMetaMethod(const QObject & qObj, const char * signature);

    static QMetaProperty getMetaProperty(const QObject & qObj, const char * name);

    static QString getUniqueIdentifier()
        {
            return QString::number((int)time(NULL)) + QString::number(rand());
        };

private:
    QtObjectUtils();
    ~QtObjectUtils();
};


#endif // THEKLA_QTOBJECTUTILS_H
