
#include "pbrrmmaterial.h"

using namespace r1h;

PBRRoughnessMetallicMaterial::PBRRoughnessMetallicMaterial(BSDFRef bsdf):
	Material(),
    baseColorFactor(1.0, 1.0, 1.0),
    roughnessFactor(1.0),
    metallicFactor(1.0),
    specularIor(1.5),
    diffuseRoughness(0.5)
{
    textures.resize(kNumTextures);
    diffuseBSDF = BSDFRef(new BurleyBSDF());
    specularBSDF = BSDFRef(new GGXBSDF());
}

PBRRoughnessMetallicMaterial::~PBRRoughnessMetallicMaterial() {
}

BurleyBSDF* PBRRoughnessMetallicMaterial::getDiffuseBSDF() const {
    return reinterpret_cast<BurleyBSDF*>(diffuseBSDF.get());
}
GGXBSDF* PBRRoughnessMetallicMaterial::getSpecularBSDF() const {
    return reinterpret_cast<GGXBSDF*>(specularBSDF.get());
}

R1hFPType PBRRoughnessMetallicMaterial::getTerminationProbability(const FinalIntersection &isect) const {
    Texture *albedtex = getTexture(kBaseColor);
    Color albed = albedtex->sample(&isect);
    albed = Vector3::mul(albed, baseColorFactor);
    R1hFPType albedmax = albed.getMaxComponent();
    
    Texture *roughtex = getTexture(kMetallicRoughness);
    R1hFPType gloss = 1.0 - roughtex->sample(&isect).y * roughnessFactor;
    
    return std::max(albedmax, gloss);
}

Color PBRRoughnessMetallicMaterial::getEmittance(const FinalIntersection &isect) const {
	Texture *tex = getTexture(kEmissive);
    return Color::mul(tex->sample(&isect), emissiveFactor);
}

void PBRRoughnessMetallicMaterial::makeNextSampleRays(Renderer::Context* cntx, const FinalIntersection &isect, const int depth) const {
    Color albedo = getTexture(kBaseColor)->sample(&isect);
    albedo = Color::mul(albedo, baseColorFactor);
    
    Color metalrough = getTexture(kMetallicRoughness)->sample(&isect);
    R1hFPType metalness = metalrough.r * metallicFactor;
    R1hFPType roughness = metalrough.g * roughnessFactor;
    
    Color diffuseweight = albedo * (1.0 - metalness);
    Color specularweight = Vector3(1.0 - metalness) + albedo * metalness;
    specularweight = specularweight * (1.0 - roughness);
    
    BurleyBSDF *diffusebsdf = getDiffuseBSDF();
    GGXBSDF *specbsdf = getSpecularBSDF();
    
    if(depth > 1) {
        // diffuse or specular
        R1hFPType prob = 0.25 + 0.5 * std::max(metalness, roughness);
        
        if(cntx->random.next01() > prob) {
            BSDF::Sample smpl = specbsdf->getSample(cntx, isect, roughness, specularIor);
            cntx->emitRay(smpl.getRay(), specularweight * (1.0 / prob), specbsdf->getType());
        } else {
            BSDF::Sample smpl = diffusebsdf->getSample(cntx, isect, diffuseRoughness);
            cntx->emitRay(smpl.getRay(), diffuseweight * (1.0 / (1.0 - prob)), diffusebsdf->getType());
        }
    } else {
        // both
        BSDF::Sample smpl;
        smpl = specbsdf->getSample(cntx, isect, roughness, specularIor);
        cntx->emitRay(smpl.getRay(), specularweight, specularBSDF->getType());
        
        smpl = diffusebsdf->getSample(cntx, isect, diffuseRoughness);
        cntx->emitRay(smpl.getRay(), diffuseweight, diffuseBSDF->getType());
    }
}

Color PBRRoughnessMetallicMaterial::evalShadowRay(Renderer::Context* cntx, const Ray &shadowray, const FinalIntersection &isect) const {
#if 0
    if((bsdf->getType() & (BSDF::kSpecular | BSDF::kRefraction)) != 0) {
        return Color(0.0);
    }
    
    const Ray &inray = cntx->getCurrentInsidentRay();
    BSDF::Sample insmpl;
    insmpl.position = isect.position;
    insmpl.normal = isect.shadingNormal;
    insmpl.direction = inray.direction;
    
    BSDF::Sample outsmpl;
    outsmpl.position = isect.position;
    outsmpl.normal = isect.shadingNormal;
    outsmpl.direction = shadowray.direction;
    
    Color f = bsdf->evaluate(insmpl, outsmpl);
    Texture *tex = getTexture(kReflectance);
    Color reflectance = tex->sample(&isect);
    //R1hFPType p = bsdf->probabilityForSample(insmpl, outsmpl);
    
    return Color::mul(f, reflectance);// / p;
#else
    return Color(0.0);
# endif
}

Vector3 PBRRoughnessMetallicMaterial::getShadingNormal(const FinalIntersection &isect) const {
    Texture *tex = getTexture(kNormal);
    if(tex == nullptr) {
        return isect.hitNormal;
    } else {
        // normal map
        Vector3 retnorm;
        Vector3 mapnorm = tex->sampleAsVector(&isect);
        switch (tex->getMapSpace()) {
            case Texture::kTangentSpace:
            {
                retnorm.x = mapnorm.x * isect.shadingTangent.x + mapnorm.y * isect.shadingNormal.x + mapnorm.z * isect.shadingCotangent.x;
                retnorm.y = mapnorm.x * isect.shadingTangent.y + mapnorm.y * isect.shadingNormal.y + mapnorm.z * isect.shadingCotangent.y;
                retnorm.z = mapnorm.x * isect.shadingTangent.z + mapnorm.y * isect.shadingNormal.z + mapnorm.z * isect.shadingCotangent.z;
                retnorm.normalize();
            }
                break;
            case Texture::kObjectSpace:
                // TODO
                retnorm = mapnorm;
                break;
            case Texture::kWorldSpace:
            default:
                retnorm = mapnorm;
                break;
        }
        return retnorm;
    }
}


void PBRRoughnessMetallicMaterial::setBaseColorFactor(Color factor) {
    baseColorFactor = factor;
}

void PBRRoughnessMetallicMaterial::setRoughnessFactor(R1hFPType factor) {
    roughnessFactor = factor;
}

void PBRRoughnessMetallicMaterial::setMetallicFactor(R1hFPType factor) {
    metallicFactor = factor;
}

void PBRRoughnessMetallicMaterial::setEmissiveFactor(Color factor) {
    emissiveFactor = factor;
}

void PBRRoughnessMetallicMaterial::setDiffuseRoughness(R1hFPType r) {
    diffuseRoughness = r;
}

void PBRRoughnessMetallicMaterial::setSpecularIor(R1hFPType ior) {
    specularIor = ior;
}

