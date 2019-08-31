#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <memory>

#include <nlohmann/json.hpp>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#define TINYGLTF_NO_INCLUDE_JSON
#define TINYGLTF_NO_INCLUDE_STB_IMAGE
#define TINYGLTF_NO_INCLUDE_STB_IMAGE_WRITE
#include <tinygltf/tiny_gltf.h>

#include <pinkycore/assetlibrary.h>
#include <pinkycore/scene.h>

#include "sceneloader.h"

#include <pinkycore/material.h>
#include <pinkycore/texture.h>
#include <pinkycore/mesh.h>
#include <pinkycore/light.h>
#include <pinkycore/node.h>
#include <pinkycore/camera.h>
//#include <pinkycore/>
//#include <pinkycore/>

using namespace PinkyPi;

namespace {
    // Constants
    std::string kExtraKey = "PinkyPi-Extra";
    
    // Utikity Classes
    class BufferAccessor {
    private:
        unsigned char* bufferPtr;
        size_t byteStride;
        size_t componentSize;
        int componentsInStruct;
        int structCount;
        float componentNormalizer;
        
        int curStruct;
        int curComponent;
        
        float (BufferAccessor::*getCompFloat)(int, int) const;
        int (BufferAccessor::*getCompInt)(int, int) const;
        
        template<typename R, typename T> R getT(int structIndex, int componentIndex) const {
            T *val = (T*)(bufferPtr + byteStride * structIndex + componentIndex * componentSize);
            return R(*val);
        }
        
    public:
        BufferAccessor():
            bufferPtr(nullptr),
            byteStride(0),
            componentSize(0),
            componentsInStruct(1),
            structCount(0),
            componentNormalizer(1.0)
        {
            rewind();
        }
        
        BufferAccessor(const tinygltf::Accessor& accessor, const tinygltf::Model& model) {
            init(accessor, model);
        }
        
        void init(const tinygltf::Accessor& accessor, const tinygltf::Model& model) {
            auto bufferview = model.bufferViews[accessor.bufferView];
            auto buffer = model.buffers[bufferview.buffer];
            
            size_t offset = bufferview.byteOffset + accessor.byteOffset;
            bufferPtr = buffer.data.data() + offset;
            byteStride = bufferview.byteStride;
            structCount = accessor.count;
            
            switch (accessor.type) {
                case TINYGLTF_TYPE_SCALAR:
                    componentsInStruct = 1;
                    break;
                case TINYGLTF_TYPE_VEC2:
                    componentsInStruct = 2;
                    break;
                case TINYGLTF_TYPE_VEC3:
                    componentsInStruct = 3;
                    break;
                case TINYGLTF_TYPE_VEC4:
                    componentsInStruct = 4;
                    break;
                case TINYGLTF_TYPE_MAT2:
                    componentsInStruct = 4;
                    break;
                case TINYGLTF_TYPE_MAT3:
                    componentsInStruct = 9;
                    break;
                case TINYGLTF_TYPE_MAT4:
                    componentsInStruct = 16;
                    break;
                default:
                    // unknown type
                    std::cerr << "Unknown component type accessor:" << accessor.componentType << std::endl;
                    componentsInStruct = 1;
                    break;
            }
            
            switch (accessor.componentType) {
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                    componentSize = 1;
                    componentNormalizer = 1.0 / float(0xff);
                    getCompFloat = &BufferAccessor::getT<float, unsigned char>;
                    getCompInt = &BufferAccessor::getT<int, unsigned char>;
                    break;
                    
                case TINYGLTF_COMPONENT_TYPE_BYTE:
                    componentSize = 1;
                    componentNormalizer = 1.0 / float(0x7f);
                    getCompFloat = &BufferAccessor::getT<float, char>;
                    getCompInt = &BufferAccessor::getT<int, char>;
                    break;
                    
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                    componentSize = 2;
                    componentNormalizer = 1.0 / float(0xffff);
                    getCompFloat = &BufferAccessor::getT<float, unsigned short>;
                    getCompInt = &BufferAccessor::getT<int, unsigned short>;
                    break;
                    
                case TINYGLTF_COMPONENT_TYPE_SHORT:
                    componentSize = 2;
                    componentNormalizer = 1.0 / float(0x7fff);
                    getCompFloat = &BufferAccessor::getT<float, short>;
                    getCompInt = &BufferAccessor::getT<int, short>;
                    break;
                    
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                    componentSize = 4;
                    componentNormalizer = 1.0 / float(0x7fffffff);
                    getCompFloat = &BufferAccessor::getT<float, long>;
                    getCompInt = &BufferAccessor::getT<int, long>;
                    break;
                    
                default:
                case TINYGLTF_COMPONENT_TYPE_FLOAT:
                    componentSize = 4;
                    componentNormalizer = 1.0;
                    getCompFloat = &BufferAccessor::getT<float, float>;
                    getCompInt = &BufferAccessor::getT<int, float>;
                    break;
            }
            
            if(byteStride == 0) {
                byteStride = componentSize * componentsInStruct;
            }
            rewind();
        }
        
