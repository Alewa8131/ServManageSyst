// Copyright 2025 Alewa8131
#include "Privilege.h"
#include <string>
#include <iostream>

Privilege::Privilege()
    : _name("None"), _price(0.0), _purchase_date(DateTime::DateTime()) {}

Privilege::Privilege(const std::string& name, double price, const DateTime& purchase_date)
    : _name(name), _price(price), _purchase_date(purchase_date) {}

void Privilege::set_name(const std::string& name) {
    _name = name;
}
void Privilege::set_price(double price) {
    _price = price;
}
void Privilege::set_purchase_date(const DateTime& date) {
    _purchase_date = date;
}

std::string Privilege::get_name() const {
    return _name;
}
double Privilege::get_price() const {
    return _price;
}
DateTime Privilege::get_purchase_date() const {
    return _purchase_date;
}

int Privilege::get_duration_days() const {
    DateTime now;
    now.set_current();

    int days_now = now.days_since_epoch();
    int days_then = _purchase_date.days_since_epoch();

    return days_now - days_then;
}

bool Privilege::is_active(int valid_days, const DateTime& now) const {
    int purchase_days = _purchase_date.days_since_epoch();
    int current_days = now.days_since_epoch();

    return (current_days - purchase_days) < valid_days;
}
DateTime Privilege::get_expiry_date(int valid_days) {
    DateTime copy = _purchase_date;
    copy.add_days(valid_days);
    return copy;
}

std::string Privilege::to_csv_line() const {
    return _name + "," + std::to_string(_price) + "," + _purchase_date.to_string();
}
Privilege Privilege::from_csv_line(const std::string& line) {
    size_t pos1 = line.find(',');
    if (pos1 == std::string::npos) {
        throw std::invalid_argument("Invalid CSV line: missing first comma");
    }

    size_t pos2 = line.find(',', pos1 + 1);
    if (pos2 == std::string::npos) {
        throw std::invalid_argument("Invalid CSV line: missing second comma");
    }

    std::string name = line.substr(0, pos1);
    std::string price_str = line.substr(pos1 + 1, pos2 - pos1 - 1);
    std::string date_str = line.substr(pos2 + 1);

    double price = std::stod(price_str);
    DateTime date = DateTime::from_string(date_str);

    return Privilege(name, price, date);
}
