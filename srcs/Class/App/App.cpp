#include "App.hpp"

int doingnothing(Graphics &win, void *ptr);

void App::Setup()
{
    this->win = new Graphics(720, 480, "Watanabe Engine");
    win->eventsFunc = nullptr;
    win->updateFunc = doingnothing;
    running = true;
}

void App::Update()
{
    this->win->loop(nullptr);
}


void App::Destroy()
{
    delete this->win;
    this->win = nullptr;
    running = false;
}

