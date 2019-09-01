#include <cstdio>
#include <iostream>
#include "sceneconverter.h"

#include <pinkycore/assetlibrary.h>
#include <pinkycore/scene.h>
#include <pinkycore/camera.h>
#include <pinkycore/mesh.h>
#include <pinkypi/sceneloader.h>

bool LoadAndConvertPinkyPiScene(std::string path, r1h::Scene *rrtscene) {
    PinkyPi::AssetLibrary *assetlib = nullptr;
    PinkyPi::Scene *ppscene = nullptr;
    
    assetlib = PinkyPi::SceneLoader::load(path);
    ppscene = assetlib->getDefaultScene();
    
    if(ppscene == nullptr) {
        std::cerr << "PinkyPi scene load failed:" << path << std::endl;
        return false;
    }
    
    ppscene->buildForTrace(assetlib);
    
    int numMeshs = ppscene->meshes.size();
    for(int imesh = 0; imesh < numMeshs; imesh++) {
        PinkyPi::Mesh *ppmesh = ppscene->meshes[imesh];
        
        r1h::SceneObject *rrtobj = new r1h::SceneObject();
        
        // TODO
        
    }
    
    // lights
    // TODO
    
    // bg
    // TODO
    
    // Camera
    auto rrtcam = new r1h::Camera();
    auto ppcam = ppscene->cameras[0];
    
    r1h::Matrix4 rrtm(ppcam->globalTransform.m);
    rrtcam->setTransform(rrtm);
    
    rrtcam->setFNumber(ppcam->fNumber);
    rrtcam->setFocalLength(ppcam->focalLength);
    rrtcam->setFocusDistance(ppcam->focusDistance);
    // TODO FOV
    
    return true;
}
