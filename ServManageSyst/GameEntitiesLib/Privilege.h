// Copyright 2025 Alewa8131
#pragma once
#include <string>
#include "../CoreLib/DateTime.h"

constexpr const char* PRIVILEGE_DB_PATH = "../GameEntitiesLib/db/privileges.csv";

class Privilege {
 private:
    std::string _name;
    double _price;
    Core::DateTime _purchase_date;

 public:
    Privilege();
    Privilege(const std::string& name, double price,
        const Core::DateTime& purchase_date);

    void set_name(const std::string& name);
    void set_price(double price);
    void set_purchase_date(const Core::DateTime& date);

    std::string get_name() const;
    double get_price() const;
    Core::DateTime get_purchase_date() const;
    int get_duration_days() const;

    bool is_active(int valid_days, const Core::DateTime& now) const;
    Core::DateTime get_expiry_date(int valid_days) const;

    std::string to_csv_line() const;
    static Privilege from_csv_line(const std::string& line);
};
