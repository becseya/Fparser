#include "node.hpp"

class FieldList : public FieldNode {
public:
    FieldList(const char* name, NamedPVector<NodeBase>& children, int flags = 0);

protected:
    int set_override(const char* in, void* arg) override;
};



class NamedFieldList : public FieldNode {
public:
    NamedFieldList(const char* name, NamedPVector<NodeBase>& children, int flags = 0);

protected:
    int set_override(const char* in, void* arg) override;
};
