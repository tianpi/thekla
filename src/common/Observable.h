
#ifndef THEKLA_OBSERVABLE_H
#define THEKLA_OBSERVABLE_H

#include "Variant.h"

class Log;
class Observer;

//--------------------------------------------------------------------------------
class THEKLA_EXPORT Observable : public Variant
{
public:
    Observable()
        : Variant() {};
    Observable(QObject * qtObj)
        : Variant(qtObj) {};
    Observable(QObject * qtObj, const QMetaMethod & qtSignal)
        : Variant(qtObj, qtSignal) { type_ = QOBJECTSIGNAL; };
    Observable(QObject * qtObj, const QMetaProperty & qtProperty)
        : Variant(qtObj, qtProperty) {};
    Observable(SoField * oivField)
        : Variant(oivField) {};
    Observable(const Observable & copy)
        : Variant(copy) {};
    Observable(const Observer & copy);
    ~Observable()
        {};

public:

    bool operator==(const Observable & cmp) const
        { return Variant::operator==(cmp); };

    void getObservable(QObject *& qObj) const
        { getVariant(qObj); };
    void getObservable(QObject *& qObj, QMetaMethod *& qSignal) const
        { getVariant(qObj, qSignal); };
    void getObservable(QObject *& qObj, QMetaProperty *& qProperty) const
        { getVariant(qObj, qProperty); };
    void getObservable(SoField *& oivField) const
        { getVariant(oivField); };

    bool isSerializable() const;

};

#endif // THEKLA_OBSERVABLE_H
