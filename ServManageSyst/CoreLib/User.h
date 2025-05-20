// Copyright 2025 Alewa8131
#pragma once
#include <string>

class User {
 protected:
    int _id;
    std::string _username;

 public:
    virtual ~User() = default;
    int getId() const;
    std::string getUsername() const;
};
