void overloaded(int x) {cout<<x<<endl;}
void overloaded(int x, int y) {cout<<x+y<<endl;}

template <class userType>
int commandInterface(userType* loggedUser) {

    auto lambda =[]( int x, int y )->void { std::printf( "tu lambda. podane wartosci to: %d %d\n", x, y); };

    string cmd = "";
    string menu1[] = {"q", "help", "averagetime", "history", "user", "changepasswd", "logout", "score", "run", "extra", "averagerunners"}; //lista komend; uzywana przez funkcje searchMenu()
    vector <string> parsedCommand; //sparsowana komenda
    vector <string> previousCommands; //vector poprzednich komend

    userType* command = loggedUser;

    while(true) {
        try {
            cout << "run$ " << flush;
            getline(cin, cmd);

            if(cmd != "") {
                parsedCommand = command->commandParser(cmd); //parsowanie komendy
                command->saveCommandHistory(cmd, previousCommands); //zapisanie komendy
                switch (command->searchMenu(parsedCommand[0], menu1, 11)) { //wyszukanie numeru ID polecenia i wywolanie polecen
                    case 0:
                        //quit
                        command->logout();
                        return 1;
                    case 1:
                        cout << "Lista komend: " << endl;
                        cout << "score:\n"
                                "   score new \n"
                                "   score edit <username>" << endl;
                        cout << "run:\n"
                                "   run find <runner_id> <run_id> \n"
                                "   run show <runner_id> \n"
                                "   run count <runner_id> \n"
                                "   run new \n"
                                "   run delete <username> \n"
                                "   run edit <username>" << endl;
                        cout << "user:\n"
                                "   user new \n"
                                "   user delete <username> \n"
                                "   user edit <username> \n"
                                "   user show <username>" << endl;
                        cout << "averagetime"<< endl;
                        cout << "averagerunners"<< endl;
                        cout << "changepasswd"<< endl;
                        cout << "extra - lambda and overloaded functions"<< endl;
                        cout << "logout"<< endl;
                        cout << "history n - liczba ostatnich polecen do wyswietlenia (n musi byc od 1 do 20)" << endl;
                        break;
                    case 2:
                        command->countAverageTime();
                        break;
                    case 3:
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
                    case 4:
                        command->userCommand(parsedCommand);
                        break;
                    case 5:
                        command->changePassword();
                        break;
                    case 6:
                        command->logout();
                        return 0;
                    case 7:
                        command->scoreCommand(parsedCommand);
                        break;
                    case 8:
                        command->runCommand(parsedCommand);
                        break;
                    case 9:
                        lambda(1,2);
                        overloaded(1);
                        overloaded(1,2);
                        break;
                    case 10:
                        command->countAverageRunners();
                        break;
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
