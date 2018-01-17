//
// Created by FGlowacki on 08.01.18.
//

#include "admin.h"

int admin::createUser() {
    connection C(loginDetails());
    nontransaction N(C);
    string username, password, role;
    cout << "Username:" << endl;
    cin >> username;
    cout << "Password:" << endl;
    cin >> password;
    cout << "Role (1 - admin, 2 - runner):" << endl;
    cin >> role;

    string sql = "INSERT INTO ACCOUNT (USERNAME, PASSWORD, ROLE_ID) VALUES ("
                 + string("'") + N.esc(username)
                 + "', '" + N.esc(password)
                 + "', " + N.esc(role)
                 + ");";
    N.exec( convert(sql) );

    sql = "SELECT currval('account_id_seq');";
    result R( N.exec( convert(sql) ));
    int USER_ID = R.begin()[0].as<int>();

    return USER_ID;
}

bool admin::deleteUser(string account_id) {
    connection C(loginDetails());
    nontransaction N(C);

    string sql = "DELETE FROM ACCOUNT WHERE ACCOUNT_ID = " + account_id + ";";
    N.exec( convert(sql) );

//    return deleted;
}

bool admin::updateUser(string account_id) {
    connection C(loginDetails());
    nontransaction N(C);
    string username_new, password_new, role_new;
    cout << "New username:" << endl;
    getline(cin, username_new);
    cout << "New password:" << endl;
    getline(cin, password_new);
    cout << "New role (1 - admin, 2 - doctor, 3 - secretary):" << endl;
    getline(cin, role_new);

    string sql = "UPDATE ACCOUNT SET"
                 + ((username_new != "")? " USERNAME = '" +  N.esc(username_new) + "'" : "")
                 + ((password_new != "")? " PASSWORD = '" +  N.esc(password_new) + "'" : "")
                 + ((role_new != "")? " ROLE_ID = " +  N.esc(role_new) + "" : "")
                 + " WHERE ACCOUNT_ID = " + account_id + ";";
    N.exec( convert(sql) );

//    return updated;
}

int admin::createRun() {
    connection C(loginDetails());
    nontransaction N(C);
    string run_id, date;
    cout << "Run name:" << endl;
    cin >> run_id;
    cout << "Date:" << endl;
    cin >> date;

    string sql = "INSERT INTO RUN (NAME, DATE) VALUES ("
                 + string("'") + N.esc(run_id)
                 + "', to_date('" + N.esc(date) + "', 'DD.MM.YYYY'));";
    N.exec( convert(sql) );

    sql = "SELECT currval('run_id_seq');";
    result R( N.exec( convert(sql) ));
    int RUN_ID = R.begin()[0].as<int>();

    return RUN_ID;
}

bool admin::deleteRun(string run_id) {
    connection C(loginDetails());
    nontransaction N(C);

    string sql = "DELETE FROM RUN WHERE RUN_ID = " + run_id + ";";
    N.exec( convert(sql) );

//    return deleted;
}

bool admin::updateRun(string run_id) {
    connection C(loginDetails());
    nontransaction N(C);
    string runname_new, date_new;
    cout << "New run name:" << endl;
    getline(cin, runname_new);
    cout << "New run date:" << endl;
    getline(cin, date_new);

    string sql = "UPDATE RUN SET"
                 + ((runname_new != "")? " RUNNAME = '" +  N.esc(runname_new) + "'" : "")
                 + ((date_new != "")? " DATE = to_date('" +  N.esc(date_new) + "', 'DD.MM.YYYY')" : "")
                 + " WHERE RUN_ID = " + run_id + ";";
    N.exec( convert(sql) );

//    return updated;
}

void admin::userCommand(vector <string> parsedCommand) {
    if(parsedCommand.size() == 2 && parsedCommand[1] == "new") {
        this->createUser();
    } else if(parsedCommand.size() == 3 && parsedCommand[1] == "delete") {
        this->deleteUser(parsedCommand[2]);
    } else if(parsedCommand.size() == 3 && parsedCommand[1] == "edit") {
        this->updateUser(parsedCommand[2]);
    } else {
        cout << "user:\n"
                "   user new \n"
                "   user edit <username> \n"
                "   user update <username>" << endl;
    }
}