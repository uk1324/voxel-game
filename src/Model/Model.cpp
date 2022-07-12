#include <Model/Model.hpp>

size_t Model::nodeIndex(const Node* node) const
{
    return node - nodes.data();
}
