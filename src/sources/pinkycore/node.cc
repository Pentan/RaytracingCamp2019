//
//  node.cpp
//  PinkyCore
//
//  Created by SatoruNAKAJIMA on 2019/08/16.
//

#include "node.h"

#include "pptypes.h"
using namespace PinkyPi;

Node::Node():
    contentType(kContentTypeEmpty)
{
    initialTransform.matrix.setIdentity();
    initialTransform.translate.set(0.0, 0.0, 0.0);
    initialTransform.rotation.set(0.0, 0.0, 0.0, 1.0);
    initialTransform.scale.set(1.0, 1.0, 1.0);
    currentTransform = initialTransform;
}

Node::~Node() {
    
}

void Node::Transfom::makeMatrix() {
    matrix.setIdentity();
    matrix.translate(translate);
    matrix = matrix * rotation.getMatrix();
    matrix.scale(scale);
}
