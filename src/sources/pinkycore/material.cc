//
//  material.cpp
//  PinkyCore
//
//  Created by SatoruNAKAJIMA on 2019/08/16.
//

#include "material.h"

#include "pptypes.h"
using namespace PinkyPi;

Material::Material():
    baseColorFactor(1.0, 1.0, 1.0),
    baseColorAlpha(1.0),
    metallicFactor(1.0),
    roughnessFactor(1.0),
    alphaMode(kAlphaAsBlend), // spec default : OPAQUE
    alphaCutoff(0.5),
    doubleSided(true), // spec default : false
    ior(1.33)
{
    
}

Material::~Material() {
    
}
