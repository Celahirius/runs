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

void CommandManager::showRuns(vector <string> parsedCommand) {
        vector <vector<string>> res;
        vector <string> row;
        string sql;
        connection C(loginDetails());
        nontransaction N(C);
        sql = "SELECT R.RUN_ID, R.NAME, S.SCORE, S.TIME DATE FROM RUN AS R, SCORE AS S, RUNNER AS U WHERE R.RUN_ID = S.RUN_ID AND S.RUNNER_ID = U.RUNNER_ID AND U.RUNNER_ID = " + parsedCommand[2] + ";";
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

void CommandManager::findRuns(vector <string> parsedCommand) {
        vector <vector<string>> res;
        vector <string> row;
        string sql;
        connection C(loginDetails());
        nontransaction N(C);
        sql = "SELECT R.RUN_ID, R.NAME, S.SCORE, S.TIME DATE FROM RUN AS R, SCORE AS S, RUNNER AS U WHERE R.RUN_ID = S.RUN_ID AND S.RUNNER_ID = U.RUNNER_ID AND U.RUNNER_ID = " + parsedCommand[2] + " AND (R.NAME ~* '.*" + N.esc(parsedCommand[3]) + ".*' OR R.DATE = to_date('" + N.esc(parsedCommand[3]) + "', 'DD.MM.YYYY') OR U.NAME ~* '.*" + N.esc(parsedCommand[3]) + ".*' OR U.SURNAME ~* '.*" + N.esc(parsedCommand[3]) + ".*');";
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

int CommandManager::countRuns(vector <string> parsedCommand) {
    string sql = "SELECT COUNT(*) FROM RUN;";
    connection C(loginDetails());
    nontransaction N(C);
    result R( N.exec( convert(sql) ));
    int count = R.begin()[0].as<int>();
    return count;
}

string CommandManager::countAverageTime(vector <string> parsedCommand) {
    string sql = "SELECT TO_CHAR(((SUM(EXTRACT(EPOCH FROM TIME))/COUNT(*)) || ' second')::interval, 'HH:MI:SS') FROM SCORE WHERE RUNNER_ID = " + parsedCommand[2] + ";";
    connection C(loginDetails());
    nontransaction N(C);
    result R( N.exec( convert(sql) ));
    string time = clearWhite(R.begin()[0].as<string>());
    return time;
}

void CommandManager::userCommand(vector <string> parsedCommand) {
    cout << "You do not have permission to use that command." << endl;
}