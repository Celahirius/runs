//
// Created by FGlowacki on 08.01.18.
//

#ifndef CLINIC_ADMIN_H
#define CLINIC_ADMIN_H

class admin : public user{
//    doctor* doctorPrivileges;
//    secretary* secretaryPrivileges;
public:
    admin() {}
    ~admin() {}

    int createUser();
    bool deleteUser(string username);
    bool updateUser(string username);
    int createRun();
    bool deleteRun(string run_id);
    bool updateRun(string run_id);
    bool updateRunner(string runner_id);
    int insertRunnerScore(string runner_id, string run_id);
    bool updateRunnerScore(string runner_id, string run_id);
    virtual void userCommand(vector <string> parsedCommand);
    virtual void scoreCommand(vector <string> parsedCommand);
    virtual void runCommand(vector <string> parsedCommand);
};

#endif //CLINIC_ADMIN_H
