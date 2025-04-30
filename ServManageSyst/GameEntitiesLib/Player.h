#pragma once
#include <../CoreLib/User.h>
#include <../CoreLib/DateTime.h>

class Privilege;
class Server;

class Player : public User {
private:
    Server* server;
    DateTime joinDate;
    Privilege* privilege;
    int minutesPlayed;
    std::string status; // "active", "inactive", "banned"
    double moneySpent;
    //std::vector<Privilege*> privilegeHistory;
public:
    void setName(const std::string& name);
    void changePrivilege(Privilege* p);
    void addPlaytime(int minutes);
    void updateStatus(const std::string& s);
    void addSpent(double amount);
    void printInfo() const;
};