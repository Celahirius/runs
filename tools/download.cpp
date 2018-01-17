// z podanej tabeli do vektora wstawiaja sie nazwy kolumn
vector <string> downloadFromSql_columns(string schema_name, string table_name, connection *C){
    vector <string> res;
    nontransaction N(*C);
    string sql = "SELECT * FROM information_schema.columns WHERE table_schema = '" + N.esc(schema_name) + "' AND table_name   = '" + N.esc(table_name) + "'";
    result R( N.exec( convert(sql) ));
    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
         res.push_back(c[3].as<string>());
    }
    return res;
}

// z podanej tabeli do vektora vectorow wstawia sie cala tabela
vector< vector <string> > downloadFromSql_record(string table_name, connection *C, string fileID = "", string orderBy = ""){
    vector< vector <string> > res;
    string sql;
    nontransaction N(*C);
    if(orderBy != "") orderBy = " ORDER BY \"" + orderBy + "\" ASC ";
    
    if(fileID == "") {
        sql = "SELECT * FROM \"" + N.esc(schema_name) + "\".\"" + N.esc(table_name) + "\"" + orderBy;
    } else sql = "SELECT * FROM \"" + N.esc(schema_name) + "\".\"" + N.esc(table_name) + "\"WHERE \"FileID\" = '" + N.esc(fileID) + "'" + orderBy;
    result R( N.exec( convert(sql) ));
    for(result::const_iterator c = R.begin(); c != R.end(); c++) {
        vector <string> row;
        for(int i = 0; i < c.size(); i++)
            row.push_back(clearWhite(c[i].c_str()));
        res.push_back(row);
    }
    return res;
}

// funkcja zwraca reprezentacje tabeli jako pare vector<string> - nazwy kolumn i vector < vector< string> > - cala tabela
// pomijane sa pomocnicze kolumny
//pair<vector<string> , vector < vector< string> > > downloadFromSql(string schema_name, string table_name, connection *C, string fileID = "", string orderBy = ""){
//    pair<vector<string> , vector < vector< string> > > res_vec;
//    vector <string> col_names = downloadFromSql_columns(schema_name, table_name, C);
//    vector < vector <string> > col_val = downloadFromSql_record(schema_name, table_name, C, fileID, orderBy);
//    vector <string> emptyVec;
//    for(int i = 0; i < col_val.size(); i++)
//        res_vec.second.push_back(emptyVec);
//    for(int i = 0; i < col_names.size(); i++){
//        if(col_names[i] == "ID" || col_names[i] == "FileName" || col_names[i] == "FileID" || col_names[i] == "DataWyciagu" || col_names[i] == "mt940")
//            continue;
//        res_vec.first.push_back(col_names[i]);
//        for(int j = 0; j < res_vec.second.size(); j++){
//            res_vec.second[j].push_back(col_val[j][i]);
//        }
//    }
//    return res_vec;
//}
