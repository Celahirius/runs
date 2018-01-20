//
// Created by FGlowacki on 13.11.17.
//

#ifndef LIBRARY_COMMANDS_H
#define LIBRARY_COMMANDS_H

class CommandManager {
protected:
    int longestField[4];
    string header[4];

public:
    friend class myLittlePony;

    CommandManager() : longestField{6, 4, 5, 4}, header{"Run ID", "Name", "Score", "Time"} {}
    ~CommandManager() {}

    vector<string> commandParser(string cmd);
    int searchMenu(string option, string *menu, int n);
    bool saveCommandHistory(string cmd, vector <string>& previousCommands);
    bool showCommandHistory(int n, vector <string>* previousCommands);
    virtual void showRuns(vector <string> parsedCommand);
    virtual void findRuns(vector <string> parsedCommand);
    virtual int countRuns(vector <string> parsedCommand);
    virtual string countAverageTime(vector <string> parsedCommand);
    virtual void userCommand(vector <string> parsedCommand);
    virtual void scoreCommand(vector <string> parsedCommand);
    virtual void runCommand(vector <string> parsedCommand);
};

#endif //LIBRARY_COMMANDS_H
