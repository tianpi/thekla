
#ifndef THEKLA_VARIANT_H
#define THEKLA_VARIANT_H

#include <common/config-win32.h>

#include <QObject>
#include <QMetaObject>
#include <QMetaMethod>
#include <QMetaProperty>

#include <vector>

#include <Inventor/fields/SoField.h>
#include <Inventor/lists/SoFieldList.h>

class Log;

//--------------------------------------------------------------------------------
class THEKLA_EXPORT Variant
{
private:
    static Log cLog;

public:
    enum Type
    {
        INVALID = 0,

        QOBJECT,

        QOBJECTMETHOD,
        QOBJECTSLOT = QOBJECTMETHOD,
        QOBJECTSIGNAL = QOBJECTMETHOD,

        QOBJECTPROPERTY,

        SOFIELD,
        SOFIELDLIST
    };

protected:

    Type type_;
    QList<void *> ptrList_;

protected:

    Variant()
        : type_(INVALID),
          ptrList_()
        {};
    Variant(QObject * qtObj)
        : type_(QOBJECT),
          ptrList_()
        {
            ptrList_.push_back(qtObj);
        };
    Variant(QObject * qObj, const QMetaMethod & qMethod)
        : type_(QOBJECTMETHOD),
          ptrList_()
        {
            ptrList_.push_back(qObj);
            ptrList_.push_back(new QMetaMethod(qMethod));
        };
    Variant(QObject * qObj, const QMetaProperty & qProperty)
        : type_(QOBJECTPROPERTY),
          ptrList_()
        {
            ptrList_.push_back(qObj);
            ptrList_.push_back(new QMetaProperty(qProperty));
        };
    Variant(SoField * oivField)
        : type_(SOFIELD),
          ptrList_()
        {
            ptrList_.push_back(oivField);
        };
    Variant(SoFieldList * oivFieldList)
        : type_(SOFIELDLIST),
          ptrList_()
        {
            ptrList_.push_back(oivFieldList);
        };
    Variant(const Variant & copy)
        : type_(copy.type_),
          ptrList_(copy.ptrList_)
        {
        };

public:

    virtual ~Variant();

protected:

    void getVariant(QObject *& qObj) const;
    void getVariant(QObject *& qObj, QMetaMethod *& qMethod) const;
    void getVariant(QObject *& qObj, QMetaProperty *& qProperty) const;
    void getVariant(SoField *& oivField) const;
    void getVariant(SoFieldList *& oivFieldList) const;

public:

    bool operator==(const Variant & cmp) const;

    bool contains(const Variant & cmp) const;

    Type type() const { return type_; };

    bool isNull() const;
};

#endif // THEKLA_VARIANT_H
