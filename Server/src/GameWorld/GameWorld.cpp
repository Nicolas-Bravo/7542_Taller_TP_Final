#include "GameWorld.h"
#include <random>
#include <functional>
#include <iterator>
#include <utility>
#include <algorithm>

GameWorld::GameWorld(const TypeGame& type, TypeDifficulty difficulty) :
    players_amount(INITIAL_PLAYERS_AMOUNT), players(), type(type), map(this->generateMapType()),
    collidables(), infectedId(INITIAL_INFECTED_ID), obsacleId(INITIAL_OBSTACLE_ID),
    infectedFactory(), RC(), ended(false), obstacleFactory(), playerFactory(),
    deadPlayersId(), difficulty(difficulty) {
    this->generateInfecteds();
    this->generateObstacles();
}

uint8_t GameWorld::addPlayer(TypeOperator op) {
    std::shared_ptr<Player> newPlayer = playerFactory.getPlayer(op,
                                                                players_amount,
                                                                collidables,
                                                                RC);
    this->players.insert({players_amount, newPlayer});
    return players_amount++;
}

void GameWorld::deletePlayer(uint8_t id) {
    this->players.erase(id);
    this->collidables.erase(id);
    this->deadPlayersId.push_back(id);
}

void GameWorld::updateMovementDirection(Event event, uint8_t id, MoveTo direction) {
    bool found = (std::find(deadPlayersId.begin(),
                            deadPlayersId.end(),
                            id) != deadPlayersId.end());
    // Ignore actions from dead players
    if (found) {
        return;
    }

    if (event == Event::event_move) {
        players.at(id)->setMovementDirection(direction);
    } else {
        players.at(id)->stopMovementDirection(direction);
    }
}

void GameWorld::updateShootingState(Event event, uint8_t id) {
    bool found = (std::find(deadPlayersId.begin(),
                            deadPlayersId.end(),
                            id) != deadPlayersId.end());
    // Ignore actions from dead players
    if (found) {
        return;
    }

    if (event == Event::event_shoot) {
        players.at(id)->setShootingState();
    } else {
        players.at(id)->stopShootingState();
    }
}

void GameWorld::updateReloadingState(Event event, uint8_t id) {
    bool found = (std::find(deadPlayersId.begin(),
                            deadPlayersId.end(),
                            id) != deadPlayersId.end());
    // Ignore actions from dead players
    if (found) {
        return;
    }

    if (event == Event::event_recharge) {
        players.at(id)->setReloadingState();
    }
}

void GameWorld::updateSkillState(Event event, uint8_t id) {
    bool found = (std::find(deadPlayersId.begin(),
                            deadPlayersId.end(),
                            id) != deadPlayersId.end());
    if (found) {
        return;
    }

    if (event == Event::event_throw_grenade or
        event == Event::event_throw_smoke or
        event == Event::event_blitz_atack) {
        players.at(id)->setSkillState(event);
    } else {
        players.at(id)->stopSkillState(event);
    }
}

void GameWorld::simulateStep(double stepTime) {
    if(!ended) {
        simulatePlayersStep(stepTime);
        simulateInfectedStep(stepTime);
        simulateGrenadeStep(stepTime);
        simulateBlitzAtackStep(stepTime);
    }
}

std::shared_ptr<Snapshot> GameWorld::getSnapshot(bool first) {
    if(!ended) {
        std::vector<StOperator> playersInfo;
        for (auto& player : players) {
            playersInfo.push_back(StOperator(player.first,
                                         player.second->getTypeOperator(),
                                         player.second->getState(),
                                         player.second->getPosition(), 
                                         player.second->getHealth(),
                                         player.second->getMunition(),
                                         player.second->isGrenadeAvailable(),
                                         player.second->isSmokeAvailable()));
        }
        
        std::vector<EnemyDto> enemies;
        for (auto& enemy : infecteds) {
            enemies.push_back(EnemyDto(enemy.first,
                                         enemy.second->getTypeInfected(),
                                         enemy.second->getState(),
                                         enemy.second->getPosition()));
        }

        if (first){
            std::vector<ObstacleDto> obsts;
            for (auto& ob : obstacles) {
                obsts.push_back(ObstacleDto(ob.first,
                                             ob.second->getTypeObstacle(),
                                             ob.second->getPosition()));
                }
            return std::make_shared<Snapshot>(playersInfo, enemies, obsts, type, map);        
        }

        std::vector<GrenadeDto> grenadesInfo;
        for (auto& grenade : grenades) {
            grenadesInfo.push_back(GrenadeDto(grenade->exploded(),
                                         grenade->getTypeGrenade(),
                                         grenade->getPosition()));
        }
        
        bool blitzAttacking = !blitzAtacks.empty();
        
        return std::make_shared<Snapshot>(playersInfo, enemies, grenadesInfo, blitzAttacking);
    }
    return std::make_shared<Snapshot>(Event::event_end);
}

