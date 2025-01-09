#include "baza.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include "baza.hpp"

baza::baza() : env(nullptr), err(nullptr), svc(nullptr), stmt(nullptr), defn(nullptr), status(OCI_ERROR) {}

int baza::checkOCIStatus(sword status) {
    if (status == OCI_SUCCESS || status == OCI_SUCCESS_WITH_INFO) {
        return 0; // Success or Success with info
    } else {
        return 1; // Error or failure
    }
}

void baza::init() {
    OCIEnvCreate(&env, OCI_DEFAULT, NULL, NULL, NULL, NULL, 0, NULL);
    OCIHandleAlloc(env, (void **)&err, OCI_HTYPE_ERROR, 0, NULL);
    OCIHandleAlloc(env, (void **)&svc, OCI_HTYPE_SVCCTX, 0, NULL);
    OCILogon(env, err, &svc, (text *)"system", strlen("system"),
                      (text *)"1379", strlen("1379"),
                      (text *)"localhost", strlen("localhost"));
    OCIHandleAlloc(env, (void **)&stmt, OCI_HTYPE_STMT, 0, NULL);
    tables = {"angajat", "departament", "pozitie", "pregatire", "proiect",
              "sarcina", "recenzie_de_performanta", "recrutare"};
}

void baza::get_column_count(OCIStmt* stmt, OCIError* err) {
    int column_count = 0;
    sword status = OCIStmtExecute(svc, stmt, err, 0, 0, NULL, NULL, OCI_DEFAULT);
    if (status != OCI_SUCCESS) {
        std::cerr << "OCIStmtExecute failed!" << std::endl;
        return;
    }

    status = OCIAttrGet(
        (dvoid*)stmt, OCI_HTYPE_STMT, (dvoid*)&column_count, (ub4*)0,
        OCI_ATTR_PARAM_COUNT, err);
    if (status == OCI_SUCCESS) {
        std::cout << "Number of columns: " << column_count << std::endl;
    } else {
        char errbuf[512];
        sb4 errcode;
        OCIErrorGet(err, 1, NULL, &errcode, (text*)errbuf, sizeof(errbuf), OCI_HTYPE_ERROR);
        std::cout << "Error getting column count: " << errbuf << std::endl;
    }
}

void baza::print_column(OCIStmt* stmt, OCIError* err, int column_index){
    char col[500];
    OCIDefineByPos(stmt, &defn, err, column_index, col, sizeof(col), SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT);
    OCIStmtExecute(svc, stmt, err, column_index, 0, NULL, NULL, OCI_DEFAULT);

    while (OCIStmtFetch2(stmt, err, 1, OCI_FETCH_NEXT, 0, OCI_DEFAULT) == OCI_SUCCESS) {
        printf("Column: %s\n", col);
    }
}

void baza::print_columns(OCIStmt* stmt, OCIError* err){
    int num_columns = 0;
    OCIAttrGet(stmt, OCI_HTYPE_STMT, &num_columns, 0, OCI_ATTR_PARAM_COUNT, err);
    std::cout << "Number of columns: " << num_columns << '\n';

    std::vector<char*> column_buffers(num_columns);
    std::vector<OCIDefine*> defines(num_columns);

    for (int i = 0; i < num_columns; ++i) {
        column_buffers[i] = new char[500];
        OCIDefineByPos(stmt, &defines[i], err, i + 1, column_buffers[i], 500, SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT);
    }

    while (OCIStmtFetch2(stmt, err, 1, OCI_FETCH_NEXT, 0, OCI_DEFAULT) == OCI_SUCCESS) {
        for (int i = 0; i < num_columns; ++i) {
            printf("%d: %s\n", i + 1, column_buffers[i]);
        }
        std::cout << "------------------\n";
    }

    for (int i = 0; i < num_columns; ++i) {
        delete[] column_buffers[i];
    }
}

void baza::list_column_table(const std::string& table_name, int column_index) {
    std::string sql_query = "SELECT * FROM " + table_name;
    OCIStmtPrepare(stmt, err, (text*)sql_query.c_str(), sql_query.length(), OCI_NTV_SYNTAX, OCI_DEFAULT);

    print_column(stmt,err,column_index);
}

void baza::list_all_columns_table(const std::string& table_name) {
    std::string sql_query = "SELECT * FROM " + table_name;
    OCIStmtPrepare(stmt, err, (text*)sql_query.c_str(), sql_query.length(), OCI_NTV_SYNTAX, OCI_DEFAULT);
    OCIStmtExecute(svc, stmt, err, 0, 0, NULL, NULL, OCI_DEFAULT);

    print_columns(stmt,err);
}

void baza::list_all_tables() {
    for (auto& nume : tables) {
        std::cout << "----------------" << nume << "----------------\n";
        list_all_columns_table(nume);
        std::cout << "\n";
    }
}

void baza::sort_table(const std::string& table_name, const std::string& column_name, const std::string &type_of_sort){
    std::string sql_query = "SELECT * FROM " + table_name + " ORDER BY " + column_name + " " + type_of_sort;
    OCIStmtPrepare(stmt, err, (text*)sql_query.c_str(), sql_query.length(), OCI_NTV_SYNTAX, OCI_DEFAULT);
    OCIStmtExecute(svc, stmt, err, 0, 0, NULL, NULL, OCI_DEFAULT);

    print_columns(stmt,err);
}

void baza::modify(const std::string& table_name, const std::string& id, const std::string& attribute, const std::string& value, const std::string& type){
    int ok=1;
    std::string sql_query;
    if(type == "NUMBER" || type == "INT" || type == "DATE"){
        sql_query = "UPDATE " + table_name + " SET " + attribute + " = "+ value
                    + " WHERE id_" + table_name + " = " + id + ";";
        ok=0;
    } else if(type == "DATE"){
        sql_query = "UPDATE " + table_name + " SET " + attribute + " ='" + value
                    + "' WHERE id_" + table_name + " = " + id;
        ok=0;
    } else {
        std::cout<<"Error! >"<<type<<'\n';
    }
    
    if(ok == 0){
        std::cout<<sql_query<<"\n";
        sql_query = "UPDATE DEPARTAMENT SET buget = 23211 WHERE id_departament = 1;";
        OCIStmtPrepare(stmt, err, (text*)sql_query.c_str(), sql_query.length(), OCI_NTV_SYNTAX, OCI_DEFAULT);
        OCIStmtExecute(svc, stmt, err, 1, 0, NULL, NULL, OCI_COMMIT_ON_SUCCESS );
        
        list_all_columns_table(table_name);
    }
}

baza::~baza() {
    OCIHandleFree(stmt, OCI_HTYPE_STMT);
    OCIHandleFree(svc, OCI_HTYPE_SVCCTX);
    OCIHandleFree(err, OCI_HTYPE_ERROR);
    OCILogoff(svc, err);
}