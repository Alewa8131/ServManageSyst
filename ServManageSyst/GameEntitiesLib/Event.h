#pragma once

class Event {
private:
    int id;
    std::string name;
    DateTime startDate;
    double duration;
    std::string status;
    //std::vector<Player*> participants;
public:
    void registerPlayer(Player* p);
    int getParticipantCount() const;
};