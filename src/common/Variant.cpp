
#include "Variant.h"
#include "QtObjectUtils.h"
#include "TypeConverter.h"
#include "Log.h"

//--------------------------------------------------------------------------------
Log Variant::cLog(Log::getClassLog("Variant"));

//--------------------------------------------------------------------------------
Variant::~Variant()
{
    switch(type_) {

    case INVALID:
    case QOBJECT:
    case SOFIELD:
    case SOFIELDLIST:
        break;

    case QOBJECTMETHOD:
    {
        QObject * qObj;
        QMetaMethod * qMethod;
        getVariant(qObj, qMethod);
//        delete qMethod;
        break;
    }
    case QOBJECTPROPERTY:
    {
        QObject * qObj;
        QMetaProperty *qProperty;
        getVariant(qObj, qProperty);
//        delete qProperty;
        break;
    }
    }
}

//--------------------------------------------------------------------------------
bool Variant::operator==(const Variant & cmp) const
{
    if (type_ != cmp.type_)
        return false;
    if (ptrList_.size() != cmp.ptrList_.size())
        return false;
    if (isNull() || cmp.isNull())
        return false;

    switch(type_) {

    case QOBJECT:
    case SOFIELD:
    case SOFIELDLIST:
        if (ptrList_[0] != (cmp.ptrList_[0]))
            return false;
        break;

    case QOBJECTMETHOD:
    {
        QObject * qObj, *qObjCmp;
        QMetaMethod *qMethod, *qMethodCmp;
        getVariant(qObj, qMethod);
        cmp.getVariant(qObjCmp, qMethodCmp);
        if (qObj != qObjCmp)
            return false;
        if (QString(qMethod->signature()) != QString(qMethodCmp->signature()))
            return false;

        break;
    }
    case QOBJECTPROPERTY:
    {
        QObject * qObj, *qObjCmp;
        QMetaProperty *qProperty, *qPropertyCmp;
        getVariant(qObj, qProperty);
        cmp.getVariant(qObjCmp, qPropertyCmp);
        if (qObj != qObjCmp)
            return false;
        if (QString(qProperty->name()) != QString(qPropertyCmp->name()))
            return false;
        if (QString(qProperty->typeName()) != QString(qPropertyCmp->typeName()))
            return false;
        break;
    }
    default:
        return false;
    }

    return true;
}

//--------------------------------------------------------------------------------
// TODO: unit test
bool Variant::contains(const Variant & cmp) const
{
    if (operator==(cmp))
        return true;

    if (ptrList_.size() != cmp.ptrList_.size())
        return false;
    if (isNull() || cmp.isNull())
        return false;

    switch(type_) {

    case QOBJECT:
    {
        switch (cmp.type())
        {
        case QOBJECTPROPERTY:
        case QOBJECTMETHOD:
            if (ptrList_[0] != (cmp.ptrList_[0]))
                return false;
            break;
        default:
            return false;
        };
    }

    // TODO: SOFIELD, SOFIELDLIST

    default:
        return false;

    };

    return true;
}


//--------------------------------------------------------------------------------
void Variant::getVariant(QObject *& qtObj) const
{
    if (type_ != QOBJECT) { qtObj = NULL; return; }
    qtObj = reinterpret_cast<QObject *>(ptrList_[0]);
}

//--------------------------------------------------------------------------------
void Variant::getVariant(QObject *& qObj, QMetaMethod *& qMethod) const
{
    if (type_ != QOBJECTMETHOD) { qObj = NULL; qMethod = NULL; return; }
    qObj = reinterpret_cast<QObject*>(ptrList_[0]);
    qMethod = reinterpret_cast<QMetaMethod*>(ptrList_[1]);
}


//--------------------------------------------------------------------------------
void Variant::getVariant(QObject *& qObj, QMetaProperty *& qProperty) const
{
    if (type_ != QOBJECTPROPERTY) { qObj = NULL; qProperty = NULL; return; }
    qObj = reinterpret_cast<QObject*>(ptrList_[0]);
    qProperty = reinterpret_cast<QMetaProperty*>(ptrList_[1]);

}

//--------------------------------------------------------------------------------
void Variant::getVariant(SoField *& oivField) const
{
    if (type_ != SOFIELD) { oivField = NULL; return; }
    oivField = reinterpret_cast<SoField*>(ptrList_[0]);
}

//--------------------------------------------------------------------------------
void Variant::getVariant(SoFieldList *& oivFieldList) const
{
    if (type_ != SOFIELDLIST) { oivFieldList = NULL; return; }
    oivFieldList = reinterpret_cast<SoFieldList*>(ptrList_[0]);
}


//--------------------------------------------------------------------------------
bool Variant::isNull() const
{
    for (int i = 0; i < ptrList_.size(); i++) {
        if (ptrList_[i] == NULL)
            return true;
    }
    return false;
}

