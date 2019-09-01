#ifndef PINKYPI_BVH_H
#define PINKYPI_BVH_H

#include <vector>
#include <memory>
#include "aabb.h"

namespace PinkyPi {
    
    class BVH {
    private:
        class Node {
        public:
            Node();
            
            AABB bounds;
            std::vector<std::shared_ptr<Node> > children;
            bool isLeaf;
        };
        
        Node *rootNode;
        std::vector<std::shared_ptr<Node> > leafNodes;
        
    public:
        BVH();
        BVH(int numleafs);
        ~BVH();
        
        void appendLeaf(const AABB& bnd);
        void build();
        
    private:
        Node* buildTree(const std::vector<std::shared_ptr<Node> >& childnodes, int depth);
    };
}


#endif
