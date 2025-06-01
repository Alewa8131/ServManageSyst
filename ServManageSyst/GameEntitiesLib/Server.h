// Copyright 2025 Alewa8131
#pragma once
#include <string>
#include "../TVector/my_vector.h"
class Player;
class Event;

constexpr const char* SERVER_DB_PATH = "../GameEntitiesLib/db/servers.csv";
constexpr const char* VECTORS_DB_PATH = "../GameEntitiesLib/db/Servers/";


class Server {
 private:
    std::string _name;
    std::string _ip;
    TVector<std::string> _players;
    TVector<Event*> _events;
    double _rent_cost;
    int _uptime;

 public:
     Server();
     Server(std::string name);
     Server(std::string name, std::string ip, double rent_cost, int uptime);

     void set_name(const std::string& name);
     void set_ip(const std::string& ip);
     void set_rent_cost(double cost);
     void set_uptime(int uptime);

     std::string get_name() const;
     std::string get_ip() const;
     double get_rent_cost() const;
     int get_uptime() const;
     TVector<std::string> get_players() const;
     TVector<Event*> get_events() const;

     // Player management
     void add_player(const std::string& player_login);
     void remove_player(const std::string& player_login);

     // Event management
     void create_event(Event* event);
     void remove_event(int index);
     void update_event(int index, Event* newEvent);
     void sort_events_by_date();

     void write_players_to_file();
     void write_events_to_file();

     static TVector<Server> load_all();
     void check_and_add_to_server_db();
};