        float getFloat(int totalComponentIndex) const {
            if(bufferPtr == nullptr) return 0;
            int structIndex = totalComponentIndex / componentsInStruct;
            int compIndex = totalComponentIndex - componentsInStruct * structIndex;
            return (this->*getCompFloat)(structIndex, compIndex) * componentNormalizer;
        }
        
        float getFloat(int structIndex, int componentIndex) const {
            if(bufferPtr == nullptr) return 0;
            return (this->*getCompFloat)(structIndex, componentIndex) * componentNormalizer;
        }
        
        int getInt(int structIndex, int componentIndex) const {
            if(bufferPtr == nullptr) return 0;
            return (this->*getCompInt)(structIndex, componentIndex);
        }
        
        int getStructCount() const {
            return structCount;
        }
        
        int getComponentCountInStruct() const {
            return componentsInStruct;
        }
        
        int getTotalComponentCount() const {
            return structCount * componentsInStruct;
        }
        
        void rewind() {
            curStruct = 0;
            curComponent = 0;
        }
        
        float readFloat() {
            float c = getFloat(curStruct, curComponent);
            updateReadPosition();
            return c;
        }
        
        int readInt() {
            int c = getInt(curStruct, curComponent);
            updateReadPosition();
            return c;
        }
        
    private:
        void updateReadPosition() {
            curComponent += 1;
            if(curComponent >= componentsInStruct) {
                curStruct += 1;
                curComponent = 0;
            }
        }
    };
    
    // Utility functions
    const tinygltf::Value FindPinkyPiExtra(const tinygltf::Value& val) {
        if(val.IsArray()) {
            size_t n = val.ArrayLen();
            for(size_t i = 0; i < n; i++) {
                auto v = FindPinkyPiExtra(val.Get(i));
                if(v.Type() != tinygltf::NULL_TYPE) {
                    return v;
                }
            }
            
        } else if(val.IsObject()) {
            auto keys = val.Keys();
            for(auto ite = keys.begin(); ite != keys.end(); ++ite) {
                auto k = *ite;
                if(k.compare(kExtraKey) == 0) {
                    return val.Get(kExtraKey);
                    
                } else {
                    auto v = val.Get(k);
                    if(v.IsObject() || v.IsArray()) {
                        return FindPinkyPiExtra(v);
                    }
                }
            }
        }
        
        return tinygltf::Value();
    }
    
