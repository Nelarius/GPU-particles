#ifndef ISERIALIZABLE_H_INCLUDED
#define ISERIALIZABLE_H_INCLUDED

#include <serialization/ObjectNode.h>

namespace ce {

class ISerializable {
public:
    virtual void                    deserialize( ObjectNode::node_ptr ) = 0;
    virtual ObjectNode::node_ptr    serialize()                         = 0;
};

}

#endif // ISERIALIZABLE_H_INCLUDED
