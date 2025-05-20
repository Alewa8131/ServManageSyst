// Copyright 2025 Alewa8131
#pragma once
#include <string>
#include "../GameEntitiesLib/Player.h"

class Report {
 private:
    int _id;
    Player* _author;
    std::string _content;
    std::string _reportType;  // "cheating", "abuse"
    Player* _reportedPlayer;
    std::string _status;  // "pending", "reviewed"
    std::string _verdict;
    DateTime _submissionDate;
    DateTime _decisionDate;

 public:
    void setContent(const std::string& text);
    void setType(const std::string& type);
    void setReportedPlayer(Player* player);
    void submit();
    void review(const std::string& verdict);
    void print() const;
};
