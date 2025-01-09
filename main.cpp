#include <occi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include "baza.hpp"

int main() {
    baza hr;
    hr.init();
    hr.list_column_table("angajat", 4);
    // hr.sort_table("departament","buget","ASC");
    // hr.list_all_tables();
    
    //modify
    // hr.list_column_table("departament",3);
    // hr.modify("departament","1","buget","50000","NUMBER");
    // hr.list_all_columns_table("departament");
    // hr.list_column_table("departament",3);
    
    
    return 0;
}
