#pragma once
#include <User.h>

class Moderator : public User {
public:
    void reviewReport(class Report* report, const std::string& verdict);
};