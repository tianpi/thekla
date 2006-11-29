
#ifndef THEKLA_XMLELEMENTINFO_H
#define THEKLA_XMLELEMENTINFO_H

#include <common/Observable.h>

#include <QString>

class XmlFactory;
class XmlElement;
class Observable;

//--------------------------------------------------------------------------------
class XmlElementInfo
{
private:

    friend class XmlFactory;

private:

    QString tagName_;

    XmlElement * impl_;

    XmlElementInfo * parentInfo_;

protected:

    XmlElementInfo(const QString & tagName, XmlElement * impl, XmlElementInfo * parentInfo = NULL);

    ~XmlElementInfo();

private:

    XmlElementInfo();

    XmlElementInfo(const XmlElementInfo & copy);

    XmlElementInfo operator=(const XmlElementInfo & copy);

public:

    XmlElementInfo * setParentInfo(XmlElementInfo * parentInfo);

    void setImpl(XmlElement * impl);

    inline QString getTagName() const { return tagName_; };

    inline XmlElement * getImpl() const { return impl_; };

    inline XmlElementInfo * getParentInfo() const { return parentInfo_; };

    bool operator==(const QString & tagName) const;

    bool operator==(const Observable::Type & obsType) const;

    bool operator==(const XmlElementInfo &cmp) const;

};

#endif // THEKLA_XMLELEMENTINFO_H
