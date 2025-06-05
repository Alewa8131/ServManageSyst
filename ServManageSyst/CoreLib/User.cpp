// Copyright 2025 Alewa8131
#include "User.h"
#include <string>

User::User() : _id(0), _username(""), _password(""), _role(UserRole::Player) {}
User::User(int id, const std::string& username,
    const std::string& password, UserRole role)
    : _id(id), _username(username), _password(password), _role(role) {}

User::User(const std::string& username,
    const std::string& password, UserRole role) {

    if (!is_valid_input(username) || !is_valid_input(password)) {
        throw std::runtime_error("Username or password contains forbidden characters");
    }

    TVector<User> users = load_all_users(USER_DB_PATH);
    if (find_index_by_username(users, username) != -1) {
        throw std::runtime_error("Username already taken");
    }

    _id = get_next_id(USER_DB_PATH);
    _username = username;
    _password = password;
    _role = role;
}
User User::create_user() {
    _id = get_next_id(USER_DB_PATH);
    _username = "user" + std::to_string(_id);
    _password = "default";
    _role = UserRole::Player;
    return *this;
}

int User::get_id() const {
    return _id;
}
void User::set_id(int id) {
    _id = id;
}

std::string User::get_username() const {
    return _username;
}
void User::set_username(const std::string& username) {
    _username = username;
}

std::string User::get_password() const {
    return _password;
}
void User::set_password(const std::string& password) {
    _password = password;
}

UserRole User::get_role() const {
    return _role;
}

bool User::is_valid_input(const std::string& input) {
    const std::string forbidden_chars = " ;:'\",./\\`~%^()[]{}";

    for (char c : input) {
        if (forbidden_chars.find(c) != std::string::npos) {
            return false;
        }
    }
    return true;
}

TVector<User> User::load_all_users(const std::string& filename) {
    TVector<User> users;
    std::ifstream file(filename);
    if (!file.is_open()) {
        return users;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        users.push_back(from_csv_line(line));
    }
    file.close();
    return users;
}

bool User::update_user_in_file(const std::string& filename) const {
    TVector<User> users = load_all_users(filename);

    int index = find_index_by_id(users, this->_id);
    if (index != -1) {
        users[index] = *this;
        return save_all_objects(users, filename);
    }

    return false;
}

int User::get_next_id(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return 1;

    int max_id = 0;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string id_str;
        if (!std::getline(ss, id_str, ',')) continue;
        try {
            int id = std::stoi(id_str);
            if (id > max_id) max_id = id;
        }
        catch (...) {
            continue;
        }
    }

    return max_id + 1;
}
bool User::add_user(const User& new_user, const std::string& filename) {
    TVector<User> users = load_all_users(filename);

    users.push_back(new_user);
    return save_all_objects(users, filename);
}

int User::find_index_by_id(const TVector<User>& users, int id) {
    for (int i = 0; i < users.size(); ++i) {
        if (users[i].get_id() == id) {
            return i;
        }
    }
    return -1;
}
int User::find_index_by_username(const TVector<User>& users,
    const std::string& username) {

    for (int i = 0; i < users.size(); ++i) {
        if (users[i].get_username() == username) {
            return i;
        }
    }
    return -1;
}

User User::from_csv_line(const std::string& line) {
    std::stringstream ss(line);
    std::string id_str, username, password, role_str;

    std::getline(ss, id_str, ',');
    std::getline(ss, username, ',');
    std::getline(ss, password, ',');
    std::getline(ss, role_str, ',');

    int id = std::stoi(id_str);
    UserRole role = (role_str == "Moderator")
        ? UserRole::Moderator : UserRole::Player;

    return User(id, username, password, role);
}
std::string User::to_csv_line() const {
    return std::to_string(_id) + "," + _username + "," + _password + "," +
        (_role == UserRole::Moderator ? "Moderator" : "Player");
}
