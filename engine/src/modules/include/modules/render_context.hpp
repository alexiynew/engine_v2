#pragma once

namespace game_engine
{

class IRenderContext
{
public:
    virtual ~IRenderContext() = default;

    virtual void MakeCurrent() const = 0;
    virtual void DropCurrent() const = 0;
    virtual void SwapBuffers() const = 0;
};

} // namespace game_engine
