template <class userType>
int commandInterface(userType* loggedUser) {

    string cmd = "";
    string menu1[] = {"q", "showruns", "findruns", "help", "countruns", "averagetime", "history", "user", "changepasswd", "logout", "score", "runner"}; //lista komend; uzywana przez funkcje searchMenu()
    vector <string> parsedCommand; //sparsowana komenda
    vector <string> previousCommands; //vector poprzednich komend

    userType* command = loggedUser;

    while(true) {
        try {
            cout << "clinic$ " << flush;
            getline(cin, cmd);

            if(cmd != "") {
                parsedCommand = command->commandParser(cmd); //parsowanie komendy
                command->saveCommandHistory(cmd, previousCommands); //zapisanie komendy
                switch (command->searchMenu(parsedCommand[0], menu1, 12)) { //wyszukanie numeru ID polecenia i wywolanie polecen
                    case 0:
                        //quit
                        command->logout();
                        return 1;
                    case 1:
                        command->showRuns(parsedCommand);
                        break;
                    case 2:
                        command->findRuns(parsedCommand);
                        break;
                    case 3:
                        cout << "Lista komend: " << endl;

                        cout << "changepasswd"<< endl;
                        cout << "logout"<< endl;
                        cout << "history n - liczba ostatnich polecen do wyswietlenia (n musi byc od 1 do 20)" << endl;
                        break;
                    case 4:
                        command->countRuns(parsedCommand);
                        break;
                    case 5:
                        command->countAverageTime(parsedCommand);
                        break;
                    case 6:
                        if(parsedCommand.size() == 1)
                            command->showCommandHistory(20, &previousCommands);
                        else if(parsedCommand.size() == 2) {
                            if(atoi(parsedCommand[1].c_str()) < 1 || atoi(parsedCommand[1].c_str()) > 20) {
                                cout << "liczba polecen do wystwietlenie powinna byc z przedzialu od 1 do 20>";
                            } else command->showCommandHistory(atoi(parsedCommand[1].c_str()), &previousCommands);
                        } else {
                            cout << "history n - liczba ostatnich polecen do wyswietlenia (n musi byc od 1 do 20)" << endl;
                        }
                        break;
                    case 7:
                        command->userCommand(parsedCommand);
                        break;
                    case 8:
                        command->changePassword();
                        break;
                    case 9:
                        command->logout();
                        return 0;
                    case 10:
                        command->scoreCommand(parsedCommand);
                        return 0;
                    case 11:
                        command->runCommand(parsedCommand);
                        return 0;
                    default:
                        cerr << "Nieznane polecenie\n Lista polecen - wpisz help \n";
                        break;
                }
            }
            parsedCommand.clear();
            cmd="";
        } catch (const std::runtime_error& e) {
            std::cerr << "ERROR in commandInterface.cpp: runtime error:" << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "ERROR in commandInterface.cpp: error: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "ERROR in commandInterface.cpp: an unknown error occurred." << std::endl;
        }
    }
}
