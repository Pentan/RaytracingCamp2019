//
//  light.cpp
//  PinkyCore
//
//  Created by SatoruNAKAJIMA on 2019/08/16.
//

#include "light.h"
#include "pptypes.h"
#include "node.h"

using namespace PinkyPi;

Light::Light():
    color(0.0, 0.0, 0.0),
    intensity(1.0),
    lightType(LightType::kPointLight),
    spot(0.0, kPI * 0.25),
    sampleWeight(1.0)
{
}

Light::~Light() {
    
}

Color Light::evaluate(const Node* node, const SurfaceInfo& surf, PPTimeType timerate, EvalLog* log) const {
    Color ret;

    switch (lightType)
    {
        case LightType::kPointLight:
        {
            Vector3 lp = Matrix4::transformV3(node->computeGlobalMatrix(timerate), Vector3(0.0, 0.0, 0.0));
            Vector3 lv = surf.position - lp;
            PPFloat ll = lv.length();
            lv = lv / ll;
            ret = color * intensity / (ll * ll) / (54.351413 * 2.0 * kPI);

            log->lightPdf = 1.0;
            log->position = lp;
            log->direction = lv;
        }
            break;
        case LightType::kDirectionalLight:
        {
            Vector3 lv = Matrix4::mulV3(node->computeGlobalMatrix(timerate), Vector3(0.0, 0.0, -1.0));
            lv.normalize();
            ret = color * intensity / 683.0;

            log->lightPdf = 1.0;
            log->position = surf.position - lv * kDirLightOffset;
            log->direction = lv;
        }
            break;
        case LightType::kSpotLight: // TODO
        case LightType::kMeshLight: // TODO
        default:
            log->lightPdf = 1.0;
            log->position.set(0.0, 0.0, 0.0);
            log->direction.set(0.0, 0.0, -1.0);
            break;
    }

    return ret;
}
