//
// Created by FGlowacki on 08.01.18.
//

#include "admin.h"

int admin::createUser() {
    connection C(loginDetails());
    work W(C);
    string username, password, role;
    cout << "Username:" << endl;
    cin >> username;
    cout << "Password:" << endl;
    cin >> password;
    cout << "Role (1 - admin, 2 - runner):" << endl;
    cin >> role;

    string sql = "INSERT INTO ACCOUNT (USERNAME, PASSWORD, ROLE_ID) VALUES ("
                 + string("'") + W.esc(username)
                 + "', '" + W.esc(password)
                 + "', " + W.esc(role)
                 + ");";
    W.exec( convert(sql) );

    if(role == "2") {
        string name, surname;
        cout << "Name:" << endl;
        cin >> name;
        cout << "Surname:" << endl;
        cin >> surname;

        sql = "INSERT INTO RUNNER (USER_ID, NAME, SURNAME) VALUES ("
              + string("(SELECT currval('account_id_seq'))")
                     + ", '" + W.esc(name)
                     + "', '" + W.esc(surname) + "');";
        W.exec( convert(sql) );
    }
    W.commit();

    sql = "SELECT currval('account_id_seq');";
    nontransaction N(C);
    result R = N.exec( convert(sql) );
    int USER_ID = R.begin()[0].as<int>();

    return USER_ID;
}

bool admin::deleteUser(string account_id) {
    connection C(loginDetails());
    nontransaction N(C);

    string sql = "DELETE FROM ACCOUNT WHERE ACCOUNT_ID = " + account_id + ";";
    N.exec( convert(sql) );

    return 1;
}

bool admin::updateUser(string account_id) {
    connection C(loginDetails());
    nontransaction N(C);
    string username_new, password_new, role_new;
    cout << "New username:" << endl;
    getline(cin, username_new);
    cout << "New password:" << endl;
    getline(cin, password_new);

    string sql = "UPDATE ACCOUNT SET"
                 + ((username_new != "")? " USERNAME = '" +  N.esc(username_new) + "'" : "")
                 + ((password_new != "")? " PASSWORD = '" +  N.esc(password_new) + "'" : "")
                 + " WHERE ACCOUNT_ID = " + account_id + ";";
    N.exec( convert(sql) );

    sql = "SELECT ROLE_ID FROM ACCOUNT WHERE ACCOUNT_ID = " + account_id + ";";
    result R = N.exec( convert(sql) );
    string role = R.begin()[0].as<string>();

    if(role == "2") {
        string username_new, password_new;
        cout << "New name:" << endl;
        getline(cin, username_new);
        cout << "New surname:" << endl;
        getline(cin, password_new);

        string sql = "UPDATE RUNNER SET"
                     + ((username_new != "")? " NAME = '" +  N.esc(username_new) + "'" : "")
                     + ((password_new != "")? " SURNAME = '" +  N.esc(password_new) + "'" : "")
                     + " WHERE USER_ID = " + account_id + ";";
        N.exec( convert(sql) );
    }

    return 1;
}

bool admin::updateRunner(string runner_id) {
    connection C(loginDetails());
    nontransaction N(C);
    string username_new, password_new;
    cout << "New name:" << endl;
    getline(cin, username_new);
    cout << "New surname:" << endl;
    getline(cin, password_new);

    string sql = "UPDATE RUNNER SET"
                 + ((username_new != "")? " NAME = '" +  N.esc(username_new) + "'" : "")
                 + ((password_new != "")? " SURNAME = '" +  N.esc(password_new) + "'" : "")
                 + " WHERE RUNNER_ID = " + runner_id + ";";
    N.exec( convert(sql) );

    return 1;
}

int admin::insertRunnerScore(string runner_id, string run_id) {
    connection C(loginDetails());
    work W(C);
    string score, time;
    cout << "Score:" << endl;
    getline(cin, score);
    cout << "Time:" << endl;
    getline(cin, time);

    string sql = "INSERT INTO SCORE (RUNNER_ID, RUN_ID, SCORE, TIME, FINISHED_RUN) VALUES ("
                 + string("'") + W.esc(runner_id)
                 + "', '" + W.esc(run_id)
                 + "', '" + W.esc(score)
                 + "', '" + W.esc(time)
                 + "', TRUE"
                 + ");";
    W.exec( convert(sql) );
    W.commit();

    sql = "SELECT currval('score_id_seq');";
    nontransaction N(C);
    result R = N.exec( convert(sql) );
    int SCORE_ID = R.begin()[0].as<int>();

    return SCORE_ID;
}

