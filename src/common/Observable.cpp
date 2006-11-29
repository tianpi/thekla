
#include "Observable.h"
#include <common/Observer.h>
#include <common/QtObjectUtils.h>


//--------------------------------------------------------------------------------
Observable::Observable(const Observer & copy)
    : Variant(copy)
{

}

//--------------------------------------------------------------------------------
bool Observable::isSerializable() const
{
    switch(type_)
    {
    case INVALID:
    case SOFIELDLIST:
        return false;

    case QOBJECT:
    case SOFIELD:
        // NOTE: calm down, I checked this - it's ok ;-)
        return true;

    case QOBJECTSIGNAL:
    {
        QMetaMethod * qSignal = reinterpret_cast<QMetaMethod*>(ptrList_[1]);
        if (!QtObjectUtils::isSerializable(*qSignal))
            return false;
        break;
    }
    case QOBJECTPROPERTY:
    {
        QMetaProperty * qProp = reinterpret_cast<QMetaProperty*>(ptrList_[1]);
        if (!QtObjectUtils::isSerializable(*qProp))
            return false;
        break;
    }
    };

    return true;
}

