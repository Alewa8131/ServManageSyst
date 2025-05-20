// Copyright 2025 Alewa8131
#pragma once
#include <string>
#include "../GameEntitiesLib/Player.h"

class Event {
 private:
    int _id;
    std::string _name;
    DateTime _startDate;
    double _duration;
    std::string _status;
    // std::vector<Player*> participants;
 public:
    void registerPlayer(Player* p);
    int getParticipantCount() const;
};
