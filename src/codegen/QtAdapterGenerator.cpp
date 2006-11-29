
#include "QtAdapterGenerator.h"

#include <common/QtAdapterUtils.h>
#include <common/QtObjectUtils.h>

#include <QVariant>

//--------------------------------------------------------------------------------
QtAdapterGenerator::QtAdapterGenerator(const QMetaObject * qMeta)
    : CxxCodeGenerator(),
      qMeta_(qMeta),
      adapterOwnerClassName_(),
      adapterSignalList_(),
      adapterSlotList_()
{
}

//--------------------------------------------------------------------------------
void QtAdapterGenerator::preProcess()
{
    className_ = QtAdapterUtils::getAdapterClassName(qMeta_);
    baseClassName_ = QtAdapterUtils::getAdapterBaseClassName(qMeta_);
    adapterOwnerClassName_ = QtAdapterUtils::getAdapterOwnerClassName();

    localIncludeList_.push_back(QtAdapterUtils::getAdapterHeaderFileName(baseClassName_));

    for (int i = qMeta_->methodOffset(); i < qMeta_->methodCount(); i++) {
        QMetaMethod qSignal = qMeta_->method(i);
        if (qSignal.methodType() != QMetaMethod::Signal)
            continue;
        preProcessSignal(qSignal);
    }
}

//--------------------------------------------------------------------------------
void QtAdapterGenerator::preProcessSignal(const QMetaMethod & qSignal)
{
    // create data containers
    Method adapterSignal, adapterSlot;

    // basic settings
    QString signature(qSignal.signature());
    adapterSignal.signature_ = signature;
    adapterSlot.signature_ = signature;
    adapterSignal.name_ = QtAdapterUtils::getSignalName(qSignal);
    adapterSlot.name_ = QtAdapterUtils::getSlotName(qSignal);

    ParameterList paramList;
    QList<QByteArray> paramTypeList = qSignal.parameterTypes();
    for (int i=0; i < paramTypeList.size(); i++) {

        // fetch parameter type
        QString paramType(paramTypeList.value(i));

        // parameter type is Qt class
        if (paramType.startsWith('Q')) {
            QString includeName = paramType;

            // remove pointer '*'
            includeName = includeName.left(includeName.indexOf('*'));
            // remove inner class symbols '::'
            includeName = includeName.left(includeName.indexOf("::"));

            if (!globalIncludeList_.contains(includeName))
                globalIncludeList_.push_back(includeName);
        }

        Parameter param;
        param.type_ = paramType;
        param.name_ = "v" + QString::number(i);

        paramList.push_back(param);
    }

    // add parameter list to methods
    adapterSlot.parameterList_ = paramList;
    adapterSignal.parameterList_ = paramList;

    // parameter types can be de/serialialized ?
    bool b = QtObjectUtils::isSerializable(qSignal);
    adapterSlot.isSerializable_ = b;
    adapterSignal.isSerializable_ = b;

    // add to method lists
    adapterSignalList_.push_back(adapterSignal);
    adapterSlotList_.push_back(adapterSlot);
}

//--------------------------------------------------------------------------------
void QtAdapterGenerator::codeConstructor()
{
    code_ <<  className_ + "(" + adapterOwnerClassName_ + "* owner)";
    code_ <<  " : " + baseClassName_  + "(owner)";
    code_ << "{};";
}

//--------------------------------------------------------------------------------
void QtAdapterGenerator::codeDestructor()
{
    code_ <<  "virtual ~" + className_ + "()";
    code_ << " {}; ";
}

//--------------------------------------------------------------------------------
void QtAdapterGenerator::codeCreatorFunc()
{
    code_ << "static " + QtAdapterUtils::getAdapterSuperBaseClassName() + "* create(" + adapterOwnerClassName_ + "* owner)";
    code_ << "{";
    code_ << "  return new " + className_ + "(owner);";
    code_ << "};";
}

