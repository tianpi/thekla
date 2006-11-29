
#include "Observer.h"

#include <common/Log.h>
#include <common/QtObjectUtils.h>
#include <common/TypeConverter.h>
#include <common/Observable.h>

#include <Inventor/fields/SoSFTrigger.h>

//--------------------------------------------------------------------------------
Log Observer::cLog(Log::getClassLog("Observer"));

//--------------------------------------------------------------------------------
Observer::Observer(const Observable & copy)
    : Variant(copy)
{

}

//--------------------------------------------------------------------------------
bool Observer::isConnectable() const
{
    switch (type_) {

    case INVALID:
        return false;

    case QOBJECTSLOT:
    {
        QMetaMethod * qSlot = reinterpret_cast<QMetaMethod*>(ptrList_[1]);
        if (!QtObjectUtils::isConnectable(*qSlot))
            return false;
        break;
    }
    case QOBJECTPROPERTY:
    {
        QMetaProperty * qProperty = reinterpret_cast<QMetaProperty*>(ptrList_[1]);
        if (!QtObjectUtils::isConnectable(*qProperty))
            return false;
        break;
    }
    default:
        return true;
    }
    return true;
}

//--------------------------------------------------------------------------------
bool Observer::isConnectable(QVariant::Type qVariantType) const
{
    if (!isConnectable())
        return false;

    switch (type_) {

    case QOBJECTSLOT:
    case SOFIELDLIST:
    {
        // call isConnectable(QList)
        QList<QVariant::Type> qVariantTypeList;
        qVariantTypeList.push_back(qVariantType);
        if (!isConnectable(qVariantTypeList))
            return false;
        break;
    }
    case QOBJECTPROPERTY:
    {
        // check type compatability
        QMetaProperty * qProperty = reinterpret_cast<QMetaProperty*>(ptrList_[1]);
        QVariant v(qProperty->type());
        if (!v.canConvert(qVariantType))
            return false;
        break;
    }
    case SOFIELD:
    {
        // check type equivalence
        SoField * oivField = reinterpret_cast<SoField*>(ptrList_[0]);
        if (oivField->getTypeId() != TypeConverter::findType(qVariantType))
            return false;
        break;
    }
    default:
        return false;
    }

    return true;
}

//--------------------------------------------------------------------------------
bool Observer::isConnectable(const QList<QVariant::Type> & qVariantTypeList) const
{
    Log log(Log::getMethodLog(cLog, "isConnectable(QList<QVariant>)"));
    if (!isConnectable())
        return false;

    THEKLA_DEBUG(log, "** START: qVariantTypeList.size() = " << qVariantTypeList.size());

    switch (type_) {

    case INVALID:
    case QOBJECT:
        return false;

    case QOBJECTSLOT:
    {
        QMetaMethod * qSlot = reinterpret_cast<QMetaMethod*>(ptrList_[1]);
        THEKLA_DEBUG(log, "-- QOBJECTSLOT: signature = " << qSlot->signature());

        QList<QByteArray> paramTypeList = qSlot->parameterTypes();
        THEKLA_DEBUG(log, "-- START: paramTypeList.size() = " << paramTypeList.size());

        // compare parameter count
        if (paramTypeList.size() != qVariantTypeList.size())
            return false;

        // compare parameter types
        for (int i = 0; i < qVariantTypeList.size(); i++) {

            QByteArray paramType = paramTypeList[i];
            THEKLA_DEBUG(log, "paramType = " << paramType.data());
            QVariant::Type type = QVariant::nameToType(paramType);
            THEKLA_DEBUG(log, "QVariant::nameToType() = " << type);

            if (type != qVariantTypeList[i])
                return false;
        }
        break;
    }
    case SOFIELDLIST:
    {
        THEKLA_DEBUG(log, "-- SOFIELDLIST");
        SoFieldList * oivFieldList = reinterpret_cast<SoFieldList*>(ptrList_[0]);
        THEKLA_DEBUG(log, "oivFieldList.getLength() = " << oivFieldList->getLength()
                    << ", qVariantTypeList.size() = " << qVariantTypeList.size());

        // Special case: Signal with no parameters -> SoSFTrigger
        if (qVariantTypeList.size() == 0) {

            if (oivFieldList->getLength() != 1)
                return false;
            if (oivFieldList->get(0)->getTypeId() != SoSFTrigger::getClassTypeId())
                return false;

        }
        // Normal case: Each Signal type == SoType
        else {

            if (qVariantTypeList.size() != oivFieldList->getLength())
                return false;

            for (int i = 0; i < qVariantTypeList.size(); i++) {
                SoField * oivField = oivFieldList->operator[](i);
                THEKLA_DEBUG(log, "qVariantTypeList[i] = " << qVariantTypeList[i]
                            << ", findType() = " << TypeConverter::findType(oivField->getTypeId()));
                if (TypeConverter::findType(oivField->getTypeId()) != qVariantTypeList[i])
                    return false;
            }
        }
        break;
    }
    case QOBJECTPROPERTY:
    case SOFIELD:
    {
        return false;
    }

    }

    THEKLA_DEBUG(log, "** END.");

    return true;
}

//--------------------------------------------------------------------------------
bool Observer::isConnectable(const SoType oivFieldType) const
{
    if (!isConnectable())
        return false;

    switch (type_) {

    case INVALID:
    case QOBJECT:
    case SOFIELDLIST:
    case QOBJECTSLOT:
    {
        return false;
    }
    case QOBJECTPROPERTY:
    {
        QMetaProperty * qProperty = reinterpret_cast<QMetaProperty*>(ptrList_[1]);

        // Special case: any field type can be connected to String
        if (qProperty->type() == QVariant::String && oivFieldType != TypeConverter::findType(QVariant::String)) {
            // do nothing
        }
        // Normal case: equivalent types
        else {
            if (TypeConverter::findType(qProperty->type()) != oivFieldType)
                return false;
        }
        break;
    }
    case SOFIELD:
    {
        SoField * oivField = reinterpret_cast<SoField*>(ptrList_[0]);
        if (oivField->getTypeId() != oivFieldType)
            return false;
        break;
    }
    }

    return true;
}

