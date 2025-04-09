#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "../common.h"
#include "../texture.h"

using glm::vec1;
using glm::vec2;
using glm::vec3;
using glm::vec4;

CLASS_PTR(UGameObject)
class UGameObject
{

private:
    vec3 Position;
    TextureUPtr m_texture;

    // material parameter
    struct Material 
    {
        TextureUPtr diffuse;
        TextureUPtr specular;
        float shininess { 32.0f };
    };
};


#endif // __GAMEOBJECT_H__