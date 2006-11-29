
#ifndef THEKLA_TYPECONVERTER_H
#define THEKLA_TYPECONVERTER_H

#include <Inventor/fields/SoField.h>
#include <Inventor/SoType.h>
#include <QMetaType>
#include <QVariant>
#include <QList>

class OivToQtTypeMapping;

//--------------------------------------------------------------------------------
class TypeConverter
{
private:

private:
    static bool isInitialized_;
    static QList<OivToQtTypeMapping*> oivToQtTypeMappingList_;

private:

    static void registerMapping(const SoType oivFieldType, QMetaType::Type qMetaType);

    static QMetaType::Type findTypeAux(const SoType oivFieldType);


public:
    static void init();

    static SoType getTypeFromName(const QString & typeName);

    static SoType findType(QVariant::Type qVariantType);

    static QVariant::Type findType(const SoType oivFieldType);

    static SoField* getInstance(const SoType oivFieldType);

    static SoField& convert(const QVariant & qVariant, SoField & oivField);

    static QVariant& convert(const SoField & oivField, QVariant & qVariant);
};

#endif // THEKLA_TYPECONVERTER_H