std::shared_ptr<Snapshot> GameWorld::getStats() {
    return std::make_shared<Snapshot>(10000, 1000, infectedId);
}

void GameWorld::generateInfecteds() {
    switch (this->type) {
        case TypeGame::game_idle:
            break;
        case TypeGame::game_survival:
            // TODO: implement survival mode logic (ex: when all the infecteds are dead, respawn
            //  new infecteds and increment difficulty)
            this->infecteds = infectedFactory.generateInfecteds(TypeDifficulty::difficulty_easy,
                                                                infectedId,
                                                                collidables,
                                                                RC);
            break;
        case TypeGame::game_clear_zone:
            this->infecteds = infectedFactory.generateInfecteds(TypeDifficulty::difficulty_easy,
                                                                infectedId,
                                                                collidables,
                                                                RC);
            break;
        default:
            break;
    }
}

void GameWorld::generateObstacles() {
    // Random obstacle (can be Tire or Crater):
    std::shared_ptr<Obstacle> newObstacle = this->obstacleFactory.getObstacle(generateObstacleType(),
                                                                              obsacleId,
                                                                              collidables,
                                                                              RC);
    this->obstacles.insert({obsacleId++, newObstacle});
}

TypeObstacle GameWorld::generateObstacleType() {
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int> distr(0, 1);
    return (TypeObstacle)distr(generator);
}

bool GameWorld::isEnded() {
    return ended;
}

int GameWorld::generateMapType() {
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int> distr(1, 3);
    return distr(generator);
}

void GameWorld::simulatePlayersStep(double stepTime) {
    // Erase dead players
    for (auto it = players.cbegin(); it != players.cend(); /*no increment*/ ){
        if (not it->second->isAlive()) {
            deadPlayersId.push_back(it->first);
            collidables.erase(it->first);
            players.erase(it++);
        } else {
            ++it;
        }
    }

    // Apply players step
    for (auto& player : players) {
        players.at(player.first)->applyStep(this->collidables,
                                            this->infecteds,
                                            this->grenades,
                                            this->blitzAtacks,
                                            stepTime);
    }
}

void GameWorld::simulateInfectedStep(double stepTime) {
    // Erase dead infecteds
    for (auto it = infecteds.cbegin(); it != infecteds.cend(); /* no increment */){
        if (not it->second->isAlive()) {
            collidables.erase(it->first);
            infecteds.erase(it++);
        } else {
            ++it;
        }
    }

    // Apply infecteds step
    for (auto& infected : infecteds) {
        infecteds.at(infected.first)->applyStep(this->collidables, this->players);
    }
}

void GameWorld::simulateGrenadeStep(double stepTime) {
    // Erase exploded grenades
    for (auto it = grenades.cbegin(); it != grenades.cend(); /* no increment */){
        if ((*it)->exploded()) {
            grenades.erase(it++);
        } else {
            ++it;
        }
    }

    // Apply grenade step
    for (auto& grenade : grenades) {
        grenade->applyStep(this->players, this->infecteds, stepTime);
    }
}

void GameWorld::simulateBlitzAtackStep(double stepTime) {
    // Erase ended blitz atack
    for (auto it = blitzAtacks.cbegin(); it != blitzAtacks.cend(); /* no increment */){
        if ((*it)->ended()) {
            blitzAtacks.erase(it++);
        } else {
            ++it;
        }
    }

    // Apply blitz step
    for (auto& blitzAtack : blitzAtacks) {
        blitzAtack->applyStep(this->infecteds, stepTime);
    }
}
