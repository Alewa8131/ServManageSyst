// Copyright 2025 Alewa8131
#include "Server.h"

#include <string>

void Server::set_name(const std::string& name) {
    _name = name;
}

std::string Server::get_name() {
    return _name;
}

void Server::add_player(Player* player) {
    _players.push_back(player);
}
