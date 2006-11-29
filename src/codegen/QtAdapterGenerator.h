
#ifndef THEKLA_QTADAPTERGENERATOR_H
#define THEKLA_QTADAPTERGENERATOR_H

#include "CxxCodeGenerator.h"
#include <QMetaObject>
#include <QMetaMethod>

//--------------------------------------------------------------------------------
class THEKLA_EXPORT  QtAdapterGenerator : public CxxCodeGenerator
{
private:
    typedef struct {
        QString type_;
        QString name_;
    } Parameter;
    typedef QList<Parameter> ParameterList;

    typedef struct {
        QString name_;
        QString signature_;
        ParameterList parameterList_;
        bool isSerializable_;
    } Method;
    typedef QList<Method> MethodList;

private:
    const QMetaObject * qMeta_;

    QString adapterOwnerClassName_;

    MethodList adapterSignalList_;
    MethodList adapterSlotList_;

public:
    QtAdapterGenerator(const QMetaObject * qMeta);
    ~QtAdapterGenerator() {};

private:

    void preProcess();

    void preProcessSignal(const QMetaMethod & qSignal);

    void generateCode();

    void codeConstructor();

    void codeDestructor();

    void codeCreatorFunc();

    void codeMethodHeader(Method method);

    void codeSlotBody(Method adapterSlot);

    void codeEmitHeader();

    void codeEmitBody();

    void codeEmitBodySignal(Method adapterSignal);
};

#endif // THEKLA_QTADAPTERGENERATOR_H
