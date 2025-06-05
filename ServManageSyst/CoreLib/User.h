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

    static bool is_valid_input(const std::string& input);

    // DataBase
    static TVector<User> load_all_users(const std::string& filename);
    template <class T>
    static bool save_all_objects(const TVector<T>& objects, const std::string& filename) {
        std::ofstream file(filename, std::ios::trunc);
        if (!file.is_open()) return false;

        for (int i = 0; i < objects.size(); ++i) {
            file << objects[i].to_csv_line() << "\n";
        }

        file.close();
        return true;
    }
    bool update_user_in_file(const std::string& filename) const;

    static int get_next_id(const std::string& path);
    static bool add_user(const User& new_user, const std::string& filename);
    static int find_index_by_id(const TVector<User>& users, int id);
    static int find_index_by_username(const TVector<User>& users,
        const std::string& username);

    // CSV convet
    virtual std::string to_csv_line() const;
    static User from_csv_line(const std::string& line);
};
