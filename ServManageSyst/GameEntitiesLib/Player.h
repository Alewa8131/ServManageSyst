// Copyright 2025 Alewa8131
#pragma once
#include <string>
#include "../CoreLib/User.h"
#include "../CoreLib/DateTime.h"
#include "../TVector/my_vector.h"

#include "../GameEntitiesLib/Privilege.h"
#include "../GameEntitiesLib/Server.h"

constexpr const char* PLAYER_DB_PATH = "../GameEntitiesLib/db/players.csv";

class Player : public User {
 private:
    Server* _server;
    Core::DateTime _join_date;
    Privilege* _privilege;
    int _minutes_played;
    std::string _status;  // "active", "inactive", "banned"
    double _money_spent;
    TVector<Privilege*> _privilege_history;

    void update_in_file();

 public:
    Player();
    Player(const std::string& username,
        const std::string& password);
    Player(int id,
        const std::string& username,
        const std::string& password,
        const std::string& server_name,
        const std::string& join_date,
        const std::string& privilege_name,
        int minutes_played,
        const std::string& status,
        double money_spent);

    void set_server(const std::string& name);
    void set_join_date(const Core::DateTime& dt);
    void set_privilege(Privilege* p);
    void change_privilege(Privilege* p);
    void add_playtime(int minutes);
    void update_status(const std::string& s);
    void add_spent(double amount);
    std::string save_privilege_history() const;

    Server* get_server() const;
    Core::DateTime get_join_date() const;
    Privilege* get_privilege() const;
    int get_minutes_played() const;
    std::string get_status() const;
    double get_money_spent() const;
    TVector<Privilege*> get_privilege_history() const;
    void parse_privilege_history(const std::string& field);

    std::string to_csv_line() const override;
    static Player from_csv_line(const std::string& line);
    static TVector<Player> load_all_players();
};
