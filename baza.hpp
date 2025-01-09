#ifndef BAZA_HPP
#define BAZA_HPP

#include <iostream>
#include <vector>
#include <string>
#include <oci.h>

int checkOCIStatus(sword status);

class baza {
private:
    OCIEnv *env;
    OCIError *err;
    OCISvcCtx *svc;
    OCIStmt *stmt;  
    OCIDefine *defn;
    std::vector<std::string> tables;
    sword status;

public:
    baza();
    void init();
    int checkOCIStatus(sword status);
    
    void get_column_count(OCIStmt* stmt, OCIError* err);
    void print_column(OCIStmt* stmt, OCIError* err, int &column_index);
    void print_columns(OCIStmt* stmt, OCIError* err); 
    void list_column_table(const std::string& table_name, int column_index);
    void list_all_columns_table(const std::string& table_name);
    void list_all_tables();
    
    void sort_table(const std::string& table_name, const std::string& column_name, const std::string &type_of_sort);
    ~baza();
};

#endif // baza_HPP
