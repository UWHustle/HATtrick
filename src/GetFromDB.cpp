#include "GetFromDB.h"
using namespace std;

void GetFromDB::getCustPhones(vector<string>&  c_phones){
    SQLHENV env = 0;
    SQLHDBC dbc = 0;
    SQLHSTMT stmt = 0;
    char phone[30] = {0};
    Driver::connectDB(env, dbc);
    Driver::allocHandle(dbc,stmt);
    Driver::executeStmtDiar(stmt, (SQLDialect::init[0]).c_str());
    Driver::bindCharColumn(stmt, phone, 16, 1);
    for (int i=0; i<UserInput::getCustSize(); i++) {
        SQLRETURN f = SQLFetch(stmt);
        if (f == SQL_SUCCESS)
            c_phones.push_back(phone);
        else
            break;
    }
    Driver::freeStmtHandle(stmt);
    Driver::disconnectDB(dbc);
}

void GetFromDB::getSuppPhones(vector<string>&  s_phones){
    SQLHENV env = 0;
    SQLHDBC dbc = 0;
    SQLHSTMT stmt = 0;
    char phone[30] = {0};
    Driver::connectDB(env, dbc);
    Driver::allocHandle(dbc,stmt);
    Driver::executeStmtDiar(stmt, (SQLDialect::init[1]).c_str());
    Driver::bindCharColumn(stmt, phone, 16, 1);
    for (int i=0; i<UserInput::getCustSize(); i++) {
        SQLRETURN f = SQLFetch(stmt);
        if (f == SQL_SUCCESS)
            s_phones.push_back(phone);
        else
            break;
    }
    Driver::freeStmtHandle(stmt);
    Driver::disconnectDB(dbc);
}

void GetFromDB::getNumOrders(int& lo_orderkey, SQLHENV& env){
    //SQLHENV env = 0;
    SQLHDBC dbc = 0;
    SQLHSTMT stmt = 0;
    SQLLEN indicator = 0;
    Driver::connectDB2(env, dbc);
    Driver::allocHandle(dbc,stmt);
    Driver::executeStmtDiar(stmt, (SQLDialect::init[2]).c_str());
    SQLRETURN f = SQLFetch(stmt);
    if (f == SQL_SUCCESS)
        SQLGetData(stmt, 1, SQL_C_DEFAULT, &lo_orderkey, 0, &indicator);
    Driver::freeStmtHandle(stmt);
    Driver::disconnectDB(dbc);
}