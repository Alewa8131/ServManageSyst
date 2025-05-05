#pragma once
#include <string>

class User {
protected:
    int id;
    std::string username;
public:
    virtual ~User() = default;
    int getId() const;
    std::string getUsername() const;
};