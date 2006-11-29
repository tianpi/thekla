
#ifndef THEKLA_DBROOTELEMENT_H
#define THEKLA_DBROOTELEMENT_H

#include "XmlElement.h"

//--------------------------------------------------------------------------------
class THEKLA_EXPORT DBRootElement : public XmlElement
{
protected:
    DBRootElement(const DBRootElement & copy)
        : XmlElement(copy)
        {};
public:
    DBRootElement()
        : XmlElement(false)
        {};
    virtual ~DBRootElement()
        {};
public:
    virtual XmlElement* clone() const { return new DBRootElement(*this); };
};

#endif // THEKLA_DBROOTELEMENT_H
