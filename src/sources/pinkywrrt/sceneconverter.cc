#include <cstdio>
#include <iostream>
#include "sceneconverter.h"

#include <pinkycore/assetlibrary.h>
#include <pinkycore/scene.h>
#include <pinkycore/camera.h>
#include <pinkycore/mesh.h>
#include <pinkycore/material.h>
#include <pinkycore/texture.h>
#include <pinkypi/sceneloader.h>

namespace {
    r1h::TextureRef ConvertImageTexture(PinkyPi::Texture *pptex) {
        auto ppimgtx = reinterpret_cast<PinkyPi::ImageTexture*>(pptex);
        auto rrttex = new r1h::ImageTexture();
        
        int w = ppimgtx->width;
        int h = ppimgtx->height;
        int numTexels = w * h;
        std::vector<r1h::Color> tmptxl(numTexels);
        for(int i = 0; i < numTexels; i++) {
            const PinkyPi::Color& srcrgb = ppimgtx->image[i].rgb;
            tmptxl[i].set(srcrgb.r, srcrgb.g, srcrgb.b);
        }
        rrttex->init(tmptxl.data(), w, h);
        rrttex->setMapType(r1h::ImageTexture::kUV);
        
        return r1h::TextureRef(rrttex);
    }
    
    r1h::MaterialRef ConvertMaterial(PinkyPi::Material *ppmat) {
        auto rrtmat = new r1h::PBRRoughnessMetallicMaterial();
        const PinkyPi::Color& bcf = ppmat->baseColorFactor;
        rrtmat->setBaseColorFactor(r1h::Color(bcf.r, bcf.g, bcf.b));
        rrtmat->setMetallicFactor(ppmat->metallicFactor);
        rrtmat->setRoughnessFactor(ppmat->roughnessFactor);
        const PinkyPi::Color& emf = ppmat->emissiveFactor;
        rrtmat->setEmissiveFactor(r1h::Color(emf.r, emf.g, emf.b));
        
        if(ppmat->baseColorTexture.texture != nullptr) {
            auto tex = ConvertImageTexture(ppmat->baseColorTexture.texture);
            rrtmat->setTexture(r1h::PBRRoughnessMetallicMaterial::kBaseColor, tex);
        } else {
            auto tex = r1h::TextureRef(new r1h::ConstantColorTexture(r1h::Color(1.0, 1.0, 1.0)));
            rrtmat->setTexture(r1h::PBRRoughnessMetallicMaterial::kBaseColor, tex);
        }
        
        if(ppmat->metallicRoughnessTexture.texture != nullptr) {
            auto tex = ConvertImageTexture(ppmat->metallicRoughnessTexture.texture);
            rrtmat->setTexture(r1h::PBRRoughnessMetallicMaterial::kMetallicRoughness, tex);
        } else {
            auto tex = r1h::TextureRef(new r1h::ConstantColorTexture(r1h::Color(1.0, 1.0, 1.0)));
            rrtmat->setTexture(r1h::PBRRoughnessMetallicMaterial::kMetallicRoughness, tex);
        }
        
        if(ppmat->emissiveTexture.texture != nullptr) {
            auto tex = ConvertImageTexture(ppmat->emissiveTexture.texture);
            rrtmat->setTexture(r1h::PBRRoughnessMetallicMaterial::kEmissive, tex);
        } else {
            auto tex = r1h::TextureRef(new r1h::ConstantColorTexture(r1h::Color(0.0, 0.0, 0.0)));
            rrtmat->setTexture(r1h::PBRRoughnessMetallicMaterial::kEmissive, tex);
        }
        
        if(ppmat->normalTexture.texture != nullptr) {
            auto tex = ConvertImageTexture(ppmat->normalTexture.texture);
            tex->setMapType(r1h::Texture::kTangentSpace);
            rrtmat->setTexture(r1h::PBRRoughnessMetallicMaterial::kNormal, tex);
        } else {
//            auto tex = r1h::TextureRef(new r1h::ConstantColorTexture(r1h::Color(0.5, 0.5, 1.0)));
//            tex->setMapType(r1h::Texture::kTangentSpace);
//            rrtmat->setTexture(r1h::PBRRoughnessMetallicMaterial::kNormal, tex);
            auto tex = r1h::TextureRef(nullptr);
            rrtmat->setTexture(r1h::PBRRoughnessMetallicMaterial::kNormal, tex);
        }
        
        rrtmat->setSpecularIor(ppmat->ior);
        
        return r1h::MaterialRef(rrtmat);
    }
    
