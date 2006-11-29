
#include "TypeConverter.h"
#include <QTextStream>
#include <Inventor/SoDB.h>

#include <Inventor/fields/SoFields.h>

//================================================================================

//--------------------------------------------------------------------------------
class OivToQtTypeMapping
{
public:
    SoType oivFieldType_;
    QMetaType::Type qMetaType_;
private:
    OivToQtTypeMapping();
public:
    OivToQtTypeMapping(const SoType oivFieldType, QMetaType::Type qMetaType)
        : oivFieldType_(oivFieldType),
          qMetaType_(qMetaType)
        {};
    ~OivToQtTypeMapping()
        {};
};


//================================================================================

//--------------------------------------------------------------------------------
bool TypeConverter::isInitialized_ = false;
QList<OivToQtTypeMapping*> TypeConverter::oivToQtTypeMappingList_ = QList<OivToQtTypeMapping*>();

//--------------------------------------------------------------------------------
void TypeConverter::init()
{
    if (isInitialized_)
        return;

    if (!SoDB::isInitialized())
        SoDB::init();

    registerMapping(SoSFTrigger::getClassTypeId(), QMetaType::Void);
    registerMapping(SoSFBool::getClassTypeId(), QMetaType::Bool);
    registerMapping(SoSFFloat::getClassTypeId(), QMetaType::Float);
    registerMapping(SoSFInt32::getClassTypeId(), QMetaType::Int);
    registerMapping(SoSFShort::getClassTypeId(), QMetaType::Short);
    registerMapping(SoMFString::getClassTypeId(), QMetaType::QString);
    registerMapping(SoSFUInt32::getClassTypeId(), QMetaType::UInt);
    registerMapping(SoSFUShort::getClassTypeId(), QMetaType::UShort);

    isInitialized_ = true;
}

//--------------------------------------------------------------------------------
void TypeConverter::registerMapping(const SoType oivFieldType, QMetaType::Type qMetaType)
{
    OivToQtTypeMapping * mapping = new OivToQtTypeMapping(oivFieldType, qMetaType);
    oivToQtTypeMappingList_.push_back(mapping);
}

//--------------------------------------------------------------------------------
SoType TypeConverter::findType(QVariant::Type qVariantType)
{
    for (int i = 0; i < oivToQtTypeMappingList_.size(); i++) {
        OivToQtTypeMapping * mapping = oivToQtTypeMappingList_[i];
        if ((unsigned int)mapping->qMetaType_ == (unsigned int)qVariantType)
            return mapping->oivFieldType_;
    }
    return SoType::badType();
}

//--------------------------------------------------------------------------------
QMetaType::Type TypeConverter::findTypeAux(const SoType oivFieldType)
{
    for (int i = 0; i < oivToQtTypeMappingList_.size(); i++) {
        OivToQtTypeMapping * mapping = oivToQtTypeMappingList_[i];
        if (mapping->oivFieldType_ == oivFieldType) {
            return mapping->qMetaType_;
        }
    }
    return QMetaType::Void;

}

//--------------------------------------------------------------------------------
SoField* TypeConverter::getInstance(const SoType oivFieldType)
{
    if (!oivFieldType.canCreateInstance())
        return NULL;
    return reinterpret_cast<SoField*>((oivFieldType.getInstantiationMethod())());
}