    // Parse functions
    int ParseTextures(const tinygltf::Model& model, AssetLibrary *assetlib) {
        int count = 0;
        if(model.textures.size() <= 0) {
            return 0;
        }
        assetlib->textures.reserve(model.textures.size());
        
        for(auto ite = model.textures.begin(); ite != model.textures.end(); ++ite) {
            const tinygltf::Texture& gltftex = *ite;
            const tinygltf::Image& gltfimg = model.images.at(gltftex.source);
            
            double gamma = 2.2;
            auto ppextra = FindPinkyPiExtra(gltfimg.extras);
            if(ppextra.Type() != tinygltf::NULL_TYPE) {
                if(ppextra.Has("gamma")) {
                    auto val = ppextra.Get("gamma");
                    gamma = val.IsInt()? val.Get<int>() : val.Get<double>();
                }
            }
            
            ImageTexture *imgtex = new ImageTexture(gltfimg.width, gltfimg.height);
            imgtex->name = gltfimg.name;
            
            // Bitmap
            switch (gltfimg.bits) {
                case 8:
                    imgtex->initWith8BPPImage(gltfimg.image.data(), gltfimg.component, gamma);
                    break;
                case 16:
                    imgtex->initWith16BPPImage((const unsigned short*)gltfimg.image.data(), gltfimg.component, gamma);
                    break;
                default:
                    std::cerr << "texture:" << gltftex.name;
                    std::cerr << ", src:" << gltfimg.name << "(" << gltfimg.uri << ")";
                    std::cerr << " is unsupported bits per pixels value:" << gltfimg.bits;
                    std::cerr << std::endl;
                    
                    imgtex->fillColor(Color(1.0, 0.0, 1.0), 1.0, gamma);
                    break;
            }
            
            // Mag filter
            if(gltftex.sampler >= 0) {
                const tinygltf::Sampler& gltfsampler = model.samplers.at(gltftex.sampler);
                
                switch (gltfsampler.magFilter) {
                    case TINYGLTF_TEXTURE_FILTER_NEAREST:
                        imgtex->sampleType = ImageTexture::kNearest;
                        break;
                    case TINYGLTF_TEXTURE_FILTER_LINEAR:
                        imgtex->sampleType = ImageTexture::kLinear;
                        break;
                    default:
                        imgtex->sampleType = ImageTexture::kLinear;
                        break;
                }
                
                // TODO? Min filter is skip now
                
                // Wrap x
                switch (gltfsampler.wrapS) {
                    case TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE:
                        imgtex->wrapX = ImageTexture::kClamp;
                        break;
                    case TINYGLTF_TEXTURE_WRAP_REPEAT:
                        imgtex->wrapX = ImageTexture::kRepeat;
                        break;
                    default:
                        std::cerr << "texture:" << gltftex.name;
                        std::cerr << ", src:" << gltfimg.name << "(" << gltfimg.uri << ")";
                        std::cerr << " is using unsupported wrapS:" << gltfsampler.wrapS;
                        std::cerr << ". now using clamp.";
                        std::cerr << std::endl;
                        
                        imgtex->wrapX = ImageTexture::kClamp;
                        break;
                }
                
                // Wrap y
                switch (gltfsampler.wrapT) {
                    case TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE:
                        imgtex->wrapY = ImageTexture::kClamp;
                        break;
                    case TINYGLTF_TEXTURE_WRAP_REPEAT:
                        imgtex->wrapY = ImageTexture::kRepeat;
                        break;
                    default:
                        std::cerr << "texture:" << gltftex.name;
                        std::cerr << ", src:" << gltfimg.name << "(" << gltfimg.uri << ")";
                        std::cerr << " is using unsupported wrapT:" << gltfsampler.wrapT;
                        std::cerr << ". now using clamp.";
                        std::cerr << std::endl;
                        
                        imgtex->wrapY = ImageTexture::kClamp;
                        break;
                }
            }
            
            // Add
            assetlib->textures.push_back(std::shared_ptr<Texture>(imgtex));
            count += 1;
        }
        return count;
    }
    
