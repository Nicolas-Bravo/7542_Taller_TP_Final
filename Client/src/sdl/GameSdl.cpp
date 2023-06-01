#include "GameSdl.h"
#include "HandleEvent.h"
#include "Defines.h"
#include "mapSdl.h"
#include <stdexcept>
#include <iostream>

GameSdl::GameSdl(int id, Queue<Snapshot*>& snapshotQueue,
    Queue<EventDTO*>& eventQueue, bool& endGame) : idOperator(id),
    snapshotQueue(snapshotQueue), eventQueue(eventQueue), endGame(endGame) {

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        throw std::runtime_error("Failed to init SDL");
    
    if(IMG_Init(IMG_INIT_PNG) < 0)
        throw std::runtime_error("Failed to init IMG_Init");
    
    this->window = SDL_CreateWindow("Left 4 Dead", 
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    WINDOW_WIDTH,
                                    WINDOW_HEIGTH,
                                    SDL_WINDOW_SHOWN);
    if (!this->window)
        throw std::runtime_error("Failed to create a window");
}

void GameSdl::run() {
    EventHandler event(this->eventQueue);
    Renderer render(window, -1, SDL_RENDERER_ACCELERATED);
    Operator soldier(idOperator, render);
    MapSdl map(0, render);
    eventQueue.push(new EventDTO(Event::event_start_game));
    while (event.isRunning()) { 
        uint32_t frameInit = SDL_GetTicks();

        render.clear();
        SDL_PumpEvents();
        event.listen();
        // this.update();
        // soldier.update(event.getMoveDirection());
        Snapshot* snap = snapshotQueue.pop();
        // std::cout << snap->getPositions()[TypeOperator(1)].first << std::endl;
        std::map<TypeOperator, std::pair<uint16_t, uint16_t>> posi = snap->getPositions();
        // std::cout << (int)posi.size() << std::endl;
        // if (posi[soldier.getType()].first) {
                soldier.updatePosition(posi[soldier.getType()]);
                // std::cout << "Actualizo la posicion\n";
        // }
        map.render();
        soldier.render();
        render.present();

        uint32_t frameEnd = SDL_GetTicks();
        uint32_t processTime = frameEnd - frameInit;

        if (1000 / 20 > processTime)
            SDL_Delay(1000 / 20 - processTime);
    }
    this->endGame = true;
    eventQueue.close();
}

GameSdl::~GameSdl() {
    SDL_DestroyWindow(this->window);
    SDL_Quit();
    IMG_Quit();
}

