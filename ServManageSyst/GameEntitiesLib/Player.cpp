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
    _server = nullptr;
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

void Player::set_join_date(const Core::DateTime& dt) {
    _join_date = dt;
    update_in_file();
}

void Player::set_privilege(Privilege* p) {
    _privilege = p;
    update_in_file();
}

Server* Player::get_server() const { return _server; }
int Player::get_minutes_played() const { return _minutes_played; }
double Player::get_money_spent() const { return _money_spent; }
std::string Player::get_status() const { return _status; }
Privilege* Player::get_privilege() const { return _privilege; }
Core::DateTime Player::get_join_date() const { return _join_date; }

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
            line = std::to_string(_id) + "," +
                _username + "," + _password + "," +
                (_server ? _server->get_name() : "Unknown") + "," +
                _join_date.to_string() + "," +
                privName + "," +
                std::to_string(_minutes_played) + "," +
                _status + "," +
                std::to_string(_money_spent);
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
        std::to_string(_money_spent);
}

Player* Player::from_csv_line(const std::string& line) {
    std::stringstream ss(line);
    std::string field;

    Player* p = new Player();

    // id
    std::getline(ss, field, ',');
    p->set_id(std::stoi(field));

    // username
    std::getline(ss, field, ',');
    p->set_username(field);

    // password
    std::getline(ss, field, ',');
    p->set_password(field);

    // server_name
    std::getline(ss, field, ',');
    p->set_server(field);

    // join_date
    std::getline(ss, field, ',');
    Core::DateTime dt = Core::DateTime::from_string(field);
    p->set_join_date(dt);

    // privilege_name
    std::getline(ss, field, ',');
    if (field != "None") {
        Privilege* pr = new Privilege();
        pr->set_name(field);
        p->change_privilege(pr);
    }

    // minutes_played
    std::getline(ss, field, ',');
    p->add_playtime(std::stoi(field));

    // status
    std::getline(ss, field, ',');
    p->update_status(field);

    // money_spent
    std::getline(ss, field, ',');
    p->add_spent(std::stod(field));

    return p;
}

TVector<Player> Player::load_all() {
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


            // Создаем объект Player напрямую в векторе
            result.push_back(Player(
                id,
                username,
                password,
                server_name,
                join_date,
                privilege_name,
                minutes_played,
                status,
                money_spent
            ));
        }
        catch (const std::exception& e) {
            // Обработка ошибок парсинга (пропуск строки)
            continue;
        }
    }

    return result;
}