    int ParseMaterials(const tinygltf::Model& model, AssetLibrary *assetlib) {
        int count = 0;
        if(model.materials.size() <= 0) {
            return 0;
        }
        assetlib->materials.reserve(model.materials.size());
        
        for(auto ite = model.materials.begin(); ite != model.materials.end(); ++ite) {
            const tinygltf::Material& gltfmat = *ite;
            
            Material *mat = new Material();
            mat->name = gltfmat.name;
            
            // "pbrMetallicRoughness"
            for(auto valite = gltfmat.values.begin(); valite != gltfmat.values.end(); ++valite) {
                auto kv = *valite;
                std::string key = kv.first;
                tinygltf::Parameter val = kv.second;

                std::cerr << key << " found" << std::endl;
                
                if(key.compare("baseColorTexture") == 0) {
                    mat->baseColorTexture.texCoord = val.TextureTexCoord();
                    mat->baseColorTexture.texture = assetlib->textures[val.TextureIndex()].get();
                    
                } else if(key.compare("baseColorFactor") == 0) {
                    auto col = val.ColorFactor();
                    mat->baseColorFactor = Color(col.at(0), col.at(1), col.at(2));
                    mat->baseColorAlpha = col.at(3);
                    
                } else if(key.compare("metallicRoughnessTexture") == 0) {
                    mat->metallicRoughnessTexture.texCoord = val.TextureTexCoord();
                    mat->metallicRoughnessTexture.texture = assetlib->textures[val.TextureIndex()].get();
                    
                } else if(key.compare("metallicFactor") == 0) {
                    mat->metallicFactor = val.number_value;
                    
                } else if(key.compare("roughnessFactor") == 0) {
                    mat->roughnessFactor = val.number_value;
                    
                }
            }

            // additional
            for(auto valite = gltfmat.additionalValues.begin(); valite != gltfmat.additionalValues.end(); ++valite) {
                auto kv = *valite;
                std::string key = kv.first;
                tinygltf::Parameter val = kv.second;

                std::cerr << key << " found" << std::endl;
                
                if(key.compare("normalTexture") == 0) {
                    mat->normalTexture.texCoord = val.TextureTexCoord();
                    mat->normalTexture.texture = assetlib->textures[val.TextureIndex()].get();
                    
                } else if(key.compare("occlusionTexture") == 0) {
                    mat->occlusionTexture.texCoord = val.TextureTexCoord();
                    mat->occlusionTexture.texture = assetlib->textures[val.TextureIndex()].get();
                    
                } else if(key.compare("emissiveTexture") == 0) {
                    mat->emissiveTexture.texCoord = val.TextureTexCoord();
                    mat->emissiveTexture.texture = assetlib->textures[val.TextureIndex()].get();
                    
                } else if(key.compare("emissiveFactor") == 0) {
                    auto col = val.ColorFactor();
                    mat->emissiveFactor = Color(col.at(0), col.at(1), col.at(2));
                    
                } else if(key.compare("alphaMode") == 0) {
                    auto amode = val.string_value;
                    if(amode.compare("OPAQUE") == 0) {
                        mat->alphaMode = Material::kAlphaAsOpaque;
                    } else if(amode.compare("MASK") == 0) {
                        mat->alphaMode = Material::kAlphaAsMask;
                    } else if(amode.compare("BLEND") == 0) {
                        mat->alphaMode = Material::kAlphaAsBlend;
                    } else {
                        std::cerr << "Unknown alphaMode:" << amode << std::endl;
                        mat->alphaMode = Material::kAlphaAsBlend;
                    }
                    
                } else if(key.compare("alphaCutoff") == 0) {
                    mat->alphaCutoff = val.number_value;
                    
                } else if(key.compare("doubleSided") == 0) {
                    mat->doubleSided = val.bool_value;
                }
            }
            
            // extra
            auto ppextra = FindPinkyPiExtra(gltfmat.extras);
            if(ppextra.Type() != tinygltf::NULL_TYPE) {
                if(ppextra.Has("ior")) {
                    mat->ior = ppextra.Get("ior").Get<PPFloat>();
                }
                // and more...
            }
            
            // Add
            assetlib->materials.push_back(std::shared_ptr<Material>(mat));
            count += 1;
        }
        return count;
    }
    
