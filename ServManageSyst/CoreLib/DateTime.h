// Copyright 2025 Alewa8131
#pragma once
#include <string>

namespace Core {
    class DateTime {
    private:
        int _day, _month, _year, _hour, _minute, _second;

    public:
        DateTime();
        DateTime(int d, int m, int y, int h, int min, int s);
        DateTime(const std::string date);
        void add_days(int days);
        void set_current();
        std::string to_string() const;
        static DateTime from_string(const std::string& str);
        bool isPast() const;
        bool operator<(const DateTime& other) const;

        bool is_leap(int year) const;
        int days_in_month(int month, int year) const;
        int days_since_epoch() const;
        int difference_in_days(const DateTime& other) const;
    };
}  // namespace Core
