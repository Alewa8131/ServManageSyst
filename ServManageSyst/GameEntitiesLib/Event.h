// Copyright 2025 Alewa8131
#pragma once
#include <string>
#include "../GameEntitiesLib/Player.h"
#include "../TVector/my_vector.h"
#include "../CoreLib/DateTime.h"

class Event {
private:
    int _id;
    std::string _name;
    Core::DateTime _startDate;
    int _duration; // in hours
    std::string _status;
    TVector<std::string> _participants;

public:
    Event(int id, const std::string& name, const Core::DateTime& startDate, int duration, const std::string& status);
    Event(int id, std::string& name, Core::DateTime& startDate, int duration, std::string& status);

    void set_name(const std::string& name);
    void set_start_date(const Core::DateTime& dt);
    void set_duration(double d);
    void set_status(const std::string& s);
    void add_player(std::string p);
    void remove_player(std::string p);


    int get_id() const;
    std::string get_name() const;
    Core::DateTime get_start_date() const;
    int get_duration() const;
    std::string get_status() const;
    int get_participant_count() const;
    TVector<std::string> get_participants() const;

    std::string to_csv_string() const;
    static Event from_csv_string(const std::string& line);
};
