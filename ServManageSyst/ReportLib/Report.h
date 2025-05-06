// Copyright 2025 Alewa8131
#pragma once
#include <../GameEntitiesLib/Player.h>

class Report {

 private:
    int id;
    Player* author;
    std::string content;
    std::string reportType;  // "cheating", "abuse"
    Player* reportedPlayer;  // необязательно
    std::string status;  // "pending", "reviewed"
    std::string verdict;
    DateTime submissionDate;
    DateTime decisionDate;

 public:
    void setContent(const std::string& text);
    void setType(const std::string& type);
    void setReportedPlayer(Player* player);
    void submit();
    void review(const std::string& verdict);
    void print() const;
};
