#include "commands.h"

//parsowanie komendy
vector<string> CommandManager::commandParser(string cmd) {
    vector<string> parsedCommand;
    parsedCommand.clear();
    string commandPart="";
    for(int i=0; i<cmd.size()+1; i++) {
        if(cmd[i]==' ' || i==cmd.size()) {
            //cout<<commandPart<<endl;
            if(commandPart!="") parsedCommand.push_back(clearWhite(commandPart));
            commandPart="";
        }
        else {
            commandPart += cmd[i];
        }
    } return parsedCommand;
}

//przeszukanie tablicy z komendami i przypasowanie komendzie jej numeru ID
int CommandManager::searchMenu(string option, string *menu, int n){
    for(int i = 0; i < n; i++)
        if(option == menu[i])
            return i;
    //cerr << "Nieznane polecenie\n Lista polecen - wpisz help \n";
    return -1;
}
//zapisywanie historii komend
bool CommandManager::saveCommandHistory(string cmd, vector <string>& previousCommands) {
    previousCommands.push_back(cmd);
    if(previousCommands.size()>20) previousCommands.erase(previousCommands.begin());
    return 1;
}
//wypisanie historii komend
bool CommandManager::showCommandHistory(int n, vector <string>* previousCommands) {
    if(n == 0) n = 20;
    if(n > previousCommands->size()) n = previousCommands->size();
    for(int i=0; i<n; i++) {
        cout<<" "<<(*previousCommands)[i]<<endl;
    }
    return 1;
}

void CommandManager::showRuns(string runner_id) {
        vector <vector<string>> res;
        vector <string> row;
        string sql;
        connection C(loginDetails());
        nontransaction N(C);
        sql = "SELECT R.RUN_ID, R.NAME, S.SCORE, S.TIME DATE FROM RUN AS R, SCORE AS S, RUNNER AS U WHERE R.RUN_ID = S.RUN_ID AND S.RUNNER_ID = U.RUNNER_ID AND U.RUNNER_ID = " + N.esc(runner_id) + ";";
        result R(N.exec(convert(sql)));
        for (result::const_iterator c = R.begin(); c != R.end(); c++) {
            row.clear();
            for (int i = 0; i < c.size(); i++) {
                row.push_back(clearWhite(c[i].c_str()));
                if (row[i].size() > this->longestField[i]) {
                    this->longestField[i] = row[i].size();
                }
            }
            res.push_back(row);
        }
        for (int i = 0; i < 4; i++) {
            cout << this->header[i];
            for (int j = 0; j < this->longestField[i] - this->header[i].size(); j++) {
                cout << " ";
            }
            cout << "  " << flush;
        }
        cout << endl;
        for (int i = 0; i < res.size(); i++) {
            for (int j = 0; j < res[i].size(); j++) {
                cout << res[i][j];
                for(int k = 0; k < this->longestField[j] - res[i][j].size(); k++) {
                    cout << " ";
                }
                cout << "  " << flush;
            }
            cout << endl;
        }
}

void CommandManager::findRuns(string runner_id, string search_frase) {
        vector <vector<string>> res;
        vector <string> row;
        string sql;
        connection C(loginDetails());
        nontransaction N(C);
    // OR R.DATE = to_date('" + N.esc(search_frase) + "', 'DD.MM.YYYY')
        sql = "SELECT R.RUN_ID, R.NAME, S.SCORE, S.TIME DATE FROM RUN AS R, SCORE AS S, RUNNER AS U WHERE R.RUN_ID = S.RUN_ID AND S.RUNNER_ID = U.RUNNER_ID AND U.RUNNER_ID = " + N.esc(runner_id) + " AND (R.NAME ~* '.*" + N.esc(search_frase) + ".*' OR U.NAME ~* '.*" + N.esc(search_frase) + ".*' OR U.SURNAME ~* '.*" + N.esc(search_frase) + ".*');";
        result R(N.exec(convert(sql)));
        for (result::const_iterator c = R.begin(); c != R.end(); c++) {
            row.clear();
            for (int i = 0; i < c.size(); i++) {
                row.push_back(clearWhite(c[i].c_str()));
                if (row[i].size() > this->longestField[i]) {
                    this->longestField[i] = row[i].size();
                }
            }
            res.push_back(row);
        }
        for (int i = 0; i < 4; i++) {
            cout << this->header[i];
            for (int j = 0; j < this->longestField[i] - this->header[i].size(); j++) {
                cout << " ";
            }
            cout << "  " << flush;
        }
        cout << endl;
        for (int i = 0; i < res.size(); i++) {
            for (int j = 0; j < res[i].size(); j++) {
                cout << res[i][j];
                for(int k = 0; k < this->longestField[j] - res[i][j].size(); k++) {
                    cout << " ";
                }
                cout << "  " << flush;
            }
            cout << endl;
        }
}

int CommandManager::countRuns(string runner_id) {
    connection C(loginDetails());
    nontransaction N(C);
    string sql = "SELECT COUNT(DISTINCT RUN_ID) FROM SCORE WHERE RUNNER_ID = " + N.esc(runner_id) + ";";
    result R( N.exec( convert(sql) ));
    int count = R.begin()[0].as<int>();
    cout << count << endl;
    return count;
}

string CommandManager::countAverageTime(vector <string> parsedCommand) {
    string sql = "SELECT TO_CHAR((SUM(EXTRACT(EPOCH FROM TIME))/COUNT(*) || ' second')::interval, 'HH24:MI:SS') FROM SCORE WHERE RUNNER_ID = " + parsedCommand[1] + ";";
//    cout << sql << endl;
    connection C(loginDetails());
    nontransaction N(C);
    result R( N.exec( convert(sql) ));
    string time = clearWhite(R.begin()[0].as<string>());
    cout << time << endl;
    return time;
}

void CommandManager::userCommand(vector <string> parsedCommand) {
    cout << "You do not have permission to use that command." << endl;
}

void CommandManager::scoreCommand(vector <string> parsedCommand) {
    cout << "You do not have permission to use that command." << endl;
}

void CommandManager::runCommand(vector <string> parsedCommand) {
    if(parsedCommand.size() == 4 && parsedCommand[1] == "find") {
        this->findRuns(parsedCommand[2], parsedCommand[3]);
    } else if(parsedCommand.size() == 3 && parsedCommand[1] == "show") {
        this->showRuns(parsedCommand[2]);
    } else if(parsedCommand.size() == 3 && parsedCommand[1] == "count") {
        this->countRuns(parsedCommand[2]);
    } else if(parsedCommand[1] == "edit" || parsedCommand[1] == "delete" || parsedCommand[1] == "new") {
        cout << "You do not have permission to use that command." << endl;
    } else {
        cout << "run:\n"
                "   run new \n"
                "   run delete <username> \n"
                "   run edit <username>" << endl;
    }
}