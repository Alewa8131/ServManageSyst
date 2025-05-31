// Copyright 2025 Alewa8131
#pragma once
#include <string>
#include "../GameEntitiesLib/Player.h"
#include "../TVector/my_vector.h"

class Event {
 private:
    int _id;
    std::string _name;
    Core::DateTime _startDate;
    double _duration;
    std::string _status;
    TVector<Player*> _participants;

 public:
    void registerPlayer(Player* p);
    int getParticipantCount() const;
};
