//
//  light.cpp
//  PinkyCore
//
//  Created by SatoruNAKAJIMA on 2019/08/16.
//

#include "light.h"

#include "pptypes.h"
using namespace PinkyPi;

Light::Light():
    color(0.0, 0.0, 0.0),
    intensity(1.0),
    lightType(kPointLight),
    spot(0.0, kPI * 0.25)
{
}

Light::~Light() {
    
}
