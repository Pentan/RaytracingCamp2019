
#ifndef R1H_PBR_ROUGHNESS_METALLIC_MATERIAL_H
#define R1H_PBR_ROUGHNESS_METALLIC_MATERIAL_H

#include <vector>

#include "material.h"

namespace r1h {

class PBRRoughnessMetallicMaterial : public Material{
public:
    enum TexTypes {
        kBaseColor,
        kMetallicRoughness,
        kNormal,
        kEmissive,
        kOcclusion,
        
        kNumTextures
    };
    
public:
	PBRRoughnessMetallicMaterial(BSDFRef bsdf=nullptr);
	~PBRRoughnessMetallicMaterial();
    
	BurleyBSDF* getDiffuseBSDF() const;
    GGXBSDF* getSpecularBSDF() const;
	
    virtual Color getEmittance(const FinalIntersection &isect) const;
    virtual R1hFPType getTerminationProbability(const FinalIntersection &isect) const;
	virtual void makeNextSampleRays(Renderer::Context* cntx, const FinalIntersection &isect, const int depth) const;
	virtual Color evalShadowRay(Renderer::Context* cntx, const Ray &shadowray, const FinalIntersection &isect) const;
    
    virtual Vector3 getShadingNormal(const FinalIntersection &isect) const;
    
    void setBaseColorFactor(Color factor);
    void setRoughnessFactor(R1hFPType factor);
    void setMetallicFactor(R1hFPType factor);
    void setEmissiveFactor(Color factor);
    
    void setDiffuseRoughness(R1hFPType r);
    void setSpecularIor(R1hFPType ior);
    
private:
    BSDFRef diffuseBSDF;
    BSDFRef specularBSDF;
	
    Color baseColorFactor;
    R1hFPType roughnessFactor;
    R1hFPType metallicFactor;
    Color emissiveFactor;
    
    R1hFPType specularIor;
    R1hFPType diffuseRoughness;
};

}

#endif