bool admin::updateRunnerScore(string runner_id, string run_id) {
    connection C(loginDetails());
    nontransaction N(C);
    string score_new, time_new;
    cout << "Score:" << endl;
    getline(cin, score_new);
    cout << "Time:" << endl;
    getline(cin, time_new);

    string sql = "UPDATE SCORE SET"
                 + ((score_new != "")? " SCORE = '" +  N.esc(score_new) + "'" : "")
                 + ((time_new != "")? " TIME = '" +  N.esc(time_new) + "'" : "")
                 + " WHERE RUNNER_ID = " + runner_id + " AND  RUN_ID = " + run_id + ";";
    N.exec( convert(sql) );

    return 1;
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

    return 1;
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
                 + ((runname_new != "")? " NAME = '" +  N.esc(runname_new) + "'" : "")
                 + ((date_new != "")? " DATE = to_date('" +  N.esc(date_new) + "', 'DD.MM.YYYY')" : "")
                 + " WHERE RUN_ID = " + run_id + ";";
    N.exec( convert(sql) );

    return 1;
}

void admin::userCommand(vector <string> parsedCommand) {
    if(parsedCommand.size() == 2 && parsedCommand[1] == "new") {
        this->createUser();
    } else if(parsedCommand.size() == 3 && parsedCommand[1] == "delete") {
        this->deleteUser(parsedCommand[2]);
    } else if(parsedCommand.size() == 3 && parsedCommand[1] == "edit") {
        this->updateUser(parsedCommand[2]);
    } else if(parsedCommand.size() == 2 && parsedCommand[1] == "show") {
        this->showUsers();
    } else {
        cout << "user:\n"
                "   user new \n"
                "   user delete <username> \n"
                "   user edit <username> \n"
                "   user show <username>" << endl;
    }
}

void admin::scoreCommand(vector <string> parsedCommand) {
    if(parsedCommand.size() == 4 && parsedCommand[1] == "new") {
        this->insertRunnerScore(parsedCommand[2], parsedCommand[3]);
    } else if(parsedCommand.size() == 4 && parsedCommand[1] == "edit") {
        this->updateRunnerScore(parsedCommand[2], parsedCommand[3]);
    } else {
        cout << "score:\n"
                "   score new \n"
                "   score edit <username>" << endl;
    }
}

void admin::runCommand(vector <string> parsedCommand) {
    if(parsedCommand.size() == 4 && parsedCommand[1] == "find") {
        this->findRuns(parsedCommand[2], parsedCommand[3]);
    } else if(parsedCommand.size() == 3 && parsedCommand[1] == "show") {
        this->showRuns(parsedCommand[2]);
    } else if(parsedCommand.size() == 3 && parsedCommand[1] == "count") {
        this->countRuns(parsedCommand[2]);
    } else if(parsedCommand.size() == 2 && parsedCommand[1] == "new") {
        this->createRun();
    } else if(parsedCommand.size() == 3 && parsedCommand[1] == "delete") {
        this->deleteRun(parsedCommand[2]);
    } else if(parsedCommand.size() == 3 && parsedCommand[1] == "edit") {
        this->updateRun(parsedCommand[2]);
    } else {
        cout << "run:\n"
                "   run find <runner_id> <run_id> \n"
                "   run show <runner_id> \n"
                "   run count <runner_id> \n"
                "   run new \n"
                "   run delete <username> \n"
                "   run edit <username>" << endl;
    }
}

void admin::showUsers() {

        vector <vector<string>> res;
        vector <string> row;
        string sql;
        connection C(loginDetails());
        nontransaction N(C);
        sql = "SELECT RUNNER_ID, USER_ID, NAME, SURNAME, (SELECT COUNT(DISTINCT RUN_ID) FROM SCORE AS S WHERE S.RUNNER_ID = R.RUNNER_ID), (SELECT TO_CHAR((SUM(EXTRACT(EPOCH FROM TIME))/COUNT(*) || ' second')::interval, 'HH24:MI:SS') FROM SCORE AS S WHERE S.RUNNER_ID = R.RUNNER_ID) FROM RUNNER AS R;";
        result R(N.exec(convert(sql)));
        for (result::const_iterator c = R.begin(); c != R.end(); c++) {
            row.clear();
            for (int i = 0; i < c.size(); i++) {
                row.push_back(clearWhite(c[i].c_str()));
                if (row[i].size() > this->runnerLongestField[i]) {
                    this->runnerLongestField[i] = row[i].size();
                }
            }
            res.push_back(row);
        }
        for (int i = 0; i < 6; i++) {
            cout << this->runnerHeader[i];
            for (int j = 0; j < this->runnerLongestField[i] - this->runnerHeader[i].size(); j++) {
                cout << " ";
            }
            cout << "  " << flush;
        }
        cout << endl;
        for (int i = 0; i < res.size(); i++) {
            for (int j = 0; j < res[i].size(); j++) {
                cout << res[i][j];
                for(int k = 0; k < this->runnerLongestField[j] - res[i][j].size(); k++) {
                    cout << " ";
                }
                cout << "  " << flush;
            }
            cout << endl;
        }
}