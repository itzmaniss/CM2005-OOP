#pragma once
#include "UserWallet.h"
#include <vector>
#include <string>

struct UserRecord { std::string username, fullName, email, salt, passwordHash; };

class UserManager {
public:
    /** Need wallet to give bonus. */
    UserManager(UserWallet& wallet);
    /** Ask user details and make new account. */
    bool registerUser();
    /** Create account without asking (useful for tests). */
    bool registerUserProgrammatic(const std::string& fullName, const std::string& email, const std::string& password);
    /** Get the username that just got made. */
    std::string getLatestUsername() const;
    /** Ask for email and password to log in. */
    bool login(std::string& outUsername);
    /** Change password if user forgot it. */
    bool resetPassword();
private:
    /** Make some random string for security. */
    static std::string generateSalt();
    /** Scramble password so nobody can read it. */
    static std::string hashPassword(const std::string& pw,
                                    const std::string& salt);
    /** Make a new username that nobody use yet. */
    std::string generateUniqueUsername();
    /** Turn CSV line into user record. */
    static UserRecord stringToUr(const std::string& line);
    /** Load all users from file. */
    std::vector<UserRecord> loadAll();
    /** Add one user to the CSV file. */
    void appendToFile(const UserRecord& rec);
    /** Save everything back to file. */
    void rewriteFile();
    /** Check if email or name already used. */
    bool exists(std::string& email, std::string& fullName);
    /** Print message and wait for user to type. */
    std::string readLine(const std::string& prompt);
    /** Find user by their username. */
    UserRecord* findByUsername(const std::string& username);
    /** Make sure CSV file exists. */
    void ensureFilesExist();

    const std::string users_file = "data/USERS_REGISTER.CSV";
    std::vector<UserRecord> users;
    UserWallet& wallet;
    static constexpr double SIGNUP_BONUS = 1000.0;
};
