
#ifndef THEKLA_XMLTRASHDOCUMENT_H
#define THEKLA_XMLTRASHDOCUMENT_H

#include "XmlDocument.h"
#include <QWidget>
#include <QList>

class QtObject;

//--------------------------------------------------------------------------------
class THEKLA_EXPORT XmlTrashDocument : public XmlDocument
{
private:

    static Log cLog;

public:

    XmlTrashDocument();

    ~XmlTrashDocument();

public:

    void push(QtObject * qtObject);

    QtObject* get(QObject * qObject);
};

#endif // THEKLA_XMLTRASHDOCUMENT_H
