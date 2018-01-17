//funkcje zwiazane z baza postgress

//sciaganie danych do polaczenia z Postgressem z pliku
char *loginDetails(){
    return convert("dbname=run user=postgres password=postgres hostaddr=127.0.0.1 port=5432");
}

template <class userType>
class checkConnectionClass {
public:
    //sprawdzanie polacznia z baza
    userType checkConnection(connection *C){
        if(C -> is_open())
            return 1;
        else {
            cerr << "ERROR: There is no connection to the database.\n";
            return 0;
        }
    }
};

bool checkConnection(connection *C){
    if(C -> is_open())
        return 1;
    else {
        cerr << "Blad polaczenia SQL\n";
        return 0;
    }
}

//zliczanie rekordow w podanej tabeli
int countSQL(connection *C, string from){
    string sql = "SELECT count(*) from " + from;
    //cout << sql << "\n";
    nontransaction N(*C);
    result R( N.exec( convert(sql) ));
    return (*R.begin())[0].as<int>();
}

//pobranie z zadanej tabeli sumy wybranej kolumny
string downloadSum(string tableName, string columnName, connection *C) {
    string sql = "SELECT SUM(" + tableName + ".\"" + columnName + "\") FROM " + tableName;
    nontransaction N(*C);
    result R( N.exec( convert(sql) ));
    string columnSum = clearWhite(R.begin()[0].c_str());

    return columnSum;
}

//zapytanie o wartosc z bazy zawierajace klauzule WHERE
pipeline::query_id downloadValueWhere(string tableName, string columnName, string whereColumnName, string whereValue, pipeline *P) {
    string sql = "SELECT " + tableName + ".\"" + columnName +
                 "\" FROM " + tableName + " WHERE " + tableName + ".\"" + whereColumnName + "\" = " + whereValue;
    return(P -> insert(sql));
}

//zapytanie o sume z bazy zawierajace klauzule WHERE
pipeline::query_id downloadSumWhere(string tableName, string columnName, string numerPorzadkowyName, string whereOperator, string numerPorzadkowy, pipeline *P) {
    string sql = "SELECT SUM(" + tableName + ".\"" + columnName +
                 "\") FROM " + tableName + " WHERE " + tableName + ".\"" + numerPorzadkowyName + "\" " + whereOperator + " " + numerPorzadkowy;
    return(P -> insert(sql));
}

//wycieaganie stringa z pqxx -> result
string resToString(result *R){
    string value = "";
    field F = (R -> begin())[0];
    if(!F.is_null()) {
        value = clearWhite(F.c_str());
    }
    return value;
}
//select * from where
result selectWhere(string tableName, string whereColumn, string whereValue, connection *C) {
    nontransaction N(*C);
    string sql = "SELECT * FROM " + tableName + " WHERE \"" + whereColumn + "\" = \'" + whereValue + "\'";
    result R( N.exec(sql));
    return R;
}

//Zapisanie wartosci danej kolumny do vectora
vector<string> getColumn(string tableName, string columnName, connection *C) {

    string sql = "SELECT " + tableName + ".\"" + columnName + "\" FROM " + tableName;
    nontransaction N(*C);
    result R( N.exec( convert(sql) ));
    vector<string> column;
    column.erase(column.begin(),column.end());
    for (result::const_iterator it = R.begin(); it != R.end(); ++it) {
        string value = "";
        if(!it[0].is_null()) value = it[0].as<string>();
        else value = "0";
        column.push_back(clearWhite(value));
    }
    return column;
}

//wywolanie funkcji agregujacej
double aggrSQL(connection *C, string sql){
    //cout << sql << "\n";
    nontransaction N(*C);
    result R( N.exec( convert(sql) ));
    if(strcmp((*R.begin())[0].c_str(), "") == 0) return 0;
    else return (*R.begin())[0].as<double>();
}

//zapytanie do SQL i wynik jako string
string querSQL(connection *C, string sql){
//    cout << sql << "\n";
    nontransaction N(*C);
    result R = N.exec( sql.c_str() );
//    return (*R.begin())[0].as<string>();
    string result;// = (*R.begin())[0].c_str();
    if(R.begin()[0].is_null()) result = "0";
    else result = R.begin()[0].as<string>();
//    cout<<sql<<endl;
//    cout<<R.begin()[0].as<string>()<<"   "<<result<<endl<<endl;
    return result;
}

//zapytanie do SQL, czy tabela istnieje
bool table_exists(connection *C, string schem, string table){
    nontransaction N(*C);
    string sql = "SELECT EXISTS ( SELECT 1 FROM information_schema.tables WHERE table_schema = '" + N.esc(clearWhiteChar(schem)) + "' AND table_name = '" + N.esc(clearWhiteChar(table)) + "');";
//    cout<<sql<<endl;
    result R( N.exec( convert(sql) ));
    return (*R.begin())[0].as<bool>();
}

