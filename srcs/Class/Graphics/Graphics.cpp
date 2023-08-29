#include "Graphics.hpp"

void fatal(std::string msg)
{
    perror(msg.c_str());
    exit( 1 );
}

void fatal_SDL(std::string msg)
{
    std::cerr << msg << std::endl;
    SDL_GetError();
    exit( 1 );
}

Graphics::Graphics(u_int16_t _w, u_int16_t _h, std::string name) : w(_w), h(_h)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        fatal_SDL("SDL_INIT");
    SDL_DisplayMode display_mode;
    if (SDL_GetCurrentDisplayMode(0, &display_mode) < 0)
        fatal_SDL("SDL_GetCurrentDisplayMode");
    this->win = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->w, this->h, SDL_WINDOW_SHOWN);
    if (!this->win)
        fatal_SDL("SDL_CreateWindow");
    this->colorBuffer = (u_int32_t *)calloc(sizeof(u_int32_t), this->h * this->w);
    if (!this->colorBuffer)
        fatal("malloc");
    memset(this->colorBuffer, 0, this->h * this->w);
    this->renderer = SDL_CreateRenderer(this->win, -1, 0);
    if (!this->renderer)
        fatal_SDL("SDL_CreateRenderer");
    this->texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, this->w, this->h);
    if (!this->texture)
        fatal_SDL("SDL_CreateTexture");
        
    this->is_running = 1;

    this->defaultEventFunction = &Graphics::defaultEvent;
    this->eventsFunc = NULL;
    this->updateFunc = NULL;
    this->FRAME_RATE = 1000 / 60;
    this->n_pixel = this->w * this->h;
}

Graphics::Graphics() : Graphics(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_NAME)
{
}

Graphics::~Graphics()
{
    free(this->colorBuffer);
    SDL_DestroyTexture(this->texture);
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->win);
    SDL_Quit();
}

void Graphics::defaultEvent(void)
{
    SDL_Event event;

    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        this->is_running = 0;
        break;
    case SDL_KEYDOWN:
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            this->is_running = 0;
            break;
        }
    }
    break;
    }
}

void Graphics::loop(void *ptr)
{
    while (this->is_running)
    {
        if (!this->updateFunc)
            fatal("No update Function !");
        (this->*defaultEventFunction)();
        if (eventsFunc)
            (this->eventsFunc)(*this);
        (this->updateFunc)(*this, ptr);

        render();        
        SDL_Delay(this->FRAME_RATE);
    }
}

void           Graphics::change_frame_rate(u_int8_t v)
{
    this->FRAME_RATE = v;
}

void            Graphics::render(void)
{
    if (SDL_UpdateTexture(this->texture, NULL, this->colorBuffer, this->w * sizeof(u_int32_t)) < 0)
        fatal_SDL("SDL_UpdateTexture");
    if (SDL_RenderCopy(this->renderer, this->texture, NULL, NULL) < 0)
        fatal_SDL("SDL_RenderCopy");

    SDL_RenderPresent(this->renderer);
}

void Graphics::stop_running(void)
{
    this->is_running = 0;
}



void Graphics::drawPixel(u_int32_t y, u_int32_t x, u_int32_t color)
{
    if (y >= this->h || x >= this->w)
        return ;
    this->colorBuffer[y * this->w + x] = color;
}

void           Graphics::clear_color_buffer(u_int32_t color)
{
    for (u_int32_t i = 0; i < this->n_pixel; i++)
        this->colorBuffer[i] = color;
}

void Graphics::draw_grid(void)
{
    for (u_int32_t i = 0; i < this->h; i++)
        for (uint32_t j = 0; j < this->w; j++)
            if (!(i % 10) || !(j % 10))
                drawPixel(i, j, 0x101010);
}

void Graphics::drawRecrangle(u_int32_t posX, u_int32_t posY, u_int32_t w, u_int32_t h, u_int32_t color)
{
    for (u_int32_t i = 0; i < h; i++)
        for (uint32_t j = 0; j < w; j++)
            drawPixel(i + posY, j + posX, color);
}

void           Graphics::drawLine(int32_t start_x, int32_t finish_x, int32_t start_y, int32_t finish_y, uint32_t color)
{
        
    int32_t dx = std::abs(finish_x - start_x);
    int32_t dy = std::abs(finish_y - start_y);

    int32_t sx = start_x < finish_x ? 1 : -1;
    int32_t sy = start_y < finish_y ? 1 : -1;

    int32_t err = (dx > dy ? dx : -dy) / 2;
    int32_t e2;


    while (true) {
        this->drawPixel(start_y, start_x, color);

        if (start_x == finish_x && start_y == finish_y) {
            break;
        }

        e2 = err;

        if (e2 > -dx) {
            err -= dy;
            start_x += sx;
        }

        if (e2 < dy) {
            err += dx;
            start_y += sy;
        }
    }
}

