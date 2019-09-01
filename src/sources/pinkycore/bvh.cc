#include "bvh.h"

using namespace PinkyPi;

/////
BVH::Node::Node():
    isLeaf(false)
{
}

/////
BVH::BVH():
    rootNode(nullptr)
{
}

BVH::BVH(int numleafs):
    BVH()
{
    leafNodes.reserve(numleafs);
}

BVH::~BVH()
{
    delete rootNode;
}

void BVH::appendLeaf(const AABB& bnd) {
    auto node_sptr = std::make_shared<Node>();
    auto node = node_sptr.get();
    node->bounds = bnd;
    node->isLeaf = true;
    
    leafNodes.push_back(node_sptr);
}

void BVH::build()
{
    rootNode = buildTree(leafNodes, 0);
}

BVH::Node* BVH::buildTree(const std::vector<std::shared_ptr<Node> >& childnodes, int depth)
{
    if(childnodes.size() < 2) {
        // leaf
        return nullptr;
    } else {
        auto curnode = new Node();
        curnode->isLeaf = false;
        curnode->bounds.clear();
        
        for(auto inode = childnodes.begin(); inode != childnodes.end(); ++inode) {
            curnode->bounds.expand((*inode).get()->bounds);
        }
        
        Vector3 boundSize = curnode->bounds.size();
        int splitAxis = (std::abs(boundSize.x) > std::abs(boundSize.y))? 0 : 1;
        splitAxis = (std::abs(boundSize.v[splitAxis]) > std::abs(boundSize.z))? splitAxis : 2;
        
        
        
        return curnode;
    }
}
