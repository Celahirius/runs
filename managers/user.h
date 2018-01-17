//
// Created by FGlowacki on 08.01.18.
//

#ifndef CLINIC_USER_H
#define CLINIC_USER_H

class user : public CommandManager{
private:
    int logged_user_id;
public:
    int role;
    string username;
    string userID;

    user() {}
    ~user() {}

    virtual vector<string> login();
    bool logout();
    bool changePassword();

};

#endif //CLINIC_USER_H