    int ParseMeshs(const tinygltf::Model& model, AssetLibrary *assetlib) {
        int count = 0;
        if(model.meshes.size() <= 0) {
            return 0;
        }
        assetlib->meshes.reserve(model.meshes.size());
        
        for(auto ite = model.meshes.begin(); ite != model.meshes.end(); ++ite) {
            auto gltfmesh = *ite;
            auto gltfprims = gltfmesh.primitives;
            
            if(gltfprims.size() <= 0) {
                continue;
            }
            
            auto mesh = new Mesh();
            mesh->clusters.reserve(gltfprims.size());
            
            for(auto prmite = gltfprims.begin(); prmite != gltfprims.end(); ++prmite) {
                auto gltfprimitive = *prmite;
                
                // Vertex
                int accid;
                
                accid = gltfprimitive.attributes.at("POSITION");
                auto posba = BufferAccessor(model.accessors[accid], model);
                
                accid = gltfprimitive.attributes.at("NORMAL");
                auto nrmba = BufferAccessor(model.accessors[accid], model);
                
                BufferAccessor tanba;
                if(gltfprimitive.attributes.find("TANGENT") != gltfprimitive.attributes.end()) {
                    accid = gltfprimitive.attributes.at("TANGENT");
                    tanba.init(model.accessors[accid], model);
                }
                
                BufferAccessor uv0ba;
                if(gltfprimitive.attributes.find("TEXCOORD_0") != gltfprimitive.attributes.end()) {
                    accid = gltfprimitive.attributes.at("TEXCOORD_0");
                    uv0ba.init(model.accessors[accid], model);
                }
                
                BufferAccessor uv1ba;
                if(gltfprimitive.attributes.find("TEXCOORD_1") != gltfprimitive.attributes.end()) {
                    accid = gltfprimitive.attributes.at("TEXCOORD_1");
                    uv1ba.init(model.accessors[accid], model);
                }
                
                BufferAccessor colba;
                if(gltfprimitive.attributes.find("COLOR") != gltfprimitive.attributes.end()) {
                    accid = gltfprimitive.attributes.at("COLOR");
                    colba.init(model.accessors[accid], model);
                }
                
                // JOINTS
                // WEIGHTS
                // TARGETS
                
                
                // Triangle
                BufferAccessor indba(model.accessors[gltfprimitive.indices], model);
                
                int numverts = posba.getStructCount();
                int numtris = indba.getStructCount() / 3;
                
                auto cluster = new Mesh::Cluster(numverts, numtris);
                
                for(int iv = 0; iv < numverts; iv++) {
                    Vector3 &v = cluster->vertices[iv];
                    v.x = posba.readFloat();
                    v.y = posba.readFloat();
                    v.z = posba.readFloat();
                    
                    Mesh::Attributes &attrs = cluster->attributes[iv];
                    attrs.normal.x = nrmba.readFloat();
                    attrs.normal.y = nrmba.readFloat();
                    attrs.normal.z = nrmba.readFloat();
                    
                    attrs.tangent.x = tanba.readFloat();
                    attrs.tangent.y = tanba.readFloat();
                    attrs.tangent.z = tanba.readFloat();
                    
                    attrs.uv0.x = uv0ba.readFloat();
                    attrs.uv0.y = uv0ba.readFloat();
                    attrs.uv0.z = 0.0;
                    
                    attrs.uv1.x = uv1ba.readFloat();
                    attrs.uv1.y = uv1ba.readFloat();
                    attrs.uv1.z = 0.0;
                    
                    attrs.color.x = colba.readFloat();
                    attrs.color.y = colba.readFloat();
                    attrs.color.z = colba.readFloat();
                    if(colba.getComponentCountInStruct() > 3) {
                        // Alpha
                        colba.readFloat();
                    }
                }
                
                // Triangle
                for(int itri = 0; itri < numtris; itri++) {
                    Mesh::Triangle& tri = cluster->triangles[itri];
                    tri.a = indba.readInt();
                    tri.b = indba.readInt();
                    tri.c = indba.readInt();
                }
                
                // Material
                cluster->material = assetlib->getMaterial(gltfprimitive.material);
                
                //
                auto sptr = std::shared_ptr<Mesh::Cluster>(cluster);
                mesh->clusters.push_back(sptr);
                if(cluster->material->emissiveFactor.getMaxComponent() > 0.0) {
                    mesh->emissiveClusters.push_back(sptr);
                }
            }
            
            // Add
            assetlib->meshes.push_back(std::shared_ptr<Mesh>(mesh));
            count += 1;
        }
        return count;
    }
    