    void ConvertPinkyMeshToRrTObject(PinkyPi::Mesh *ppmesh, r1h::SceneObject *rrtobj) {
        auto rrtmesh = new r1h::Mesh();
        rrtmesh->newAttributeContainer(); // 0:uv
        
        int numClstrs = ppmesh->clusters.size();
        int vertOffset = 0;
        for(int icls = 0; icls < numClstrs; icls++) {
            auto ppcluster = ppmesh->clusters[icls].get();
            auto rrtmat = ConvertMaterial(ppcluster->material);
            
            rrtobj->addMaterial(rrtmat);
            int matId = icls;
            
            for(int ivert = 0; ivert < ppcluster->vertices.size(); ivert++) {
                const PinkyPi::Vector3& ppvrt = ppcluster->vertices[ivert];
                const PinkyPi::Mesh::Attributes& ppattr = ppcluster->attributes[ivert];
                
                r1h::Vector3 rrtvrt(ppvrt.x, ppvrt.y, ppvrt.z);
                r1h::Vector3 rrtnorm(ppattr.normal.x, ppattr.normal.y, ppattr.normal.z);
                r1h::Vector3 rrttan(ppattr.tangent.x, ppattr.tangent.y, ppattr.tangent.z);
                r1h::Vector3 rrtuv0(ppattr.uv0.x, ppattr.uv0.y, ppattr.uv0.z);
                
                rrtmesh->addVertex(rrtvrt);
                rrtmesh->addNormal(rrtnorm);
                rrtmesh->addTangent(rrttan);
                rrtmesh->addAttribute(0, rrtuv0);
            }
            
            for(int itri = 0; itri < ppcluster->triangles.size(); itri++) {
                const PinkyPi::Mesh::Triangle& pptriangle = ppcluster->triangles[itri];
                int rrtfa = pptriangle.a + vertOffset;
                int rrtfb = pptriangle.b + vertOffset;
                int rrtfc = pptriangle.c + vertOffset;
                
                rrtmesh->addFace(rrtfa, rrtfb, rrtfc, matId);
            }
            
            vertOffset += ppcluster->vertices.size();
        }
        
        rrtmesh->postProcess();
        rrtobj->setGeometry(r1h::GeometryRef(rrtmesh));
        
        r1h::Matrix4 rrtm(ppmesh->globalTransform.m);
        rrtobj->setTransform(rrtm);
    }
    
    void ConvertBackground(PinkyPi::Scene *ppscn, r1h::Scene *rrtscene) {
        auto skymat = new r1h::SkyMaterial();
        if(ppscn->background == nullptr) {
            skymat->setColor(r1h::Color(0.8, 0.8, 0.8));
            //skymat->setColor(r1h::Color(0.5, 0.5, 0.5));
        
        } else {
            auto bgtex = ConvertImageTexture(ppscn->background->baseColorTexture.texture);
            skymat->setTexture(bgtex);
        }
        
        rrtscene->setSkyMaterial(r1h::SkyMaterialRef(skymat));
    }
    
    r1h::CameraRef ConvertCamera(PinkyPi::Camera *ppcam) {
        auto rrtcam = new r1h::Camera();
        
        r1h::Matrix4 rrtm(ppcam->globalTransform.m);
        rrtcam->setTransform(rrtm);
        
        rrtcam->setFNumber(ppcam->fNumber);
        rrtcam->setFocalLength(ppcam->focalLength);
        rrtcam->setFocusDistance(ppcam->focusDistance);
        rrtcam->setSensorWidthWithAspect(32.0f, ppcam->perspective.aspect);
        rrtcam->setFocalLengthFromFOV(ppcam->perspective.yfov);

        return r1h::CameraRef(rrtcam);
    }
}

bool LoadAndConvertPinkyPiScene(const std::string& path, const PinkyPi::Config *ppconf, r1h::Scene *rrtscene) {
    PinkyPi::AssetLibrary *assetlib = nullptr;
    PinkyPi::Scene *ppscene = nullptr;
    
    assetlib = PinkyPi::SceneLoader::load(path, ppconf);
    ppscene = assetlib->getDefaultScene();
    
    if(ppscene == nullptr) {
        std::cerr << "PinkyPi scene load failed:" << path << std::endl;
        return false;
    }
    
    ppscene->buildForTrace(assetlib);
    
    // mesh
    int numMeshs = ppscene->meshes.size();
    for(int imesh = 0; imesh < numMeshs; imesh++) {
        PinkyPi::Mesh *ppmesh = ppscene->meshes[imesh];
        
        r1h::SceneObject *rrtobj = new r1h::SceneObject();
        ConvertPinkyMeshToRrTObject(ppmesh, rrtobj);
        
        rrtscene->addObject(r1h::SceneObjectRef(rrtobj));
    }
    
    // lights
    // TODO
    
    // bg
    ConvertBackground(ppscene, rrtscene);
    
    // Camera
    auto rrtcam = ConvertCamera(ppscene->cameras[0]);
    rrtscene->setCamera(rrtcam);
    
    return true;
}
