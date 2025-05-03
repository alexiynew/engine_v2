#pragma once

namespace game_engine
{

class RenderContext
{
public:
    virtual ~RenderContext() = default;

    virtual void makeCurrent() = 0;
    virtual void dropCurrent() = 0;
    virtual void swapBuffers() = 0;
};

} // namespace game_engine
