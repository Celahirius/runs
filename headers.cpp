#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <fstream>

using namespace std;

#include <pqxx/pqxx>    //biblioteka do Postgresa
#include <pqxx/pipeline.hxx>    //klasa do optymalizacji zapytan

using namespace pqxx;

#include "tools/string_tools.cpp"
#include "tools/sql_tools.cpp"

#include "interface/commands.cpp"
typedef CommandManager commands;

#include "managers/user.cpp"
#include "managers/runner.cpp"
#include "managers/admin.cpp"

#include "interface/commandInterface.cpp"