    /*
    int ParseLights(const tinygltf::Model& model, AssetLibrary *assetlib) {
        int count = 0;
        for(auto ite = model.lights.begin(); ite != model.lights.end(); ++ite) {
            auto gltflight = *ite;
            
            auto lit = new Light();
            lit->name = gltflight.name;
            
            if(gltflight.color.size() > 0) {
                lit->color.set(gltflight.color[0], gltflight.color[1], gltflight.color[2]);
            }
            
            if (gltflight.type.compare("POINT") == 0) {
                lit->lightType = Light::kPointLight;
            }
            else if (gltflight.type.compare("SPOT") == 0) {
                lit->lightType = Light::kSpotLight;
            }
            else if (gltflight.type.compare("DIRECTIONAL") == 0) {
                lit->lightType = Light::kDirectionalLight;
            }
            
            assetlib->lights.push_back(std::shared_ptr<Light>(lit));
            count += 1;
        }
        return count;
    }
    */
    
    int ParseLights(const tinygltf::Model& model, AssetLibrary *assetlib) {
        int count = 0;
        if(model.extensions.find("KHR_lights_punctual") == model.extensions.end()) {
            return 0;
        }
        
        auto khr_lights = model.extensions.at("KHR_lights_punctual");
        if(!khr_lights.Has("lights")) {
            return 0;
        }
        auto lights = khr_lights.Get("lights");
        if(!lights.IsArray()) {
            return 0;
        }
        
        if(lights.ArrayLen() <= 0) {
            return 0;
        }
        assetlib->lights.reserve(lights.ArrayLen());
        
        for(int ilit = 0; ilit < lights.ArrayLen(); ilit++) {
            auto lit_punc = lights.Get(ilit);
            if(!lit_punc.IsObject()) {
                continue;
            }
            
            auto lit = new Light();
            
            if(lit_punc.Has("name")) {
                lit->name = lit_punc.Get("name").Get<std::string>();
            }
            
            if(lit_punc.Has("color")) {
                auto col = lit_punc.Get("color");
                if(col.IsArray()) {
                    auto r = col.Get(0);
                    auto g = col.Get(1);
                    auto b = col.Get(2);
                    lit->color.set(
                        r.IsInt()? r.Get<int>() : r.Get<double>(),
                        g.IsInt()? g.Get<int>() : g.Get<double>(),
                        b.IsInt()? b.Get<int>() : b.Get<double>()
                    );
                }
            }
            
            if(lit_punc.Has("intensity")) {
                auto val = lit_punc.Get("intensity");
                lit->intensity = val.IsInt()? val.Get<int>() : val.Get<double>();
            }
            
            if(lit_punc.Has("type")) {
                std::string littype = lit_punc.Get("type").Get<std::string>();
                if(littype.compare("point") == 0) {
                    lit->lightType = Light::kPointLight;
                    
                } else if(littype.compare("directional") == 0) {
                    lit->lightType = Light::kPointLight;
                    
                } else if(littype.compare("sopt") == 0) {
                    lit->lightType = Light::kSpotLight;
                    
                } else {
                    std::cerr << "unknown light type:" << littype << std::endl;
                }
            }
            
            if(lit_punc.Has("spot")) {
                auto spot = lit_punc.Get("spot");
                if(spot.IsObject()) {
                    if(spot.Has("innerConeAngle")) {
                        auto val = spot.Get("innerConeAngle");
                        lit->spot.innerConeAngle = val.IsInt()? val.Get<int>() : val.Get<double>();
                    }
                    if(spot.Has("outerConeAngle")) {
                        auto val = spot.Get("outerConeAngle");
                        lit->spot.innerConeAngle = val.IsInt()? val.Get<int>() : val.Get<double>();
                    }
                }
            }
            //
            assetlib->lights.push_back(std::shared_ptr<Light>(lit));
            count += 1;
        }
        return count;
    }
    
