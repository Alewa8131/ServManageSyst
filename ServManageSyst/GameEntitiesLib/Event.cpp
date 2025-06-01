// Copyright 2025 Alewa8131
#include "Event.h"

Event::Event(int id, const std::string& name, const Core::DateTime& startDate,
    int duration, const std::string& status)
    : _id(id), _name(name), _startDate(startDate),
    _duration(duration), _status(status) {}
Event::Event(int id, std::string& name, Core::DateTime& startDate,
    int duration, std::string& status)
    : _id(id), _name(name), _startDate(startDate),
    _duration(duration), _status(status) {}

void Event::set_name(const std::string& name) {
    _name = name;
}
void Event::set_start_date(const Core::DateTime& dt) {
    _startDate = dt;
}
void Event::set_duration(double d) {
    _duration = static_cast<int>(d);
}
void Event::set_status(const std::string& s) {
    _status = s;
}

void Event::add_player(std::string p) {
    for (int i = 0; i < _participants.size(); ++i) {
        if (_participants[i] == p) return;
    }
    _participants.push_back(p);
}
void Event::remove_player(std::string p) {
    for (int i = 0; i < _participants.size(); ++i) {
        if (_participants[i] == p) {
            _participants.erase(i);
            return;
        }
    }
}

int Event::get_id() const {
    return _id;
}
std::string Event::get_name() const {
    return _name;
}
Core::DateTime Event::get_start_date() const {
    return _startDate;
}
int Event::get_duration() const {
    return _duration;
}
std::string Event::get_status() const {
    return _status;
}
int Event::get_participant_count() const {
    return _participants.size();
}
TVector<std::string> Event::get_participants() const {
    return _participants;
}

std::string Event::to_csv_string() const {
    std::ostringstream oss;
    oss << _id << ",";
    oss << _name << ",";
    oss << _startDate.to_string() << ",";
    oss << _duration << ",";
    oss << _status;
    for (int i = 0; i < _participants.size(); ++i) {
        oss << "," << _participants[i];
    }
    return oss.str();
}

Event Event::from_csv_string(const std::string& line) {
    std::istringstream iss(line);
    std::string token;

    std::getline(iss, token, ',');
    int id = std::stoi(token);

    std::getline(iss, token, ',');
    std::string name = token;

    std::getline(iss, token, ',');
    Core::DateTime date;
    date = date.from_string(token);

    std::getline(iss, token, ',');
    int duration = std::stoi(token);

    std::getline(iss, token, ',');
    std::string status = token;

    Event e(id, name, date, duration, status);

    while (std::getline(iss, token, ',')) {
        e.add_player(token);
    }

    return e;
}
