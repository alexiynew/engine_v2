#pragma once

namespace game_engine
{

class IRenderContext
{
public:
    virtual ~IRenderContext() = default;

    virtual void makeCurrent() const = 0;
    virtual void dropCurrent() const = 0;
    virtual void swapBuffers() const = 0;
};

} // namespace game_engine
