// Copyright 2025 Alewa8131
#include "Player.h"
#include <fstream>
#include <sstream>

Player::Player() : User() {
    _server = nullptr;
    _join_date = Core::DateTime();
    _privilege = new Privilege();
    _minutes_played = 0;
    _status = "active";
    _money_spent = 0.0;
    _privilege_history = TVector<Privilege*>();
}
Player::Player(const std::string& username,
    const std::string& password) {
    this->_id = get_next_id(USER_DB_PATH);
    this->_username = username;
    this->_password = password;
    _server = new Server("DefaultServer");
    _join_date = Core::DateTime();
    _privilege = new Privilege();
    _minutes_played = 0;
    _status = "active";
    _money_spent = 0;
    _privilege_history = TVector<Privilege*>();
}

Player::Player(int id,
    const std::string& username,
    const std::string& password,
    const std::string& server_name,
    const std::string& join_date,
    const std::string& privilege_name,
    int minutes_played,
    const std::string& status,
    double money_spent) {

    this->_id = id;
    this->_username = username;
    this->_password = password;
    _server = new Server(server_name);
    _join_date = Core::DateTime(join_date);
    _privilege = new Privilege();  // NAME
    _minutes_played = minutes_played;
    _status = status;
    _money_spent = money_spent;
    _privilege_history = TVector<Privilege*>();
}

void Player::set_server(const std::string& name) {
    // пока только имя сервера, можно доработать с Server*
    update_in_file();
}
void Player::set_join_date(const Core::DateTime& dt) {
    _join_date = dt;
    update_in_file();
}
void Player::set_privilege(Privilege* p) {
    _privilege = p;
    update_in_file();
}
void Player::change_privilege(Privilege* p) {
    _privilege = p;
    _privilege_history.push_back(p);
    update_in_file();
}
void Player::add_playtime(int minutes) {
    _minutes_played += minutes;
    update_in_file();
}
void Player::update_status(const std::string& s) {
    _status = s;
    update_in_file();
}
void Player::add_spent(double amount) {
    _money_spent += amount;
    update_in_file();
}
std::string Player::save_privilege_history() const {
    std::string result;
    for (int i = 0; i < _privilege_history.size(); ++i) {
        Privilege* p = _privilege_history[i];
        if (i > 0) result += ";";
        result += p->get_name() + "|" +
            std::to_string(p->get_price()) + "|" +
            p->get_purchase_date().to_string();
    }
    return result;
}

Server* Player::get_server() const { return _server; }
Core::DateTime Player::get_join_date() const { return _join_date; }
Privilege* Player::get_privilege() const { return _privilege; }
int Player::get_minutes_played() const { return _minutes_played; }
std::string Player::get_status() const { return _status; }
double Player::get_money_spent() const { return _money_spent; }
TVector<Privilege*> Player::get_privilege_history() const {
    return _privilege_history;
}
void Player::parse_privilege_history(const std::string& field) {
    _privilege_history.clear();
    std::stringstream ss(field);
    std::string entry;

    while (std::getline(ss, entry, ';')) {
        size_t p1 = entry.find('|');
        size_t p2 = entry.find('|', p1 + 1);
        if (p1 == std::string::npos || p2 == std::string::npos) continue;

        std::string name = entry.substr(0, p1);
        double price = std::stod(entry.substr(p1 + 1, p2 - p1 - 1));
        std::string date_str = entry.substr(p2 + 1);

        Core::DateTime date = Core::DateTime::from_string(date_str);
        _privilege_history.push_back(new Privilege(name, price, date));
    }
}



void Player::update_in_file() {
    std::ifstream in(PLAYER_DB_PATH);
    std::string line;
    TVector<std::string> lines;

    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string id_str;
        std::getline(ss, id_str, ',');
        int id = std::stoi(id_str);

        if (id == _id) {
            std::string privName = (_privilege ? _privilege->get_name() : "None");
            line = to_csv_line();
        }

        lines.push_back(line);
    }

    in.close();
    std::ofstream out(PLAYER_DB_PATH, std::ios::trunc);
    for (size_t i = 0; i < lines.size(); ++i) {
        out << lines[i] << "\n";
    }
}
std::string Player::to_csv_line() const {
    std::string privilege_name = (_privilege ? _privilege->get_name() : "None");
    std::string server_name = (_server ? _server->get_name() : "Unknown");

    return std::to_string(_id) + "," +
        _username + "," +
        _password + "," +
        server_name + "," +
        _join_date.to_string() + "," +
        privilege_name + "," +
        std::to_string(_minutes_played) + "," +
        _status + "," +
        std::to_string(_money_spent) + ","
        + save_privilege_history();
}
Player Player::from_csv_line(const std::string& line) {
    std::stringstream ss(line);
    std::string field;

    Player p = Player();

    // id
    std::getline(ss, field, ',');
    p.set_id(std::stoi(field));

    // username
    std::getline(ss, field, ',');
    p.set_username(field);

    // password
    std::getline(ss, field, ',');
    p.set_password(field);

    // server_name
    std::getline(ss, field, ',');
    p.set_server(field);

    // join_date
    std::getline(ss, field, ',');
    Core::DateTime dt = Core::DateTime::from_string(field);
    p.set_join_date(dt);

    // privilege_name
    std::getline(ss, field, ',');
    Privilege* pr = new Privilege();
    pr->set_name(field);
    if (p.get_privilege_history().size() == 0)
        p.change_privilege(pr);
    else
        p.set_privilege(pr);

    // minutes_played
    std::getline(ss, field, ',');
    p.add_playtime(std::stoi(field));

    // status
    std::getline(ss, field, ',');
    p.update_status(field);

    // money_spent
    std::getline(ss, field, ',');
    p.add_spent(std::stod(field));

    if (std::getline(ss, field)) {
        p.parse_privilege_history(field);
    }

    return p;
}
TVector<Player> Player::load_all_players() {
    TVector<Player> result;
    std::ifstream file(PLAYER_DB_PATH);
    if (!file.is_open()) return result;

    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string field;

        int id;
        std::string username;
        std::string password;
        std::string server_name;
        std::string join_date;
        std::string privilege_name;
        int minutes_played;
        std::string status;
        double money_spent;
        std::string privilege_history_str;

        try {
            std::getline(ss, field, ','); id = std::stoi(field);
            std::getline(ss, username, ',');
            std::getline(ss, password, ',');
            std::getline(ss, server_name, ',');
            std::getline(ss, join_date, ',');
            std::getline(ss, privilege_name, ',');
            std::getline(ss, field, ','); minutes_played = std::stoi(field);
            std::getline(ss, status, ',');
            std::getline(ss, field, ','); money_spent = std::stod(field);
            std::getline(ss, privilege_history_str);

            Player player(
                id,
                username,
                password,
                server_name,
                join_date,
                privilege_name,
                minutes_played,
                status,
                money_spent
            );

            player.parse_privilege_history(privilege_history_str);

            result.push_back(player);
        }
        catch (const std::exception& e) {
            continue;
        }
    }

    return result;
}

