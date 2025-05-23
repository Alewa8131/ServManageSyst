// Copyright 2025 Alewa8131
#pragma once
#include "Player.h"

class Event {

 private:
    int id;
    std::string name;
    DateTime startDate;
    double duration;
    std::string status;
    // std::vector<Player*> participants;

 public:
    void registerPlayer(Player* p);
    int getParticipantCount() const;
};
