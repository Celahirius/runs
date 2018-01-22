#include "headers.cpp"

int main(int argc, char* argv[]) {
    int output = 0;
    try {
        while(output == 0) {
            connection C(loginDetails());
            checkConnectionClass<bool> check;
            if (check.checkConnection(&C)) {
                user *loggedUser = new user;
                vector<string> loggedUserInfo;
                string loggedUserID = "ERROR", username, role;

                while (loggedUserID == "ERROR") {
                    loggedUserInfo = loggedUser->login();
                    loggedUserID = loggedUserInfo[0];
                }

                username = loggedUserInfo[1];
                role = loggedUserInfo[2];

                cout << "Welcome " << username << "." << endl;

                if (role == "1") {
                    admin *userAdmin = new admin;
                    userAdmin->role = stoi(role);
                    userAdmin->username = username;
                    userAdmin->userID = loggedUserID;
                    output = commandInterface<admin>(userAdmin);
                } else if (role == "2") {
                    runner *userRunner = new runner;
                    userRunner->role = stoi(role);
                    userRunner->username = username;
                    userRunner->userID = loggedUserID;
                    output = commandInterface<runner>(userRunner);
                } else if (role == "3") {
//                    secretary *userSecretary = new secretary;
//                    userSecretary->role = stoi(role);
//                    userSecretary->username = username;
//                    userSecretary->userID = loggedUserID;
//                    output = commandInterface<secretary>(userSecretary);
                }
            }
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "ERROR in main.cpp: runtime error:" << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "ERROR in main.cpp: error: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "ERROR in main.cpp: an unknown error occurred." << std::endl;
    }
    return output;
}