//--------------------------------------------------------------------------------
SoField& TypeConverter::convert(const QVariant & qVariant, SoField & oivField)
{
    SoType oivFieldType = findType(qVariant.type());

    if (oivFieldType == SoType::badType() || oivField.getTypeId() != oivFieldType) {
    }
    else if (oivFieldType == SoSFTrigger::getClassTypeId()) {
        SoSFTrigger & ret = static_cast<SoSFTrigger&>(oivField);
        ret.touch();
    }
    else if (oivFieldType == SoSFBool::getClassTypeId()) {
        SoSFBool & ret = static_cast<SoSFBool&>(oivField);
        ret.setValue(qVariant.value<bool>());
    }
    else if (oivFieldType == SoSFFloat::getClassTypeId()) {
        SoSFFloat & ret = static_cast<SoSFFloat&>(oivField);
        ret.setValue(qVariant.value<float>());
    }
    else if (oivFieldType == SoSFInt32::getClassTypeId()) {
        SoSFInt32 & ret = static_cast<SoSFInt32&>(oivField);
        ret.setValue(qVariant.value<int>());
    }
    else if (oivFieldType == SoSFShort::getClassTypeId()) {
        SoSFShort & ret = static_cast<SoSFShort&>(oivField);
        ret.setValue(qVariant.value<short>());
    }
    else if (oivFieldType == SoMFString::getClassTypeId()) {
        SoMFString & ret = static_cast<SoMFString&>(oivField);
        ret.setValue(qVariant.value<QString>().toStdString().c_str());
    }
    else if (oivFieldType == SoSFUInt32::getClassTypeId()) {
        SoSFUInt32 & ret = static_cast<SoSFUInt32&>(oivField);
        ret.setValue(qVariant.value<unsigned int>());
    }
    else if (oivFieldType == SoSFUShort::getClassTypeId()) {
        SoSFUShort & ret = static_cast<SoSFUShort&>(oivField);
        ret.setValue(qVariant.value<unsigned short>());
    }

    return oivField;
}

//--------------------------------------------------------------------------------
QVariant& TypeConverter::convert(const SoField & oivFieldIn, QVariant & qVariant)
{
    QMetaType::Type qMetaType = findTypeAux(oivFieldIn.getTypeId());

//    qVariant.convert((QVariant::Type)qMetaType);

    if (qMetaType == QMetaType::Void) {
        // do nothing
    }
    else if (qMetaType == QMetaType::Bool) {
        const SoSFBool & oivField = static_cast<const SoSFBool&>(oivFieldIn);
        qVariant.setValue(oivField.getValue());
    }
    else if (qMetaType == QMetaType::Float) {
        const SoSFFloat & oivField = static_cast<const SoSFFloat&>(oivFieldIn);
        qVariant.setValue(oivField.getValue());
    }
    else if (qMetaType == QMetaType::Int) {
        const SoSFInt32 & oivField = static_cast<const SoSFInt32&>(oivFieldIn);
        qVariant.setValue(oivField.getValue());
    }
    else if (qMetaType == QMetaType::Short) {
        const SoSFShort & oivField = static_cast<const SoSFShort&>(oivFieldIn);
        qVariant.setValue(oivField.getValue());
    }
    else if (qMetaType == QMetaType::QString) {
        const SoMFString & oivField = static_cast<const SoMFString&>(oivFieldIn);
        QString str;
        QTextStream stream(&str, QIODevice::WriteOnly);
        const int lineCount = oivField.getNum();
        for (int i = 0; i < lineCount; i++) {
            stream << QString(oivField[i].getString());
            if ((i + 1) < lineCount) {
                stream << endl;
            }
        }
        qVariant.setValue(str);
    }
    else if (qMetaType == QMetaType::UInt) {
        const SoSFUInt32 & oivField = static_cast<const SoSFUInt32&>(oivFieldIn);
        qVariant.setValue(oivField.getValue());
    }
    else if (qMetaType == QMetaType::UShort) {
        const SoSFUShort & oivField = static_cast<const SoSFUShort&>(oivFieldIn);
        qVariant.setValue(oivField.getValue());
    }

    return qVariant;
}

//--------------------------------------------------------------------------------
QVariant::Type TypeConverter::findType(const SoType oivFieldType)
{
    return (QVariant::Type)findTypeAux(oivFieldType);
}

//--------------------------------------------------------------------------------
SoType TypeConverter::getTypeFromName(const QString & typeName)
{
    SbName name(typeName.toAscii());
    return SoType::fromName(name);
}

