#include "node.hpp"

class FieldList : public FieldNode {
public:
    FieldList(const char* name, NamedPVector<NodeBase>& children, bool named = false, int flags = 0);

protected:
    int set_override(const char* in, void* arg) override;
};