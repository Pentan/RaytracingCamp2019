#ifndef PWR_SCENECONVERTER_H
#define PWR_SCENECONVERTER_H

#include <string>

#include <rrtcore/scene.h>
#include <pinkycore/config.h>

bool LoadAndConvertPinkyPiScene(const std::string& path, const PinkyPi::Config *ppconf, r1h::Scene *rrtscene);

#endif