//--------------------------------------------------------------------------------
void QtAdapterGenerator::codeMethodHeader(Method method)
{
    code_ << "void " + method.name_;
    code_ << "(";

    int paramCount = method.parameterList_.size();
    for (int i=0; i < paramCount; i++)
    {
        Parameter parameter = method.parameterList_.value(i);
        code_ << parameter.type_ + " " + parameter.name_;
        if ((i + 1) <  paramCount)
            code_ << ", ";
    }
    code_ << ")";
}

//--------------------------------------------------------------------------------
void QtAdapterGenerator::codeSlotBody(Method adapterSlot)
{
    code_ << "{";
    code_ << "QByteArray a;";
    code_ << "QDataStream s(&a, QIODevice::WriteOnly);";

    int paramCount = adapterSlot.parameterList_.size();
    for (int i=0; i < paramCount; i++)
    {
        Parameter param = adapterSlot.parameterList_.value(i);
        code_ << "s << QVariant((" + param.type_ + ")" + param.name_ + ");";
    }
    code_ <<  "signalChanged(\"" + adapterSlot.signature_ + "\", a);";
    code_ << "};";
}

//--------------------------------------------------------------------------------
void QtAdapterGenerator::codeEmitHeader()
{
    code_ << "virtual bool signalEmit";
    code_ << "(";
    code_ << "const QString & signalSignature";
    code_ << ",";
    code_ << "const QByteArray & paramData";
    code_ << ")";

}

//--------------------------------------------------------------------------------
void QtAdapterGenerator::codeEmitBody()
{
    code_ << "{";
    code_ << "";
    code_ << "QByteArray a(paramData);";
    code_ << "QDataStream s(&a, QIODevice::ReadOnly);";
    code_ << "QVariant v;";
    code_ << "";

    for (int i=0; i < adapterSignalList_.size(); i++)
    {
        Method sig = adapterSignalList_.value(i);
        codeEmitBodySignal(sig);
    };

    code_ << "";
    code_ << "if (" + baseClassName_ + "::signalEmit(signalSignature, paramData))";
    code_ << "  return true;";
    code_ << "";
    code_ << "return false;";
    code_ << "};";
}

//--------------------------------------------------------------------------------
void QtAdapterGenerator::codeEmitBodySignal(Method adapterSignal)
{
    if (!adapterSignal.isSerializable_)
        return;

    code_ << "if (signalSignature == \"" + adapterSignal.signature_ + "\")";
    code_ << "{";

    int paramCount = adapterSignal.parameterList_.size();
    for (int i=0; i < paramCount; i++)
    {
        Parameter param = adapterSignal.parameterList_.value(i);
        code_ << param.type_ + " " + param.name_ + ";";
        code_ << "s >> v;";
        code_ << param.name_ + " = v.value<" + param.type_ + ">();";
    };

    code_ << "emit " + adapterSignal.name_;
    code_ << "(";

    for (int i=0; i < paramCount; i++)
    {
        Parameter param = adapterSignal.parameterList_.value(i);
        code_ << param.name_;
        if ((i+1) < paramCount)
            code_ << ", ";
    }
    code_ << ");";
    code_ << "return true;";
    code_ << "}";
}

//--------------------------------------------------------------------------------
void QtAdapterGenerator::generateCode()
{
    codeHeaderHead();
    codeLocalIncludes();
    codeGlobalIncludes();
    codeForwardDecls();
    codeClassDecl();
    code_ << "{";
    code_ << "Q_OBJECT";
    code_ << "protected:";
    codeConstructor();
    code_ << "public:";
    codeDestructor();
    code_ << "public:";
    codeCreatorFunc();
    code_ << "public slots:";
    for (int i=0; i < adapterSlotList_.size(); i++)
    {
        Method slot = adapterSlotList_.value(i);
        if (!slot.isSerializable_)
            continue;

        codeMethodHeader(slot);
        codeSlotBody(slot);
    }
    code_ << "signals:";
    for (int i=0; i < adapterSignalList_.size(); i++)
    {
        Method signal = adapterSignalList_.value(i);
        if (!signal.isSerializable_)
            continue;

        codeMethodHeader(signal);
        code_ << ";";
    }
    code_ << "public:";
    codeEmitHeader();
    codeEmitBody();
    code_ << "};";
    codeHeaderFoot();
    code_ << "";
}
