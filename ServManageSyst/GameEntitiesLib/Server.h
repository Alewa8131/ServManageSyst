// Copyright 2025 Alewa8131
#pragma once
#include <string>
#include "../TVector/my_vector.h"
class Player;
class Event;

class Server {
 private:
    std::string _name;
    std::string _ip;
    TVector<Player*> _players;
    TVector<class Event*> _events;
    double _rent_cost;
    double _uptime;

 public:
    void set_name(const std::string& name);
    std::string get_name();
    void add_player(Player* player);
    void remove_player(int playerId);
    void create_event(Event* event);
    void print_stats() const;
};