//zapytanie do SQL, czy kolumna istnieje
bool column_exists(connection *C, string column, string schem, string table){
    nontransaction N(*C);
    string sql = "SELECT EXISTS ( SELECT 1 FROM information_schema.columns WHERE table_schema = '" + N.esc(clearWhiteChar(schem)) + "' AND table_name = '" + N.esc(clearWhiteChar(table)) + "' AND column_name = '" + N.esc(clearWhiteChar(column)) + "');";
    result R( N.exec( convert(sql) ));
//    cout<<sql<<endl;
//    string s = "'﻿ID')";
//    for(int i = 0; i < s.size(); i++) cout<<(int)s[i]<<" ; ";
//    cout<<endl<<endl;
    return (*R.begin())[0].as<bool>();
}

int getFileTypeID(string JPKType, string kodSystemowy) {
    
    try {
        connection C(loginDetails());
        nontransaction N(C);
        string sql = "SELECT \"ID\" FROM \"Schemat_RULES\".\"JPKTypes\" WHERE \"JPKType\" = '" + JPKType + "' AND \"kodSystemowy\" = '" + kodSystemowy + "';";
        
        //cout << sql << endl;
        
        result R = N.exec(sql);
        if(!R.begin()[0].is_null()) {
            int JPKTypeID = R.begin()[0].as<int>();
            return JPKTypeID;
        } else {
            cerr << "Error while downloading JPK Types informations. There is no such JPK as " + kodSystemowy + "\n";
            return -1;
        }
        C.disconnect ();
    } catch (const exception &e) {
        cerr << e.what() << endl;
        cerr << "Error while downloading JPK Types informations.\n";
        return -1;
    }
}

//funkcja okreslajaca czy id istnieje
string ifFileIDExists(string fileID, string schema, int it, nontransaction *N) {
    try {
        string sql = "SELECT COUNT(\"FileID\") FROM \"Schemat_" + schema + "\".\"FileID\" WHERE \"FileID\" = '" + fileID + "'";
        result R( N->exec( convert(sql) ));
        if(R.begin()[0].as<int>() > 0) {
            it++;
            vector<string> tmp = separate(fileID, '-');
            string fileID = tmp[0];
            tmp.clear();
            return ifFileIDExists(fileID + "-" + conv_num(it), schema, it, N);
        } else return fileID;
    } catch (const exception &e) {
        cerr << e.what() << endl;
        cerr << "\n error while checking if new ID exists \n";
        return "NULL";
    }
}

//wykonanie operacji na bazie
void executeSQL(connection *C, string sql){
    //cout << sql << "\n";
    work W(*C);
    W.exec(sql);
    W.commit();
}

//podajemy po przecinku nazwy kolumn, uzyskujemi string z dodanymi znakami + i funkcjami COALESCE, zeby nie uzyskac sumy NULL
string columns(string col){
    string res, next;
    for(int i = 0; i <= col.size(); i++){
        if(i == col.size() || col[i]==','){
            if(next[0] != '-') {
                res += "COALESCE(\"" + next + "\", 0)" + (i == col.size() ? "" : " + ");
            } else {
                res += "COALESCE(-\"" + next.substr(1, next.size()-1) + "\", 0)" + (i == col.size() ? "" : " + ");
            }
            next = "";
        }
        else
            if(col[i] != ' ')
                next += col[i];
    }
    return res;
}

string rText(){
    int n = rand() % 30 + 5;
    string res;
    for(int i = 0; i < n; i++)
        res += (char)('a'+rand()%26);
    return res;
}
string rNumber(int range){
    return conv_num(rand() %range + 1);
}

void clearFileIDs(string schemat) {
    connection C(loginDetails());
    work W(C);
    string deleteSQL = "DELETE FROM \"" + schemat + "\".\"FileID\";";
    string alterSQL = "ALTER SEQUENCE \"" + schemat + "\".\"FileID_ID_seq\" RESTART WITH 1;";
    string sql = deleteSQL + alterSQL;
    result R(W.exec(sql));
    W.commit();
    C.disconnect ();
}

bool insertFileIDs(string schemat, string xmlFileName, string xmlFileID, map <string, string> controlSums) {
    try {
        connection C(loginDetails());
        work W(C);
        string quer1 = "", quer2 = "";
        vector<string> tmp = separate(xmlFileName, '/');
        string filename = tmp[tmp.size() - 1];
        tmp.clear();
        cout<<xmlFileID<<";"<<filename<<";"<<xmlFileName<<";"<<endl;
        
        for (map<string, string>::iterator iter = controlSums.begin(); iter != controlSums.end(); ++iter) {
            quer1 = quer1 + "\"" + iter->first + "\" = '" +  iter->second + "',";
        }
        quer1.erase(quer1.size()-1,1);
        
        string sql = "UPDATE \"" + schemat + "\".\"FileID\" SET " + quer1 + " WHERE \"FileID\" = '" + xmlFileID + "';";
        
        W.exec(sql);
        W.commit();
        C.disconnect ();
        controlSums.clear();
    } catch (const exception &e) {
        cerr << e.what() << endl;
        return 0;
    } return 1;
}

