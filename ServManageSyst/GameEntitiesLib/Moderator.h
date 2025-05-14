// Copyright 2025 Alewa8131
#pragma once
#include "../CoreLib/User.h"
#include <string>

class Moderator : public User {

 public:
    void reviewReport(class Report* report, const std::string& verdict);
};
