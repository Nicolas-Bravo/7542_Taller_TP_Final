#include "Game.h"
#include <memory>
#include <string>
#include <random>
#include <functional>
#include <iterator>
#include <utility>

Game::Game(const uint32_t id, std::string name, const TypeGame& type) :
    id(id), name(std::move(name)), mutex(),
    unprocessed_events(1000), client_snapshot_queues(),
    talking(true), alive(true), gameWorld(type, this->generateMapType()), started(false) {}

void Game::run() {
    try {
        while(talking) {
            gameLoop();
        }
    } catch(std::exception& e) {
        // hacer algo
    }
    alive = false;
}

void Game::stop() {
    talking = false;
    alive = false;
}

bool Game::ended() {
    return !alive;
}

Queue<std::shared_ptr<EventDTO>>* Game::createGame(Queue<std::shared_ptr<Snapshot>> *q,
                                                   const TypeOperator& op) {
    std::lock_guard<std::mutex> locker(mutex);
    this->generateMapType();
    uint8_t idPlayer = gameWorld.addPlayer(op);
    client_snapshot_queues.insert({idPlayer, q});
    q->push(std::make_shared<Snapshot> (Event::event_create, id, idPlayer));
    return &this->unprocessed_events;
}

Queue<std::shared_ptr<EventDTO>>* Game::joinGame(Queue<std::shared_ptr<Snapshot>> *q,
                                                 const TypeOperator& op) {
    if (not started) {                   
        std::lock_guard<std::mutex> locker(mutex);
        uint8_t idPlayer = gameWorld.addPlayer(op);
        client_snapshot_queues.insert({idPlayer, q});
        std::cout << (int)idPlayer << std::endl;
        // Notify all clients that a new player joined
        for (auto &clientQueue : client_snapshot_queues) {
            clientQueue.second->push(std::make_shared<Snapshot>(Event::event_join,
                                                        (uint8_t)0x00,
                                                        idPlayer, client_snapshot_queues.size()));
        }
        
        return &this->unprocessed_events;
    }
    // JOIN FAILED
    q->push(std::make_shared<Snapshot>(Event::event_join, (uint8_t)0x01, 0, 0));
    return nullptr;
}

void Game::startGame() {
    this->started = true;
    std::shared_ptr<Snapshot> snapshot = gameWorld.getSnapshot(true);
    broadcastSnapshot(snapshot);
    this->start();
} 

void Game::gameLoop() {
    // using namespace std::chrono;
    while (alive) {
        auto begin = std::chrono::steady_clock::now();

        //process_events() // popea hasta que no haya más eventos en la queue
        // y los procesa en el gameworld.
        processEvents();

        // TODO:
        //  step(some_time)
        gameWorld.simulateStep();
        // simula un tiempito en el 'gameworld'.
        // Este tiempito lo que va a hacer es hacer que falte menos
        // para poder lanzar una grandada, que los jugadores se muevan,
        // todos los eventos que tienen que ver con el tiempo.

        std::shared_ptr<Snapshot> snapshot = gameWorld.getSnapshot(true);
        // broadcastSnapshot() # acá recien se agarra el snapshot y se lo pushea
        // a los hilos sender. Un snapshot por gameloop. Si hacen uno por evento,
        // saturan la red sin sentido
        broadcastSnapshot(snapshot);

        // ->>Bueno y el sleep de tiempo variable también
        auto end = std::chrono::steady_clock::now();
        int t_delta = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        // duration<double>(t_delta)
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 20 - t_delta));
    }
}

void Game::processEvents() {
    std::shared_ptr<EventDTO> event = nullptr;
    int iterations = 0;
    try {
        // try_pop es no bloqueante y devuelve false en caso de que la queue este vacia
        while (unprocessed_events.try_pop(event) && iterations < 10) {
            if (event->getEvent() == Event::event_move
                    or event->getEvent() == Event::event_stop_move) {
                gameWorld.updateMovementDirection(event->getEvent(),
                                                  event->getIdPlayer(),
                                                  event->getMoveTo());
            } else if (event->getEvent() == Event::event_shoot
                        or event->getEvent() == Event::event_stop_shoot) {
                std::cout << "Llego un disparo\n";
                gameWorld.updateShootingState(event->getEvent(),
                                              event->getIdPlayer());
            } else if (event->getEvent() == Event::event_leave) {
                std::cout << "se desconecto cliente\n";
                std::lock_guard<std::mutex> l(mutex);
                gameWorld.deletePlayer(event->getIdPlayer());
                client_snapshot_queues.erase(event->getIdPlayer());
            }
            iterations++;
        }
    } catch(ClosedQueue& e) {
            // hacer algo
    }
}

void Game::broadcastSnapshot(std::shared_ptr<Snapshot> snapshot) {
    std::lock_guard<std::mutex> l(mutex);
    for (auto const& i : this->client_snapshot_queues) {
        i.second->push(snapshot);
    }
}

int Game::generateMapType() {
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int> distr(1, 3);
   return distr(generator);
}

Game::~Game() {
    join();
}