bool updateFilesList(string xmlFileName, string xmlFileID, int fileTypeID) {
    try {
        connection C(loginDetails());
        work W(C);
        string quer1 = "", quer2 = "";
        vector<string> tmp = separate(xmlFileName, '/');
        string filename = tmp[tmp.size() - 1];
        tmp.clear();
        
        time_t now = time(0);
        tm *ltm = localtime(&now);
        string uploadDate = conv_num(1900 + ltm->tm_year) + "-" + conv_num(1 + ltm->tm_mon) + "-" + conv_num(ltm->tm_mday) + " " + conv_num(ltm->tm_hour) + ":" + conv_num(ltm->tm_min) + ":" + conv_num(ltm->tm_sec);
        
        string sql = "INSERT INTO \"Schemat_JPK_COMMON\".\"AllFiles\" (\"FilePath\", \"FileName\", \"FileID\", \"FileTypeID\", \"UploadDate\") VALUES ('" + W.esc(xmlFileName) + "', '" + W.esc(filename) + "', '" + W.esc(xmlFileID) + "', '" + W.esc(conv_num(fileTypeID)) + "', '" + uploadDate + "');";
        result R(W.exec(sql));
        W.commit();
        C.disconnect ();
    } catch (const exception &e) {
        cerr << e.what() << endl;
        return 0;
    } return 1;
}

vector<string> getDateRange(string fileID) {
    vector<string> dateRange;
    dateRange.push_back("");
    dateRange.push_back("");
    try {
        connection C(loginDetails());
        nontransaction N(C);
        result R( N.exec( "SELECT \"DataOd\", \"DataDo\" FROM  \"Schemat_JPK_COMMON\".\"Naglowek\" WHERE \"FileID\" = '" + fileID + "'"));
        if((*R.begin())[0].is_null() || (*R.begin())[1].is_null()) {
            cerr<<"There is no file with such ID"<<endl;
            dateRange[0] = ("nope");
        }
        else {
            dateRange[0] = ((*R.begin())[0].as<string>());
            dateRange[1] = ((*R.begin())[1].as<string>());
        }
        C.disconnect ();
    } catch (const exception &e)
    {
        cerr << e.what() << endl;
        cerr<<"Error while getting date range from DB\n";
    } return dateRange;
}

//czyszczenie tableli i ustawianie ID na 1
void cleanTable(string name, string schem){
    if(name == "Podmiot1" || name == "Naglowek"){    //specyficzny przypadek - wezel Podmiot1 i Naglowek sa w Schemat_JPK_COMMON
        schem = "Schemat_JPK_COMMON";
    }
    try {
        connection C(loginDetails());
        checkConnection(&C);
        if(!table_exists(&C, schem, name)){
            C.disconnect();
            return;
        }
        work W(C);
        string sql;
        sql = "DELETE FROM \"" + W.esc(schem) + "\".\"" + W.esc(name) + "\"";
        W.exec(sql);
        
        sql = "ALTER SEQUENCE \"" + W.esc(schem) + "\".\"" + W.esc(name) + "_ID_seq\" RESTART WITH 1";
        W.exec(sql);
        if(name == "SprzedazWiersz") {
            sql = "ALTER SEQUENCE \"" + W.esc(schem) + "\".\"" + W.esc(name) + "_LpSprzedazy_seq\" RESTART WITH 1";
            W.exec(sql);
        }
        if(name == "ZakupWiersz") {
            sql = "ALTER SEQUENCE \"" + W.esc(schem) + "\".\"" + W.esc(name) + "_LpZakupu_seq\" RESTART WITH 1";
            W.exec(sql);
        }
        W.commit();
        C.disconnect();
    } catch (const exception &e)
    {
        cerr << e.what() << endl;
    }
}

//czyszczenie rekordow z okreslonym FileID
void cleanRecordWithFileID(string name, string schem, string fileID) {
    if (name == "Podmiot") name = "Podmiot1";
    if (name == "Podmiot1" ||
        name == "Naglowek") {    //specyficzny przypadek - wezel Podmiot1 i Naglowek sa w Schemat_JPK_COMMON
        schem = "Schemat_JPK_COMMON";
    }
    try {
        connection C(loginDetails());
        checkConnection(&C);
        if (!table_exists(&C, schem, name)) {
            C.disconnect();
            return;
        }
        work W(C);
        string sql;
        sql = "DELETE FROM \"" + W.esc(schem) + "\".\"" + W.esc(name) + "\" WHERE \"FileID\" = '" + fileID + "';";
        W.exec(sql);
        W.commit();
        C.disconnect();
    } catch (const exception &e) {
        cerr << e.what() << endl;
    }
}
