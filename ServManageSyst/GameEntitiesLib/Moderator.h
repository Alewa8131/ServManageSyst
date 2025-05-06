// Copyright 2025 Alewa8131
#pragma once
#include <User.h>

class Moderator : public User {

 public:
    void reviewReport(class Report* report, const std::string& verdict);
};
