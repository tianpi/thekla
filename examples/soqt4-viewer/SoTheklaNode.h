/**
 * -- Header head
 */
#ifndef SOTHEKLANODE_H
#define SOTHEKLANODE_H

#include <Inventor/nodes/SoSubNode.h>

/**
 * -- Local includes
 */
//#include "SoTheklaNodeBase.h"

/**
 * -- Global includes
 *
 * (1) Node fields types
 *
 * common prefix: "Inventor/fields/" <field class name> + ".h"
 */
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFVec3f.h>


//--------------------------------------------------------------------------------
class SoTheklaNode : public SoNode
{
    SO_NODE_HEADER(SoTheklaNode);

public:
    static void initClass(void);

public:
    SoTheklaNode(void);

protected:
    virtual ~SoTheklaNode();

public:

    /**
     * Example QObject: QSlider ( -> QAbstractSlider -> QWidget -> QObject)
     */

    /**
     * --- Property
     *
     * o Syntax
     *
     * p := [ t, n ] (param = type, name)
     * f := [ ~t, ~n ]
     *
     * o Conversion
     *
     * t -> ~t: (QVariant.Type) => (SoField.SoType) (TODO)
     * n -> ~n: property name => field name
     *
     * o Example: int minimum
     *
     * SoSFInt32 minimum;
     */
    SoSFInt32 minimum;
    SoSFInt32 maximum;
    SoSFInt32 value;

    /**
     * --- Signal
     *
     * o input: signalName(t1 n1, t2 n2, .. , tn nn)
     *
     * pi := [ ti, ni ] (param = type, name)
     *
     * p1, p2, .. , pn => [ f1, f2, .. , fn ] := SoFieldList
     *
     * fi := [ ~t, ~ni ] (field = type, name)
     *
     * o Conversion
     *
     * type ti => ~ti : QVariant.Type => SoField.SoType
     * name: ni => ~ni : signalName 'Param' 'X', X = 1..n OR param-name if available
     *
     * o Paramters are collected in SoFieldList
     *
     * SoFieldList <signalName>
     *
     * o Example:  void rangeChanged(int min, int max)
     *
     * p1 := int min
     * p2 := int max
     *
     * f1 := SoSFInt32 rangeChangedMin;
     * f2 := SoSFInt32 rangeChangedMax;
     *
     * Alternatively (if parameter name is not available)
     *
     * f1 := SoSFInt32 rangeChangedParam0
     * f2 := SoSFInt32 rangeChangedParam1
     *
     */
    SoSFInt32 rangeChangedMin;
    SoSFInt32 rangeChangedMax;
    SoFieldList rangeChanged;

    SoSFInt32 valueChangedValue;
    SoFieldList valueChanged;

    /**
     * --- SoField
     *
     * o Syntax
     *
     * f := t, n
     *
     * t := SoField.SoType
     * n := "manual identifier"
     *
     * o Conversion
     *
     * t -> 't : SoField.SoType => SoField.SoType
     * n -> 'n : "manual identifier" => "manual identifier"
     *
     * o Example: SoSFVec3f cameraPosition
     *
     * SoSFVec3f cameraPosition
     */
    SoSFVec3f cameraPosition;
};

#endif // SOTHEKLANODE_H
