//
// Created by FGlowacki on 08.01.18.
//

#include "user.h"

vector<string> user::login() {
    int loggedin = 0;
    vector<string> loggedUser;
    string ACCOUNT_ID = "";
    string username, password;
    cout << "Username:" << endl;
//    cin >> username;
    getline(cin, username);
    cout << "Password:" << endl;
//    cin >> password;
    getline(cin, password);

    if(password != "" && username != "") {
        connection C(loginDetails());
        nontransaction N(C);
        string sql = "SELECT ACCOUNT_ID, USERNAME, ROLE_ID FROM ACCOUNT WHERE USERNAME = '" + N.esc(username) +
                     "' AND PASSWORD = '" + N.esc(password) + "';";
        result R(N.exec(convert(sql)));
        if (R.empty()) {
            loggedUser.push_back("ERROR");
            cerr << "ERROR: USER LOGIN: logowanie nie powiodlo sie." << endl;
        } else {
            loggedUser.push_back(R.begin()[0].as<string>());
            loggedUser.push_back(R.begin()[1].as<string>());
            loggedUser.push_back(R.begin()[2].as<string>());
            ACCOUNT_ID = R.begin()[0].as<string>();
            this->logged_user_id = R.begin()[0].as<int>();
            sql = "UPDATE ACCOUNT SET LOGGED_IN = TRUE WHERE ACCOUNT_ID = " + N.esc(ACCOUNT_ID) + ";";
            N.exec(convert(sql));
        }
    } else {
        loggedUser.push_back("EMPTY");
    }

    return loggedUser;
}

bool user::logout() {
    bool loggedout = 0;
    string ACCOUNT_ID;
    connection C(loginDetails());
    nontransaction N(C);
    string sql = "SELECT ACCOUNT_ID FROM ACCOUNT WHERE ACCOUNT_ID = " + N.esc(this->userID) + ";";
    result R( N.exec( convert(sql) ));
    if(R.empty()) {
        loggedout = 0;
        cerr << "ERROR: USER LOGOUT: wylogowywanie nie powiodlo sie." << endl;
    } else {
        loggedout = 1;
        ACCOUNT_ID = R.begin()[0].as<string>();
        sql = "UPDATE ACCOUNT SET LOGGED_IN = FALSE WHERE ACCOUNT_ID = " + N.esc(ACCOUNT_ID) + ";";
        N.exec( convert(sql) );
    }

    return loggedout;
}

bool user::changePassword() {
    bool password_changed = 0;
    string ACCOUNT_ID = "";
    string newPassword;
    cout << "New password:" << endl;
    cin >> newPassword;

    connection C(loginDetails());
    nontransaction N(C);
    string sql = "SELECT ACCOUNT_ID FROM ACCOUNT WHERE ACCOUNT_ID = " + N.esc(conv_num(this->logged_user_id)) + ";";
    result R( N.exec( convert(sql) ));
    if(!R.empty()) {
        password_changed = 0;
        cerr << "ERROR: USER CHANGE PASSWORD: zmiana hasla sie nie powiodla." << endl;
    } else {
        password_changed = 1;
        ACCOUNT_ID = R.begin()[0].as<string>();
        sql = "UPDATE ACCOUNT SET PASSWORD = " + N.esc(newPassword) + " WHERE ACCOUNT_ID = " + N.esc(ACCOUNT_ID) + ";";
        N.exec( convert(sql) );
    }

    return password_changed;
}