    int ParseCameras(const tinygltf::Model& model, AssetLibrary *assetlib) {
        int count = 0;
        if(model.cameras.size() <= 0) {
            return 0;
        }
        assetlib->cameras.reserve(model.cameras.size());
        
        for(auto ite = model.cameras.begin(); ite != model.cameras.end(); ++ite) {
            auto gltfcam = *ite;
            
            Camera *cam = new Camera();
            cam->name = gltfcam.name;
            
            if (gltfcam.type.compare("perspective") == 0) {
                cam->initWithType(Camera::kPerspectiveCamera);
                auto gltfpers = gltfcam.perspective;
                cam->perspective.aspect = gltfpers.aspectRatio;
                cam->perspective.yfov = gltfpers.yfov;
                cam->perspective.zfar = gltfpers.zfar;
                cam->perspective.znear = gltfpers.znear;
                
            } else if (gltfcam.type.compare("orthographic") == 0) {
                cam->initWithType(Camera::kOrthographicsCamera);
                auto gltfortho = cam->orthographics;
                cam->orthographics.xmag = gltfortho.xmag;
                cam->orthographics.ymag = gltfortho.ymag;
                cam->orthographics.zfar = gltfortho.zfar;
                cam->orthographics.znear = gltfortho.znear;
                
            } else {
                std::cerr << "Unknown type camera found. skip:" << gltfcam.type << std::endl;
                continue;
            }
            
            auto ppextra = FindPinkyPiExtra(gltfcam.extras);
            if(ppextra.Type() != tinygltf::NULL_TYPE) {
                if(ppextra.Has("focalLength")) {
                    cam->focalLength = ppextra.Get("focalLength").Get<PPFloat>();
                }
                if(ppextra.Has("fNumber")) {
                    cam->fNumber = ppextra.Get("fNumber").Get<PPFloat>();
                }
                if(ppextra.Has("focusDistance")) {
                    cam->focusDistance = ppextra.Get("focusDistance").Get<PPFloat>();
                }
            }
            
            //
            assetlib->cameras.push_back(std::shared_ptr<Camera>(cam));
            count += 1;
        }
        return count;
    }
    
