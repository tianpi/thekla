#include "SoTheklaNode.h"

SO_NODE_SOURCE(SoTheklaNode);

//--------------------------------------------------------------------------------
void SoTheklaNode::initClass(void)
{
    /**
     *"SO_NODE_INIT_CLASS(" <node class name> "," <node base class name> ",\"" <node base class without 'So'?!> ");"
     */
    SO_NODE_INIT_CLASS(SoTheklaNode, SoNode, "Node");
}


//--------------------------------------------------------------------------------
SoTheklaNode::SoTheklaNode(void)
{
    /**
     *"SO_NODE_CONSTRUCTOR(" <node class name> ");"
     */
    SO_NODE_CONSTRUCTOR(SoTheklaNode);

    /**
     * "SO_NODE_ADD_FIELD(" <field name> ",(" <field type default value> "));"
     */
    SO_NODE_ADD_FIELD(minimum, (0));
    SO_NODE_ADD_FIELD(maximum, (1));
    SO_NODE_ADD_FIELD(value, (1));

    SO_NODE_ADD_FIELD(rangeChangedMin, (0));
    SO_NODE_ADD_FIELD(rangeChangedMax, (1));
    rangeChanged.append(&rangeChangedMin);
    rangeChanged.append(&rangeChangedMax);

    SO_NODE_ADD_FIELD(valueChangedValue, (0));
    valueChanged.append(&valueChangedValue);

    SO_NODE_ADD_FIELD(cameraPosition, (0, 0, 0));

}


//--------------------------------------------------------------------------------
SoTheklaNode::~SoTheklaNode()
{
}
