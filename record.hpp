#include <vector>
#include <string>
#include <iostream>
#include <ranges>
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <fstream>

struct Address {
    std::string street;
    std::string town;
    std::string country;
};

struct Record {
    std::string first_name;
    std::vector<std::string> other_names;
    std::string email;
    std::string tel_num;
    Address address;

public:
    int getUUID() const { return uuid; }

    std::string info() const;
    std::string serialize() const;

private:
  // This is a unique ID of the user, not a real UUID since we should not use
  // external libraries.
  int uuid;

  // The UUID should not be visible from the outside, but in this case (when
  // UUID is not generated) the AddressBook needs to provide it.
  friend class AddressBook;
};

