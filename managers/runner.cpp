//
// Created by FGlowacki on 17.01.18.
//

#include "runner.h"


void runner::runCommand(vector <string> parsedCommand) {
    if(parsedCommand.size() == 4 && parsedCommand[1] == "find") {
        this->findRuns(parsedCommand[2], parsedCommand[3]);
    } else if(parsedCommand.size() == 3 && parsedCommand[1] == "find") {
        this->findRuns(this->userID, parsedCommand[2]);
    } else if(parsedCommand.size() == 3 && parsedCommand[1] == "show") {
        this->showRuns(parsedCommand[2]);
    } else if(parsedCommand.size() == 2 && parsedCommand[1] == "show") {
        this->showRuns(this->userID);
    } else if(parsedCommand.size() == 3 && parsedCommand[1] == "count") {
        this->countRuns(parsedCommand[2]);
    } else if(parsedCommand[1] == "edit" || parsedCommand[1] == "delete" || parsedCommand[1] == "new") {
        cout << "You do not have permission to use that command." << endl;
    } else {
        cout << "run:\n"
                "   run find <runner_id> <run_id> \n"
                        "   run find <run_id> \n"
                "   run show <runner_id> \n"
                        "   run show \n"
                "   run count <runner_id> \n"
                "   run new \n"
                "   run delete <username> \n"
                "   run edit <username>" << endl;
    }
}