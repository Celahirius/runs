//
// Created by FGlowacki on 17.01.18.
//

#ifndef CLINIC_RUNNER_H
#define CLINIC_RUNNER_H

class runner : public user {
//    doctor* doctorPrivileges;
//    secretary* secretaryPrivileges;
public:
    runner() {}
    ~runner() {}
    virtual void runCommand(vector <string> parsedCommand);
};

#endif //CLINIC_RUNNER_H
