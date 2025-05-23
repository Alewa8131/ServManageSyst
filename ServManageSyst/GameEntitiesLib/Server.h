// Copyright 2025 Alewa8131
#pragma once
#include "Event.h"

class Server {

 private:
    std::string name;
    std::string ip;
    // std::vector<Player*> players;
    // std::vector<class Event*> events;
    double rentCost;
    double uptime;

 public:
    void setName(const std::string& name);
    void addPlayer(Player* player);
    void removePlayer(int playerId);
    void createEvent(Event* event);
    void printStats() const;
};
