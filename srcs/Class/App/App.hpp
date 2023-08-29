#ifndef APP_HPP
# define APP_HPP

#include "../Graphics/Graphics.hpp"

class App
{
private:
    bool running = false;
    Graphics *win = nullptr;
public:
    App() = default;
    ~App() = default;

    bool isRunning() const { return running; }
    void Setup();
    void Update();
    void Destroy();



    Graphics *getGraphics() const { return win; }
    void    setGraphics(Graphics *newWin) { if (this->win) delete win; win = newWin; }



};




#endif