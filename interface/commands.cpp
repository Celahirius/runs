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

}

void CommandManager::findRuns(vector <string> parsedCommand) {

}

int CommandManager::countRuns(vector <string> parsedCommand) {
    string sql = "SELECT COUNT(*) FROM RUN;";
    nontransaction N(*C);
    result R( N.exec( convert(sql) ));
    int count = clearWhite(R.begin()[0].as<int>());
    return count;
}

string CommandManager::countAverageTime(vector <string> parsedCommand) {
    string sql = "SELECT TO_CHAR(((SUM(EXTRACT(EPOCH FROM TIME))/COUNT(*)) || ' second')::interval, 'HH:MI:SS') FROM SCORE WHERE RUNNER_ID = " + parsedCommand[2] + ";";
    nontransaction N(*C);
    result R( N.exec( convert(sql) ));
    int time = clearWhite(R.begin()[0].as<string>());
    return time;
}

void CommandManager::userCommand(vector <string> parsedCommand) {
    cout << "You do not have permission to use that command." << endl;
}