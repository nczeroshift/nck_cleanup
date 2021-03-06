
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#ifndef NCK_PROCESSOR_H
#define NCK_PROCESSOR_H

#include "nckSceneConfig.h"
#include "nckGeometry.h"
#include "nckTexture.h"
#include "nckMaterial.h"
#include "nckObject.h"
#include "nckCamera.h"
#include "nckLamp.h"
#include "nckArmature.h"

_SCENE_BEGIN

class Compound;
class Processor {
public:
    virtual ~Processor() {}
    virtual BXON::Object * HandleData(BXON::Object * obj) { return obj; };
    virtual void HandleFinish(BXON::Map * map, Scene::Compound * compound) { };
    virtual Geometry::Mesh* HandleGeometry(Geometry::Mesh * mesh) { return mesh; };
    virtual Scene::Material * HandleMaterial(Scene::Material * mat) { return mat; };
    virtual Scene::Texture * HandleTexture(Scene::Texture * tex) { return tex; };
    virtual Scene::Camera * HandleCamera(Scene::Camera * cam) { return cam; };
    virtual Scene::Object * HandleObject(Scene::Object * obj) { return obj; };
    virtual Scene::Lamp * HandleLamp(Scene::Lamp * lamp) { return lamp; };
    virtual Scene::Armature * HandleArmature(Scene::Armature * arm) { return arm; };
};

_SCENE_END

#endif
