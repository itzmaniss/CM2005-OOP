#include "UserManager.h"
#include "UserWallet.h"
#include <iostream>
#include <functional>
#include <random>
#include <iomanip>
#include <sstream>
#include <string>
#include <limits>
#include <fstream>

#include "CSVReader.h"


UserManager::UserManager(UserWallet& wallet) : wallet(wallet) {
    ensureFilesExist();
    loadAll(); 
};


bool UserManager::registerUser() {
    std::cout << "--- Register ---" << std::endl;
    std::string fullName = readLine("Full name: ");
    std::string email    = readLine("Email: ");
    std::string password = readLine("Password: ");

    return registerUserProgrammatic(fullName, email, password);
}

bool UserManager::registerUserProgrammatic(const std::string& fullName, 
                                          const std::string& email, 
                                          const std::string& password) {
    if (fullName.empty() || email.empty() || password.empty()) {
        std::cout << "All fields are required." << std::endl;
        return false;
    }
    if (email.find('@') == std::string::npos) {
        std::cout << "That does not look like a valid email." << std::endl;
        return false;
    }

    if (fullName.find(',') != std::string::npos ||
        email.find(',')    != std::string::npos ||
        password.find(',') != std::string::npos) {
        std::cout << "Commas are not allowed in any field." << std::endl;
        return false;
    }


    if (exists(const_cast<std::string&>(email), const_cast<std::string&>(fullName))) {
        std::cout << "An account with this name and email already exists." << std::endl;
        return false;
    }

    std::string username = generateUniqueUsername();
    std::string salt     = generateSalt();
    std::string hash     = hashPassword(password, salt);

    UserRecord rec{username, fullName, email, salt, hash};
    users.push_back(rec);
    appendToFile(rec);

    wallet.createAccount(username, SIGNUP_BONUS);

    std::cout << ">>> Registered! Your username is: " << username
              << " - keep it safe to log in. <<<" << std::endl;
    return true;
}

std::string UserManager::getLatestUsername() const {
    if (users.empty()) return "";
    return users.back().username;
}


bool UserManager::login(std::string& outUsername) {
    std::cout << "--- Login ---" << std::endl;
    for (int attempt = 1; attempt <= 3; ++attempt) {
        std::string username = readLine("Username: ");
        std::string password = readLine("Password: ");

        UserRecord* rec = findByUsername(username);
        if (rec != nullptr && hashPassword(password, rec->salt) == rec->passwordHash) {
            outUsername = username;
            std::cout << "Welcome back, " << rec->fullName << "!" << std::endl;
            return true;
        }
        std::cout << "Invalid credentials (attempt " << attempt << " of 3)." << std::endl;
    }
    return false;
}


bool UserManager::resetPassword() {
    std::cout << "--- Reset password ---" << std::endl;
    std::cout << "Confirm your identity." << std::endl;
    std::string username = readLine("Username: ");
    std::string email    = readLine("Email: ");

    UserRecord* rec = findByUsername(username);
    if (rec == nullptr || rec->email != email) {
        std::cout << "Details do not match our records." << std::endl;
        return false;
    }

    std::string newPass = readLine("New password: ");
    std::string confirm = readLine("Confirm new password: ");
    if (newPass.empty()) {
        std::cout << "Password cannot be empty." << std::endl;
        return false;
    }
    if (newPass != confirm) {
        std::cout << "Passwords do not match." << std::endl;
        return false;
    }

    rec->salt         = generateSalt();
    rec->passwordHash = hashPassword(newPass, rec->salt);
    rewriteFile();
    return true;
}

std::string  UserManager::generateSalt() {
    std::random_device rd;
    std::mt19937_64 gen(rd());

    std::uniform_int_distribution<uint64_t> dist(0, std::numeric_limits<uint64_t>::max());

    const uint64_t salt_value = dist(gen);

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(16) << std::hex << salt_value;

    return ss.str();
};

std::string  UserManager::hashPassword(const std::string& pw, const std::string& salt) {
    return std::to_string( std::hash<std::string>{}(salt + pw) );
};
std::string UserManager::generateUniqueUsername() {
    thread_local std::random_device rd;
    thread_local std::mt19937_64 gen(rd());
    std::uniform_int_distribution<long long> distr(1000000000LL, 9999999999LL);

    while (true) {
        std::string candidate = std::to_string(distr(gen));

        bool taken = false;
        for (const UserRecord& r : users) {   // this->users
            if (r.username == candidate) { taken = true; break; }
        }
        if (!taken) return candidate;
    }
}

std::vector<UserRecord> UserManager::loadAll() {

    std::ifstream csvFile{this->users_file};
    this->users.clear();

    if (csvFile.is_open())
    {
        std::string line;
        while(std::getline(csvFile, line))
        {
            try {

                UserRecord ur = stringToUr(line);
                this->users.push_back(ur);
            }catch(const std::exception& e)
            {
                std::cout << "UserManager::loadAll bad data"  << std::endl;
            }
        }
    }
    else {
        std::cerr << "UserManager::loadAll bad file: " << this->users_file << std::endl;
    }

    std::cout << "UserManager::loadAll read " << this->users.size() << "entries"  << std::endl;
    return this->users;
};

UserRecord  UserManager::stringToUr(const std::string& line) {
    std::vector<std::string> const tokens = CSVReader::tokenise(line, ',');
    if (tokens.size() != 5)
    {
        std::cout << "Bad line " << std::endl;
        throw std::exception{};
    }


    UserRecord ur = {tokens[0],
        tokens[1],
        tokens[2],
        tokens[3],
        tokens[4]
    };
    return ur;
};
void UserManager::appendToFile(const UserRecord& rec) {
    std::ofstream out{this->users_file, std::ios::app};
    if (!out.is_open()) {
        std::cerr << "UserManager::appendToFile bad file: " << users_file << std::endl;
        return;
    }
    out << rec.username << "," << rec.fullName << "," << rec.email << ","
        << rec.salt << "," << rec.passwordHash << "\n";
}

void UserManager::rewriteFile() {
    std::ofstream out{this->users_file};
    if (!out.is_open()) {
        std::cerr << "UserManager::rewriteFile bad file: " << users_file << std::endl;
        return;
    }
    for (const UserRecord& r : this->users) {
        out << r.username << ","
            << r.fullName << ","
            << r.email    << ","
            << r.salt     << ","
            << r.passwordHash << "\n";
    }
}
bool  UserManager::exists(std::string& email, std::string& fullName) {
    for (const UserRecord& r : users) {   // this->users
        if (r.email == email && r.fullName == fullName) { return true; }
    }
    return false;
};


std::string UserManager::readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    size_t start = line.find_first_not_of(" \t\r\n");
    size_t end   = line.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    return line.substr(start, end - start + 1);
}

UserRecord* UserManager::findByUsername(const std::string& username) {
    for (UserRecord& r : users) {
        if (r.username == username) return &r;
    }
    return nullptr;
}


void UserManager::ensureFilesExist() {
    std::ifstream check{users_file};
    if (!check.is_open()) {
        std::ofstream create{users_file};
    }
}