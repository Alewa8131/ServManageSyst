// Copyright 2025 Alewa8131
#pragma once
#include <string>
#include "../CoreLib/User.h"
#include "../CoreLib/DateTime.h"

class Privilege;
class Server;

class Player : public User {
 private:
    Server* _server;
    DateTime _joinDate;
    Privilege* _privilege;
    int _minutesPlayed;
    std::string _status;  // "active", "inactive", "banned"
    double _moneySpent;
    // std::vector<Privilege*> privilegeHistory;
 public:
    void setName(const std::string& name);
    void changePrivilege(Privilege* p);
    void addPlaytime(int minutes);
    void updateStatus(const std::string& s);
    void addSpent(double amount);
    void printInfo() const;
};
