#pragma once

namespace game_engine
{

class RenderContext
{
public:
    virtual ~RenderContext() = default;

    virtual void makeCurrent() const = 0;
    virtual void dropCurrent() const = 0;
    virtual void swapBuffers() const = 0;
};

} // namespace game_engine
