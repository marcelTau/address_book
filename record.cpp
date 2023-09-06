#include "record.hpp"

std::string Record::info() const {
    std::string res;

    res += "===== " + std::to_string(uuid) + " =====\n";
    res += "Name: " + first_name;
    for (const auto& n : other_names) {
        res += " " + n;
    }

    res += '\n';

    res += "Email: " + email + '\n';
    res += "Tel num: " + tel_num + '\n';
    res += "Street: " + address.street + '\n';
    res += "Town: " + address.town + '\n';
    res += "Country: " + address.country + '\n';
    res += "UUID: " + std::to_string(uuid) + '\n';

    res += "=============\n";

    return res;
}

std::string Record::serialize() const {
    std::stringstream ss;

    ss << first_name << ',';

    for (int i = 0; i < other_names.size(); ++i) {
        ss << other_names[i];
        if (i + 1 < other_names.size()) {
            ss << ';';
        }
    }

    ss << ',' << email << ',' << tel_num << ',' << address.street << ','
        << address.town << ',' << address.country << ',' << uuid << ',';

    return ss.str();
}
