#ifndef _MODULE_DEBUGDRAW_H_
#define _MODULE_DEBUGDRAW_H_

#include "Module.h"

namespace math
{
    class float4x4;
}

class DDRenderInterfaceCoreGL;
class Camera;

class ModuleDebugDraw : public Module
{

public:

    ModuleDebugDraw();
    ~ModuleDebugDraw();

	bool            Init();
	update_status   Update();
	bool            CleanUp();

    void            Draw(const math::float4x4& view, const math::float4x4& proj, unsigned width, unsigned height);
private:

    static DDRenderInterfaceCoreGL* implementation;
};

#endif /* _MODULE_DEBUGDRAW_H_ */
