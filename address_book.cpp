#include "address_book.hpp"
#include <stdexcept>
#include <functional>

// ============================================================================
// Public functions
// ============================================================================

AddressBook::AddressBook() {
    deserialize();
}

void AddressBook::run() {
    std::string input;

    while (this->running) {
        std::cout << "Please enter a command (/add, /search, /remove or /exit)\n";
        std::cin >> input;

        try {
            (this->*cmdMap.at(input))();
        } catch (std::out_of_range& e) {
            std::cout << "Invalid input\n";
        }
    }
}

// ============================================================================
// Private functions
// ============================================================================


std::string AddressBook::ask(const std::string& msg) {
    std::string input;
    std::cout << msg << ": ";
    std::cin >> input;
    return input;
}

void AddressBook::deserialize() {
    std::ifstream in(database_filename);

    in >> this->total_entries;

    std::string line;
    std::getline(in, line);

    while (std::getline(in, line)) {
        Record r;
        std::stringstream ss(line);

        std::string other;
        std::string uuid;

        std::getline(ss, r.first_name, ',');
        std::getline(ss, other, ',');
        std::getline(ss, r.email, ',');
        std::getline(ss, r.tel_num, ',');
        std::getline(ss, r.address.street, ',');
        std::getline(ss, r.address.town, ',');
        std::getline(ss, r.address.country, ',');
        std::getline(ss, uuid, ',');

        r.uuid = std::stoi(uuid);

        std::stringstream other_names(other);

        std::string name;
        while (std::getline(other_names, name, ';')) {
            r.other_names.push_back(name);
        }

        this->book.push_back(r);
    }
}

void AddressBook::serialize() {
    std::ofstream out(database_filename);
    out << this->total_entries << '\n';

    for (const auto& r : this->book) {
        out << r.serialize() << '\n';
    }
}

void AddressBook::exitCmd() {
    std::cout << "Bye :^)\n";
    serialize();
    running = false;
}

void AddressBook::removeCmd() {
    std::string uuid = ask("Please provide the UUID of the record you want to remove");

    int uuid_value = std::stoi(uuid);

    this->book.erase(std::remove_if(this->book.begin(), this->book.end(), [uuid_value] (const Record& r) {
                return r.getUUID() == uuid_value;
                }), this->book.end());

    std::cout << "You removed the record with the UUID: " << uuid_value << '\n';
}

void AddressBook::searchCmd() {
    std::string search_by = ask(
            "0. First name\n"
            "1. Other names\n"
            "2. Email Address\n"
            "3. Telephone number\n"
            "4. Town\n"
            "5. Country\n"
            "By what key do you want to search? (Type the number)"
            );

    std::string search_term = ask("Search term");

    std::vector<Record> found_persons;

    int search_by_value = std::stoi(search_by);

    /**
     * Iterate through all the records, saving the ones that match the search_term.
     * Time Complexity: O(n) since we just need to iterate once through all entries.
     * Space Complexity: O(1) as we are not using any extra space, except for the results.
     */
    std::for_each(this->book.cbegin(), this->book.cend(), [&](const Record &r) {
        if (search_functions[search_by_value](r, search_term)) {
            found_persons.push_back(r);
        }
    });

    std::cout << "Search result: ";
    for (const auto &r : found_persons) {
        std::cout << r.info();
    }
}

void AddressBook::addCmd() {
    Record r;
    r.first_name = ask("First name");

    while (true) {
        if (const auto& name = ask("Other name (press '.' if no more other names)"); name == ".") {
            break;
        } else {
            r.other_names.push_back(name);
        }
    }

    r.email = ask("Email");
    r.tel_num = ask("Telephone number");
    r.address.street = ask("Street");
    r.address.town = ask("Town");
    r.address.country = ask("Country");

    r.uuid = ++total_entries;

    book.push_back(r);
}
