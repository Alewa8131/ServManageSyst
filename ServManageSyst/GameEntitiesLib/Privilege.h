// Copyright 2025 Alewa8131
#pragma once
#include <string>
#include "../CoreLib/DateTime.h"

class Privilege {
 private:
    std::string _name;
    double _price;
    DateTime _purchase_date;

 public:
    Privilege();
    Privilege(const std::string& name, double price,
        const DateTime& purchase_date);

    void set_name(const std::string& name);
    void set_price(double price);
    void set_purchase_date(const DateTime& date);

    std::string get_name() const;
    double get_price() const;
    DateTime get_purchase_date() const;
    int get_duration_days() const;

    bool is_active(int valid_days, const DateTime& now) const;
    DateTime get_expiry_date(int valid_days);

    std::string to_csv_line() const;
    static Privilege from_csv_line(const std::string& line);
};
