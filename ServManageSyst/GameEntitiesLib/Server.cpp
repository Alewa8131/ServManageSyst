// Copyright 2025 Alewa8131
#include "Server.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include "../CoreLib/DateTime.h"
#include "../GameEntitiesLib/Player.h"
#include "../GameEntitiesLib/Event.h"

Server::Server() {
    _name = "DefaultServer";
    _ip = "127.0.0.1";
    _rent_cost = 0.0;
    _uptime = 0.0;

    check_and_add_to_server_db();
}

Server::Server(std::string name) {
    _name = name;

    std::ifstream file(SERVER_DB_PATH);
    std::string line;
    bool serverFound = false;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string server_name, ip, costStr, uptimeStr;

        if (!std::getline(ss, server_name, ',')) continue;
        if (!std::getline(ss, ip, ',')) continue;
        if (!std::getline(ss, costStr, ',')) continue;
        if (!std::getline(ss, uptimeStr)) continue;

        if (server_name == _name) {
            _ip = ip;
            _rent_cost = std::stod(costStr);
            _uptime = std::stoi(uptimeStr);
            serverFound = true;
            break;
        }
    }
    file.close();
    if (!serverFound) {
        _ip = "127.0.0.1";
        _rent_cost = 0.0;
        _uptime = 0.0;
    }
    check_and_add_to_server_db();
}

Server::Server(std::string name, std::string ip, double rent_cost, int uptime) 
    : _name(name), _ip(ip), _rent_cost(rent_cost), _uptime(uptime) {

    check_and_add_to_server_db();

    // Load player logins
    std::ifstream pfile(VECTORS_DB_PATH + name + "_players.csv");
    std::string line;
    while (std::getline(pfile, line)) {
        if (!line.empty())
            _players.push_back(line);
    }
    pfile.close();

    // Load events
    std::ifstream efile(VECTORS_DB_PATH + name + "_events.csv");
    while (std::getline(efile, line)) {
        std::stringstream ss(line);
        std::string id_str, ename, date_str, duration_str, status_str, participant_str;

        if (!std::getline(ss, id_str, ',')) continue;
        if (!std::getline(ss, ename, ',')) continue;
        if (!std::getline(ss, date_str, ',')) continue;
        if (!std::getline(ss, duration_str, ',')) continue;
        if (!std::getline(ss, status_str, ',')) continue;
        if (!std::getline(ss, participant_str)) continue;


        int id = std::stoi(id_str);
        int duration = std::stoi(duration_str);
        Core::DateTime start_date;
        start_date.from_string(date_str);

        Event* e = new Event(id, ename, start_date, duration, status_str);

        TVector<std::string> participants;
        std::stringstream ps(participant_str);
        std::string login;
        while (std::getline(ps, login, ';')) {
            if (!login.empty()) {
                participants.push_back(login);
                //e->add_player();
            }
        }

        _events.push_back(e);
    }

    efile.close();
}


void Server::set_name(const std::string& new_name) {
    std::string old_player_file = VECTORS_DB_PATH + _name + "_players.csv";
    std::string old_event_file = VECTORS_DB_PATH + _name + "_events.csv";

    std::string new_player_file = VECTORS_DB_PATH + new_name + "_players.csv";
    std::string new_event_file = VECTORS_DB_PATH + new_name + "_events.csv";

    if (rename(old_player_file.c_str(), new_player_file.c_str()) != 0) {
        std::ofstream npf(new_player_file);
        npf.close();
    }

    _name = new_name;
}
void Server::set_ip(const std::string& ip) {
    _ip = ip;
}
void Server::set_rent_cost(double cost) {
    _rent_cost = cost;
}
void Server::set_uptime(int uptime) {
    _uptime = uptime;
}


std::string Server::get_name() const {
    return _name;
}
std::string Server::get_ip() const {
    return _ip;
}
double Server::get_rent_cost() const {
    return _rent_cost;
}
int Server::get_uptime() const {
    return _uptime;
}
TVector<Event*> Server::get_events() const {
    TVector<Event*> result;
    std::ifstream fin(VECTORS_DB_PATH + _name + "_events.csv");
    std::string line;

    while (std::getline(fin, line)) {
        if (line.empty()) continue;

        Event e = Event::from_csv_string(line);
        Event* ptr = new Event(e);
        result.push_back(ptr);
    }

    fin.close();
    return result;
}
TVector<std::string> Server::get_players() const {
    return _players;
}

void Server::add_player(const std::string& player_login) {
    _players.push_back(player_login);
    write_players_to_file();
}
void Server::remove_player(const std::string& player_login) {
    for (int i = 0; i < _players.size(); ++i) {
        if (_players[i] == player_login) {
            _players.erase(i);
            write_players_to_file();
            return;
        }
    }
}

void Server::create_event(Event* event) {
    _events.push_back(event);
    sort_events_by_date();
    write_events_to_file();
}
void Server::remove_event(int index) {
    if (index >= 0 && index < _events.size()) {
        _events.erase(index);
        write_events_to_file();
    }
}
void Server::update_event(int index, Event* newEvent) {
    if (index >= 0 && index < _events.size()) {
        _events[index] = newEvent;
        sort_events_by_date();
        write_events_to_file();
    }
}
void Server::sort_events_by_date() {
    hoar_sort(_events);
}

void Server::write_players_to_file() {
    std::ofstream out(VECTORS_DB_PATH + _name + "_players.csv", std::ios::trunc);
    for (int i = 0; i < _players.size(); ++i) {
        out << _players[i] << "\n";
    }
    out.close();
}

void Server::write_events_to_file() {
    std::ofstream out(VECTORS_DB_PATH + _name + "_events.csv", std::ios::trunc);
    for (int i = 0; i < _events.size(); ++i) {
        Event* e = _events[i];
        out << e->get_id() << ","
            << e->get_name() << ","
            << e->get_start_date().to_string() << ","
            << e->get_duration() << ","
            << e->get_status();

        TVector<std::string> participants = e->get_participants();
        for (int j = 0; j < participants.size(); ++j) {
            out << "," << participants[j];
        }

        out << "\n";
    }
    out.close();
}

TVector<Server> Server::load_all() {
    TVector<Server> result;
    std::ifstream file(SERVER_DB_PATH);

    if (!file.is_open())
        return result;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name, ip, costStr, uptimeStr;

        if (!std::getline(ss, name, ',')) continue;
        if (!std::getline(ss, ip, ',')) continue;
        if (!std::getline(ss, costStr, ',')) continue;
        if (!std::getline(ss, uptimeStr)) continue;

        double rent_cost = std::stod(costStr);
        int uptime = std::stoi(uptimeStr);

        Server server(name, ip, rent_cost, uptime);
        result.push_back(server);
    }

    file.close();
    return result;
}

void Server::check_and_add_to_server_db() {
    std::ifstream file(SERVER_DB_PATH);
    std::string line;
    bool found = false;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string server_name;
        if (std::getline(ss, server_name, ',')) {
            if (server_name == _name) {
                found = true;
                break;
            }
        }
    }
    file.close();

    if (!found) {
        std::ofstream outFile(SERVER_DB_PATH, std::ios::app);
        if (outFile.is_open()) {
            outFile << _name << "," << _ip << "," << _rent_cost << "," << _uptime << "\n";
            outFile.close();
        }

        std::ofstream playerFile(VECTORS_DB_PATH + _name + "_players.csv", std::ios::trunc);
        playerFile.close();

        std::ofstream eventFile(VECTORS_DB_PATH + _name + "_events.csv", std::ios::trunc);
        eventFile.close();
    }
}
