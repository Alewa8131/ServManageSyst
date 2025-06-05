// Copyright 2025 Alewa8131
#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include "../TVector/my_vector.h"

constexpr const char* USER_DB_PATH = "../CoreLib/db/users.csv";

enum class UserRole { Player, Moderator };

class User {
 protected:
    int _id;
    std::string _username;
    std::string _password;
    UserRole _role;

 public:
    User();
    User(int id, const std::string& username,
        const std::string& password, UserRole role);
    User(const std::string& username,
        const std::string& password, UserRole role);
    virtual ~User() = default;
    User create_user();

    int get_id() const;
    void set_id(int id);

    std::string get_username() const;
    void set_username(const std::string& username);

    std::string get_password() const;
    void set_password(const std::string& password);

    UserRole get_role() const;

    // DataBase
    static TVector<User> load_all_users(const std::string& filename);
    static bool save_all_users(const TVector<User>& users,
        const std::string& filename);
    static int get_next_id(const std::string& path);
    static bool add_user(const User& new_user, const std::string& filename);
    bool update_user_in_file(const std::string& filename) const;
    static int find_index_by_id(const TVector<User>& users, int id);
    static int find_index_by_username(const TVector<User>& users,
        const std::string& username);

    // CSV convet
    virtual std::string to_csv_line() const;
    static User from_csv_line(const std::string& line);
};
