#ifndef GETFROMDB_H
#define GETFROMDB_H
#include <stdio.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include "Driver.h"
#include "SQLDialect.h"

class GetFromDB{
 public:
    static void getCustPhones(vector<string>&  c_phones);
    static void getSuppPhones(vector<string>&  s_phones);
    static void getNumOrders(int& lo_orderkey, SQLHANDLE& env);
};
#endif