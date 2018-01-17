//
// Created by FGlowacki on 13.11.17.
//

#ifndef LIBRARY_COMMANDS_H
#define LIBRARY_COMMANDS_H

class CommandManager {
protected:
//    DoctorManager* doctor;
//    PatientManager* patient;
//    SpecializationManager* specialization;
//    AppointmentManager *appointment;

public:
    friend class myLittlePony;

    CommandManager() {
    }
    ~CommandManager() {}

    vector<string> commandParser(string cmd);
    int searchMenu(string option, string *menu, int n);
    bool saveCommandHistory(string cmd, vector <string>& previousCommands);
    bool showCommandHistory(int n, vector <string>* previousCommands);
    virtual void showRuns(vector <string> parsedCommand);
    virtual void findRuns(vector <string> parsedCommand);
    virtual void countRuns(vector <string> parsedCommand);
    virtual void countAverageTime(vector <string> parsedCommand);
//    void add(vector <string> parsedCommand);
//    void edit(vector <string> parsedCommand);
//    void remove(vector <string> parsedCommand);
//    void find(vector <string> parsedCommand);
//    virtual void doctorCommand(vector <string> parsedCommand);
//    virtual void patientCommand(vector <string> parsedCommand);
//    virtual void specializationCommand(vector <string> parsedCommand);
//    virtual void appointmentCommand(vector <string> parsedCommand);
    virtual void userCommand(vector <string> parsedCommand);
    //    void showCommand(vector <string> parsedCommand);
//    void accountCommand(vector <string> parsedCommand);
//    void logoutCommand(vector <string> parsedCommand);
};

#endif //LIBRARY_COMMANDS_H
