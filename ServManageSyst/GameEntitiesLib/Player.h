// Copyright 2025 Alewa8131
#pragma once
#include <string>
#include "../CoreLib/User.h"
#include "../CoreLib/DateTime.h"
#include "../TVector/my_vector.h"

class Privilege;
class Server;

class Player : public User {
 private:
    Server* _server;
    DateTime _joinDate;
    Privilege* _privilege;
    int _minutes_played;
    std::string _status;  // "active", "inactive", "banned"
    double _moneySpent;
    TVector<Privilege*> privilege_history;

 public:
    void set_server(const std::string& name);
    void change_privilege(Privilege* p);
    void add_playtime(int minutes);
    void update_status(const std::string& s);
    void add_spent(double amount);
    void print_info() const;
};
