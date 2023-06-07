#ifndef SERVER_GAME_H_
#define SERVER_GAME_H_

#include <list>
#include <mutex>
#include <string>
#include "EventDTO.h"
#include "Snapshot.h"
#include "Queue.h"
#include "Thread.h"
#include "GameWorld.h"
#include <atomic>
#include <memory>

/*
 * TDA Game
 */
class Game : public Thread {
    private:
        const uint32_t id;
        const std::string name;
        std::mutex mutex;
        Queue<std::shared_ptr<EventDTO>> unprocessed_events;
        std::list<Queue<std::shared_ptr<Snapshot>>*> client_snapshot_queues;
        std::atomic<bool> talking;
        std::atomic<bool> alive;
        GameWorld gameWorld;
        bool started;
        int map;

    public:
        explicit Game(const uint32_t id, const std::string& name, const TypeGame& type);

        virtual void run() override;

        /*
        * Método para detener la ejecución el hilo.
        */
        virtual void stop() override;

        /*
        * Método que devuelve true cuando el hilo termino de ejecutarse.
        */
        bool ended();

        /*
         * Este método debe añadir una nueva queue a la lista
         * de forma atomica y pushear el snapshot de create
         */
        Queue<std::shared_ptr<EventDTO>>* createGame(Queue<std::shared_ptr<Snapshot>> *q, const TypeOperator& op);

        /*
         * Este método debe añadir una nueva queue a la lista
         * de forma atomica y pushear el snapshot de join
         */
        Queue<std::shared_ptr<EventDTO>>* joinGame(Queue<std::shared_ptr<Snapshot>> *q, const TypeOperator& op);

        /*
         * Este método setea a true el atributo started que sirve para
         * que posteriormente no se puedan unir clientes una vez que comenzo
         * el juego, a su vez hace el start del hilo.
         */
        void startGame();

        /*
         * No queremos permitir que alguien haga copias
         * */
         Game(const Game&) = delete;
         Game& operator=(const Game&) = delete;
         ~Game();

    private:
        void gameLoop();
        void processEvents();
        void broadcastSnapshot(std::shared_ptr<Snapshot> snapshot);
        void generateMapType();
};

#endif  // SERVER_GAME_H_
