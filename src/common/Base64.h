
#ifndef THEKLA_BASE64_H
#define THEKLA_BASE64_H

#include <common/config-win32.h>

#include <QByteArray>

//--------------------------------------------------------------------------------
class THEKLA_EXPORT Base64
{
public:
    static bool isBase64(char in);
    static QByteArray encode(const QByteArray & in);
    static QByteArray decode(const QByteArray & in);
};

#endif // THEKLA_BASE64_H
