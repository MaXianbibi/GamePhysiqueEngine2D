#include "Include/Include.hpp"

int doingnothing(Graphics &win, void *ptr)
{
    static int32_t i = -200;
    static int32_t x = 5;
    (void)ptr;
    win.clear_color_buffer(0);
    std::vector<Vec2<float>> vertices = {
        Vec2<float>(0, 0),
        Vec2<float>(0, 100),
        Vec2<float>(100, 100),
        Vec2<float>(200, 200),
        Vec2<float>(100, 0)
    };
    // win.DrawPolygon()
    win.DrawFilledPolygon(100, 100, vertices, 0xff0000);
    win.DrawPolygon(100, 100, vertices, 0x00ff00);
    win.DrawCircle(100, 100, 50, 0x0000ff);
    win.DrawFilledCircle(300, 300, 50, 0x0000ff);

    i += x;
    if (i > 1000)
    {
        i = -200;
        x *= 1.5;
    }
    return (1);
}

int main(void)
{  
    App app;

    app.Setup();
    app.Update();

    return (0);
}