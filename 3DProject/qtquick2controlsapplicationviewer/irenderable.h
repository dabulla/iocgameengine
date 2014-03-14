#ifndef IRENDERABLE_H
#define IRENDERABLE_H

#include <QOpenGLContext>

class IMaterial;

class IRenderable
{
public:
    IRenderable();
    void Render(QOpenGLContext*);
    void SetMaterial(IMaterial&);
    IMaterial& GetMaterial();
};

#endif // IRENDERABLE_H