    int ParseNodes(const tinygltf::Model& model, AssetLibrary *assetlib) {
        int count = 0;
        if(model.nodes.size() <= 0) {
            return 0;
        }
        assetlib->nodes.reserve(model.nodes.size());
        
        for(auto ite = model.nodes.begin(); ite != model.nodes.end(); ++ite) {
            auto gltfnode = *ite;
            
            // get transform
            Matrix4 transmat;
            
            if(gltfnode.translation.size() > 0) {
                transmat.translate(
                    gltfnode.translation[0],
                    gltfnode.translation[1],
                    gltfnode.translation[2]
                );
            }
            
            if(gltfnode.rotation.size() > 0) {
                Quaterion rotation(gltfnode.rotation[0], gltfnode.rotation[1], gltfnode.rotation[2], gltfnode.rotation[3]);
                transmat = transmat * rotation.getMatrix();
            }
            
            if(gltfnode.scale.size() > 0) {
                transmat.scale(
                    gltfnode.scale[0],
                    gltfnode.scale[1],
                    gltfnode.scale[2]
                );
            }
            
            if(gltfnode.matrix.size() > 0) {
                auto mat = gltfnode.matrix;
                transmat.set(
                    mat[0], mat[1], mat[2], mat[3],
                    mat[4], mat[5], mat[6], mat[7],
                    mat[8], mat[9], mat[10], mat[11],
                    mat[12], mat[13], mat[14], mat[15]
                );
            }
            
            auto node = new Node();
            node->name = gltfnode.name;
            node->transform = transmat;
            
            if(gltfnode.camera >= 0) {
                node->contentType = Node::kContentTypeCamera;
                node->camera = assetlib->cameras[gltfnode.camera].get();
                
            } else if(gltfnode.mesh >= 0) {
                node->contentType = Node::kContentTypeMesh;
                node->mesh = assetlib->meshes[gltfnode.mesh].get();
            }
            
            size_t numchild = gltfnode.children.size();
            if(numchild > 0) {
                node->children.resize(numchild);
                for(int ichld = 0; ichld < numchild; ichld++) {
                    node->children[ichld] = gltfnode.children[ichld];
                }
            }
            
            auto extensions = gltfnode.extensions;
            if(extensions.find("KHR_lights_punctual") != extensions.end()) {
                auto lit_punk = extensions.at("KHR_lights_punctual");
                if(lit_punk.IsObject()) {
                    auto litid = lit_punk.Get("light");
                    if(litid.IsInt()) {
                        node->light = assetlib->lights[litid.Get<int>()].get();
                        node->contentType = Node::kContentTypeLight;
                    }
                }
            }
            
            //
            assetlib->nodes.push_back(std::shared_ptr<Node>(node));
            
            count += 1;
        }
        return count;
    }
    
    int ParseScenes(const tinygltf::Model& model, AssetLibrary *assetlib) {
        int count = 0;
        if(model.scenes.size() <= 0) {
            return 0;
        }
        assetlib->scenes.reserve(model.scenes.size());
        
        for(auto ite = model.scenes.begin(); ite != model.scenes.end(); ++ite) {
            auto gltfscene = *ite;
            
            auto scn = new Scene();
            if(gltfscene.nodes.size() > 0) {
                scn->nodes.reserve(gltfscene.nodes.size());
                for(auto nodeite = gltfscene.nodes.begin(); nodeite != gltfscene.nodes.end(); ++nodeite) {
                    int nodeid = *nodeite;
                    auto node = assetlib->nodes[nodeid].get();
                    scn->nodes.push_back(node);
                    switch (node->contentType) {
                        case Node::kContentTypeMesh:
                            scn->meshes.push_back(node->mesh);
                            break;
                        case Node::kContentTypeLight:
                            scn->lights.push_back(node->light);
                            break;
                        case Node::kContentTypeCamera:
                            scn->cameras.push_back(node->camera);
                            break;
                        case Node::kContentTypeEmpty:
                            // empty node
                            break;
                        default:
                            std::cerr << node->name << " has unknown content type found:" << node->contentType << std::endl;
                            break;
                    }
                }
            }
            
            assetlib->scenes.push_back(std::shared_ptr<Scene>(scn));
            count += 1;
        }
        return count;
    }
}

Scene* SceneLoader::load(std::string filepath) {
    tinygltf::TinyGLTF gltfloader;
    
    tinygltf::Model model;
    std::string err;
    std::string warn;
    bool loadres;
    
    loadres = gltfloader.LoadASCIIFromFile(&model, &err, &warn, filepath);
    
    if(!loadres) {
        std::cerr << "glTF load err:" << err << std::endl;
        return nullptr;
    }
    
    AssetLibrary *assetLib = new AssetLibrary();
    
    ParseTextures(model, assetLib);
    ParseMaterials(model, assetLib);
    ParseMeshs(model, assetLib);
    ParseLights(model, assetLib);
    ParseCameras(model, assetLib);
    ParseNodes(model, assetLib);
    ParseScenes(model, assetLib);
    
    assetLib->defaultSceneId = model.defaultScene;
    
    return assetLib->getDefaultScene();
}
