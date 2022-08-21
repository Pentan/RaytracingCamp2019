//
//  intersection.h
//  PinkyCore
//
//  Created by SatoruNAKAJIMA on 2022/08/14.
//

#ifndef PINKYPI_INTERSECTION_H
#define PINKYPI_INTERSECTION_H

#include "pptypes.h"

namespace PinkyPi {
    //
    struct MeshIntersection {
        int clusterId;
        int triangleId;
    };
    
    /////
    struct SceneIntersection {
        int meshId;
        MeshIntersection meshIntersect;
        
        struct Detail {
            Vector3 geometryNormal;
            
            Vector3 barycentricCoord;
            Vector3 shadingNormal;
            Vector4 shadingTangent;
            Vector3 texcoord0;
        };
    };
}
#endif /* PINKYPI_INTERSECTION_H */
