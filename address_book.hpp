#include "record.hpp"
#include <functional>


class AddressBook {

public:
    AddressBook();

    void run();

private:
    std::string ask(const std::string& msg);

    void deserialize();
    void serialize();

    // ============================================================================
    // Commands
    // ============================================================================
    void exitCmd();
    void removeCmd();
    void searchCmd();
    void addCmd();

private:
    std::vector<Record> book;
    std::size_t total_entries { 0 } ;
    bool running { true };
    static constexpr const char *database_filename = "records.txt";

    const std::unordered_map<std::string, void (AddressBook::*)()> cmdMap {
        { "/exit", &AddressBook::exitCmd },
        { "/search", &AddressBook::searchCmd },
        { "/add", &AddressBook::addCmd },
        { "/remove", &AddressBook::removeCmd }
    };

    /**
     * This array holds functions to search based on the choice of key by the user.
     * 0 -> First Name
     * 1 -> Other Names
     * ...
     */
    const std::array<std::function<bool(const Record&, const std::string& search_term)>, 6> search_functions = {
        [](const Record &r, const std::string& search_term) { return r.first_name == search_term; },
        [](const Record &r, const std::string& search_term) { 
            return std::find(r.other_names.cbegin(), r.other_names.cend(), search_term) != r.other_names.cend(); },
        [](const Record &r, const std::string& search_term) { return r.email == search_term; },
        [](const Record &r, const std::string& search_term) { return r.tel_num == search_term; },
        [](const Record &r, const std::string& search_term) { return r.address.town == search_term; },
        [](const Record &r, const std::string& search_term) { return r.address.country == search_term; }
    };
};
