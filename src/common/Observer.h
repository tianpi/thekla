
#ifndef THEKLA_OBSERVER_H
#define THEKLA_OBSERVER_H

#include "Variant.h"

class Log;
class Observable;

//--------------------------------------------------------------------------------
class THEKLA_EXPORT Observer : public Variant
{
private:
    static Log cLog;

public:
    Observer()
        : Variant() {};
    Observer(QObject * qObj, const QMetaMethod & qSlot)
        : Variant(qObj, qSlot) { type_ = QOBJECTSLOT; };
    Observer(QObject * qObj, const QMetaProperty & qProperty)
        : Variant(qObj, qProperty) {};
    Observer(SoField * oivField)
        : Variant(oivField) {};
    Observer(SoFieldList * oivFieldList)
        : Variant(oivFieldList) {};
    Observer(const Observer & copy)
        : Variant(copy) {};
    Observer(const Observable & copy);
    virtual ~Observer()
        {};

public:

    bool operator==(const Observer & cmp) const
        { return Variant::operator==(cmp); };

    void getObserver(QObject *& qObj) const
        { getVariant(qObj); };
    void getObserver(QObject *& qObj, QMetaMethod *& qSlot) const
        { getVariant(qObj, qSlot); }
    void getObserver(QObject *& qObj, QMetaProperty *& qProperty) const
        { getVariant(qObj, qProperty); }
    void getObserver(SoField *& oivField) const
        { getVariant(oivField); }
    void getObserver(SoFieldList *& oivFieldList) const
        { getVariant(oivFieldList); }

    bool isConnectable() const;
    bool isConnectable(QVariant::Type qVariantType) const;
    bool isConnectable(const QList<QVariant::Type> & qVariantTypeList) const;
    bool isConnectable(const SoType oivType) const;

};

#endif // THEKLA_OBSERVER_H
