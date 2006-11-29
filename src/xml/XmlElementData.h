
#ifndef THEKLA_XMLELEMENTDATA_H
#define THEKLA_XMLELEMENTDATA_H

#include <common/config-win32.h>

#include <QDomElement>
#include <QString>
#include <QMap>

class Log;

//--------------------------------------------------------------------------------
/**
 * Data container for an XML-element. Contains the following entities:
 *
 * @li @b tagname: known from the meta description
 *
 * @li @b attributes: @e names known from the meta description, contains the @e values
 * according to these names
 *
 * @li @b text: contains the element text for those elements which can contain text
 */
class THEKLA_EXPORT XmlElementData
{
public:

    enum ConfigFlag {
        CF_NONE = 0,
        USE_THEKLA,
        GENERATE_QTAPP_CODE,
        BIND,
        USE_AS_TRIGGER,
        UPDATE
    };

private:

    static Log cLog;

private:

    QDomDocument doc_;

    QDomElement data_;

    bool containsText_;

    QDomText text_;

public:

    explicit XmlElementData(bool containsText = false);

    explicit XmlElementData(const QDomElement & domElem);

    XmlElementData(const XmlElementData & copy);

    ~XmlElementData();

private:

    bool isConfigFlag(const QString & attribName) const;

public:

    XmlElementData & operator=(const XmlElementData & data);

    bool operator==(const XmlElementData & cmp);

    bool contains(const XmlElementData & cmp);

    QString getTagName() const { return data_.tagName(); };

    void setTagName(const QString & tagName)
        {
            data_.setTagName(tagName);
        };

    void setAttribute(const QString & name, const QString & value)
        {
            data_.setAttribute(name, value);
        };

    bool hasAttribute(const QString & name) const
        {
            return data_.hasAttribute(name);
        };

    void removeAttribute(const QString & name)
        {
            return data_.removeAttribute(name);
        };

    QString getAttribute(const QString & name) const
        {
            return data_.attribute(name);
        };

    int getAttributeCount() const;

    QString getAttributeName(int index) const;

    QString getAttributeValue(int index) const;

    void setText(const QByteArray & text, bool encodeBase64 = false);

    bool containsText() const { return containsText_; };

    QByteArray getText(bool decodeBase64 = false) const;

    QDomDocument getDom() const;

    QString getXPath() const;

    void setConfigFlag(ConfigFlag flag, bool value);

    bool getConfigFlag(ConfigFlag flag) const;

    void merge(const XmlElementData & data);

    void clear();
};


#endif // THEKLA_XMLELEMENTDATA_H
