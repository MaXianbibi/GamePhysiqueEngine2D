#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

# include "../../../SDL/include/SDL.h"
# include "../Vec2/Vec2.hpp"
# include "iostream"
# include <vector>


#include <map>
#include <algorithm>


# define DEFAULT_HEIGHT  1080  
# define DEFAULT_WIDTH   1920 
# define DEFAULT_NAME    "Graphicsdow"

void fatal(std::string msg);
void fatal_SDL(std::string msg);

class Graphics
{
private:
    SDL_Window      *win;
    SDL_Renderer    *renderer;
    SDL_Texture     *texture;
    u_int32_t       n_pixel;
    u_int32_t       *colorBuffer;
    u_int16_t       w;
    u_int16_t       h;
    u_int8_t        FRAME_RATE;
    bool            is_running;
    void            (Graphics::*defaultEventFunction)(void);
    void            render(void);
    void            defaultEvent(void);

public:
    Graphics();
    Graphics(u_int16_t _w, u_int16_t _h, std::string name);
    ~Graphics();
    int            (*eventsFunc)(Graphics &graphics);
    int            (*updateFunc)(Graphics &graphics, void *ptr);
    void           loop(void *ptr);
    void           change_frame_rate(u_int8_t v);
    void           stop_running(void); 
    void           drawPixel(u_int32_t y, u_int32_t x, u_int32_t color);
    void           clear_color_buffer(u_int32_t color);
    void           draw_grid(void);
    void           drawRecrangle(u_int32_t posX, u_int32_t posY, u_int32_t w, u_int32_t h, u_int32_t color);
    void           drawLine(int32_t start_x, int32_t finish_x, int32_t start_y, int32_t finish_y, uint32_t color);
    void          drawLine(Vec2<float> start, Vec2<float> finish, uint32_t color);
    void          DrawPolygon(int32_t x, int32_t y, const std::vector<Vec2<float>> &vertices, uint32_t color);
    void          DrawFilledPolygon(int32_t x, int32_t y, const std::vector<Vec2<float>> &vertices, uint32_t color);
    void          DrawCircle(int32_t x, int32_t y, int32_t radius, uint32_t color);
    void          DrawFilledCircle(int32_t x, int32_t y, int32_t radius, uint32_t color);
};
#endif