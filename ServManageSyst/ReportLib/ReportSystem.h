#pragma once
#include "Report.h"

class ReportSystem {
private:
    //std::vector<Report*> pendingReports;
    //std::vector<Report*> reviewedReports;
public:
    void addReport(Report* report);
    void reviewReport(int reportId, const std::string& verdict);
    void printPending() const;
    void printHistory() const;
};