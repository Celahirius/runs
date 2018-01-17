//narzedzia tekstowe

//konwersja string -> char*
char small_buffer[300];
char* convert(string s){
    for(int i = 0; i < s.size(); i++)
        small_buffer[i] = s[i];
    small_buffer[s.size()] = '\0';
    return small_buffer;
}

//odcinanie początkowej częsci, aż do ostatniego separatora sep
string afterLast(string name, char sep){
    string res;
    for(int i = 0; i < name.size(); i++){
        if(name[i] == sep)
            res = "";
        else
            res += name[i];
    }
    return res;
}

//dzielene stringa podanymi separatorami na vector
vector <string> separate(string str, char sep){
    vector <string> res;
    string part_tmp;
    for(int i = 0; i <= str.size(); i++){
        if(str[i] == sep || i == str.size()){
            if(str != "")
                res.push_back(part_tmp);
            part_tmp = "";
        }
        else{
            part_tmp += str[i];
        }
    }
    return res;
}

//wypisanie vectora do stringa
string printVector(vector <string> vec){
    string res;
    for(int i = 0; i < vec.size(); i++){
        res += vec[i];
        if(i + 1 != vec.size())
            res += ',';
    }
    return res;
}

//konwersja int -> string
string conv_num(int n){
    string str = "";
    if(n == 0)
        return "0";
    if(n < 0) {
        n = n*(-1);
        while(n > 0){
            str = (char)(n % 10 + '0') + str;
            n /= 10;
        } str = "-" + str;
    }
    else {
        while (n > 0) {
            str = (char) (n % 10 + '0') + str;
            n /= 10;
        }
    }
    return str;
}

//usuwa wszystkie biale znaki ze stringa
string clearWhiteChar(string s) {
    for(int i = 0; i < (int)s.size(); i++){
        if(isspace(s[i])) s.erase(i,1);
    }
    //    s.erase(remove_if(s.begin(), s.end(), isspace), s.end());
    return s;
}

//usuwanie spacji z poczatku i konca stringa
string clearWhite(string s){
    string res;
    int er1 = 0;
    int er2 = 0;
//    cout << s.size() << endl;
    while(s.size() > 1 + er1 && s[(int)s.size() - 1 - er1] == ' ')
        er1++;
    while(s[er2] == ' ')
        er2++;
    for(int i = er2; i < (int)s.size() - er1; i++)
        res += s[i];
    return res;
}

//usuwanie spacji
string clearSpaces(string s){
    string res;
    for(int i = 0; i < (int)s.size(); i++){
        if(s[i] != ' ')
            res += s[i];
    }
    return res;
}

//uzupelnienie zerami do ustalonej dlugosci
string set_len(string s, int l){
    while(s.size() < l)
        s = '0' + s;
    return s;
}