void          Graphics::drawLine(Vec2<float> start, Vec2<float> finish, uint32_t color)
{
    drawLine(start.getX(), finish.getX(), start.getY(), finish.getY(), color);
}

void         Graphics::DrawPolygon(int32_t x, int32_t y, const std::vector<Vec2<float>> &vertices, uint32_t color)
{
    for (size_t i = 0; i < vertices.size() - 1; i++)
        drawLine(vertices[i].getX() + x, vertices[i + 1].getX() + x, vertices[i].getY() + y, vertices[i + 1].getY() + y, color);
    drawLine(vertices[0].getX() + x, vertices[vertices.size() - 1].getX() + x, vertices[0].getY() + y, vertices[vertices.size() - 1].getY() + y, color);
}

// this code is not mine 
 void Graphics::DrawFilledPolygon(int32_t x, int32_t y, const std::vector<Vec2<float>>& vertices, uint32_t color) {
    if (vertices.size() < 3)
        return;
    struct Edge {
        float x, dx;
        int yMax;

        Edge(float x, float dx, int yMax) : x(x), dx(dx), yMax(yMax) {}
    };
    std::vector<Edge> AET;
    std::map<int, std::vector<Edge>> GET;
    for (size_t i = 0, j = vertices.size() - 1; i < vertices.size(); j = i++) {
        float x1 = vertices[i].getX(), y1 = vertices[i].getY();
        float x2 = vertices[j].getX(), y2 = vertices[j].getY();
        if (y1 == y2)
            continue;
        if (y1 > y2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        float dx = (x2 - x1) / (y2 - y1);
        GET[(int)y1].push_back(Edge(x1, dx, (int)y2));
    }
    for (int scanY = GET.begin()->first; !GET.empty() || !AET.empty(); scanY++) {
        if (GET.count(scanY)) {
            for (const auto& edge : GET[scanY])
                AET.push_back(edge);
            GET.erase(scanY);
        }
        std::sort(AET.begin(), AET.end(), [](const Edge& a, const Edge& b) { return a.x < b.x; });
        for (size_t i = 0; i < AET.size(); i += 2) {
            drawLine((int)std::round(AET[i].x) + x, (int)std::round(AET[i + 1].x) + x, scanY + y, scanY + y, color);
        }
        AET.erase(std::remove_if(AET.begin(), AET.end(), [scanY](const Edge& edge) { return edge.yMax <= scanY; }), AET.end());
        for (auto& edge : AET)
            edge.x += edge.dx;
    }
}


// L'algorithme de Bresenham
void          Graphics::DrawCircle(int32_t x, int32_t y, int32_t radius, uint32_t color)
{
    int32_t x0 = 0;
    int32_t y0 = radius;
    int32_t d = 3 - 2 * radius;
    while (y0 >= x0) {
        drawPixel(x + x0, y + y0, color);
        drawPixel(x + y0, y + x0, color);
        drawPixel(x - x0, y + y0, color);
        drawPixel(x - y0, y + x0, color);
        drawPixel(x + x0, y - y0, color);
        drawPixel(x + y0, y - x0, color);
        drawPixel(x - x0, y - y0, color);
        drawPixel(x - y0, y - x0, color);
        if (d < 0) {
            d += 4 * x0++ + 6;
        }
        else {
            d += 4 * (x0++ - y0--) + 10;
        }
    }
}

void          Graphics::DrawFilledCircle(int32_t x, int32_t y, int32_t radius, uint32_t color)
{
    int32_t x0 = 0;
    int32_t y0 = radius;
    int32_t d = 3 - 2 * radius;
    while (y0 >= x0) {
        drawLine(x - x0, x + x0, y + y0, y + y0, color);
        drawLine(x - y0, x + y0, y + x0, y + x0, color);
        drawLine(x - x0, x + x0, y - y0, y - y0, color);
        drawLine(x - y0, x + y0, y - x0, y - x0, color);
        if (d < 0) {
            d += 4 * x0++ + 6;
        }
        else {
            d += 4 * (x0++ - y0--) + 10;
        }
    }
}
