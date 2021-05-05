#include "Driver.h"
#include "UserInput.h"
using namespace std;

void Driver::extract_error(const char* fn, SQLHANDLE& handle, SQLSMALLINT type){
    SQLINTEGER i = 0;
    SQLINTEGER native;
    SQLCHAR state[7] = {0};
    SQLCHAR text[256]= {0};
    SQLSMALLINT len;
    SQLRETURN ret;
    cout << "\nThe driver reported the following diagnostics whilst running " << fn << "\n\n";
    do
    {
        ret = SQLGetDiagRec(type, handle, ++i, state, &native, text, sizeof(text), &len );
        if (SQL_SUCCEEDED(ret))
            cout << "SQL_SUCCESS_WITH_INFO:\n"
                 << "   State: " << state << "\n"
                 << "   Native error code: " << native  << "\n"
                 << "   Message text: " << text << "\n";
    }
    while( ret == SQL_SUCCESS );
}

void Driver::setEnv(SQLHENV& env){
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
    SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, 0);
}

void Driver::connectDB(SQLHENV& env, SQLHDBC& dbc){
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
    SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, 0);
    SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);
    SQLRETURN ret = SQLConnect(dbc, (SQLCHAR*) UserInput::getDSN().c_str(), SQL_NTS, (SQLCHAR*) \
    UserInput::getDBUser().c_str(), SQL_NTS, (SQLCHAR*) UserInput::getDBPwd().c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS) {
        /*if (ret == SQL_SUCCESS_WITH_INFO) {
          printf("Driver reported the following diagnostics\n");
          Driver::extract_error("SQLConnect", dbc, SQL_HANDLE_DBC);
          exit(1);
        }
        else*/
        printf("\n---------------------------------\n\tConnected to the DB\n---------------------------------\n\n");
    }
    else {
        Driver::extract_error("SQLConnect", dbc, SQL_HANDLE_DBC);
        printf("\nFailed to connect!\n\n");
        exit(1);
    }
}


void Driver::connectDB2(SQLHENV& env, SQLHDBC& dbc){
    SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);
    SQLRETURN ret = SQLConnect(dbc, (SQLCHAR*) UserInput::getDSN().c_str(), SQL_NTS, (SQLCHAR*) \
    UserInput::getDBUser().c_str(), SQL_NTS, (SQLCHAR*) UserInput::getDBPwd().c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS_WITH_INFO) {
        /*if (ret == SQL_SUCCESS_WITH_INFO) {
            printf("Driver reported the following diagnostics\n");
            Driver::extract_error("SQLConnect", dbc, SQL_HANDLE_DBC);
            exit(1);
        }
        else*/
        printf("\n---------------------------------\n\tConnected to the DB\n---------------------------------\n\n");
    }
    else if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO){
        Driver::extract_error("SQLConnect", dbc, SQL_HANDLE_DBC);
        printf("\nFailed to connect!\n\n");
        exit(1);
    }
}

int Driver::executeStmtDiar(SQLHSTMT& stmt, const char* query){
    SQLRETURN ret = SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
    if (ret == SQL_SUCCESS_WITH_INFO) {
        printf("SUCCESS_WITH_INFO Driver reported the following diagnostics\n");
        extract_error("SQLExecuteDirect", stmt, SQL_HANDLE_STMT);
        //exit(1);*/  
	      return 1;
      }
    
    else if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
      fprintf(stderr, "\nFailed to execute directly the stmt!\n\n");
      extract_error("FAIL: SQLExecuteDirect", stmt, SQL_HANDLE_STMT);
      //exit(1);*/
      return 2;
    }
    return 0;
}

int Driver::executeStmt(SQLHSTMT& stmt){
    SQLRETURN ret = SQLExecute(stmt);
    if (ret == SQL_SUCCESS_WITH_INFO){
      printf("Driver reported the following diagnostics\n");
      extract_error("SQLExecute", stmt, SQL_HANDLE_STMT);
      //exit(1);*/
      return 1;
      
    }
    else if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO){
      fprintf(stderr, "\nFailed to execute stmt!\n\n");
      extract_error("SQLExecute", stmt, SQL_HANDLE_STMT);
      //exit(1);*/
      return 2;
    }
    return 0;
}

void Driver::prepareStmt(SQLHDBC& dbc, SQLHSTMT& stmt, const char* query){
    SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (ret == SQL_SUCCESS_WITH_INFO) {
          printf("Driver reported the following diagnostics:\n");
          Driver::extract_error("SQLAllocHandle", stmt, SQL_HANDLE_STMT);
          exit(1);
      
    }
    else if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO){
        fprintf(stderr, "\nFailed to allocate stmt!\n\n");
        Driver::extract_error("SQLAllocHandle in prepareStmt.", stmt, SQL_HANDLE_STMT);
        exit(1);
    }
    SQLRETURN pre = SQLPrepare(stmt, (unsigned char*) query, SQL_NTS);

    if (pre == SQL_SUCCESS_WITH_INFO) {
          printf("Driver reported the following diagnostics:\n");
          Driver::extract_error("SQLPrepare", stmt, SQL_HANDLE_STMT);
          exit(1);
    }
    else if(pre != SQL_SUCCESS && pre != SQL_SUCCESS_WITH_INFO){
        fprintf(stderr, "\nFailed to prepare stmt!\n\n");
        Driver::extract_error("SQLPrepare in prepareStmt.", stmt, SQL_HANDLE_STMT);
        exit(1);
    }
}

void Driver::allocHandle(SQLHDBC& dbc, SQLHSTMT& stmt){
    SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (ret == SQL_SUCCESS_WITH_INFO) {
          printf("Driver reported the following diagnostics:\n");
          Driver::extract_error("SQLAllocHandle", stmt, SQL_HANDLE_STMT);
          exit(1);
    }
    else if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO){
        fprintf(stderr, "\nFailed to allocate stmt!\n\n");
        Driver::extract_error("SQLAllocHandle in prepareStmt.", stmt, SQL_HANDLE_STMT);
        exit(1);
    }
}

void Driver::bindCharColumn(SQLHSTMT& stmt, char* colBuf, int size,  int colNum){
    SQLLEN indicator = 0;
    SQLRETURN ret = SQLBindCol(stmt, colNum, SQL_C_CHAR, colBuf, size, &indicator);
    if (ret == SQL_SUCCESS) {
      if (ret == SQL_SUCCESS_WITH_INFO) {
        printf("Driver reported the following diagnostics:\n");
        extract_error("SQLBindCol for type Char.", stmt, SQL_HANDLE_STMT);
        exit(1);
      }
    }
    else {
      fprintf(stderr, "\nFailed to execute the bind column stmt!\n\n");
      extract_error("SQLBindCol of type Char.", stmt, SQL_HANDLE_STMT);
      exit(1);
    }
}

void Driver::bindIntColumn(SQLHSTMT& stmt, int& colBuf, int colNum){
    SQLLEN indicator = 0;
    SQLRETURN ret = SQLBindCol(stmt, colNum, SQL_C_DEFAULT, &colBuf, 0, &indicator);
    if (ret == SQL_SUCCESS) {
      if (ret == SQL_SUCCESS_WITH_INFO) {
        printf("Driver reported the following diagnostics:\n");
        extract_error("SQLBindCol of type Int.", stmt, SQL_HANDLE_STMT);
        exit(1);
      }
    }
    else {
      fprintf(stderr, "\nFailed to execute the bind column stmt!\n\n");
      extract_error("SQLBindCol of type Int.", stmt, SQL_HANDLE_STMT);
      exit(1);
    }
}

void Driver::bindDecColumn(SQLHSTMT& stmt, double& colBuf, int colNum){
    SQLLEN indicator = 0;
    SQLRETURN ret = SQLBindCol(stmt, colNum, SQL_C_DOUBLE, &colBuf, 0, &indicator);
    if (ret == SQL_SUCCESS) {
      if (ret == SQL_SUCCESS_WITH_INFO) {
        printf("Driver reported the following diagnostics:\n");
        extract_error("SQLBindCol of type Double.", stmt, SQL_HANDLE_STMT);
        exit(1);
      }
    }
    else {
      fprintf(stderr, "\nFailed to execute the bind column stmt!\n\n");
      extract_error("SQLBindCol of type Double.", stmt, SQL_HANDLE_STMT);
      exit(1);
    }
}

void Driver::bindCharParam(SQLHSTMT& stmt, char* param, int size, int pos){
    SQLRETURN ret = SQLBindParameter(stmt, pos, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, size, 0, param, 0, 0);
    if (ret == SQL_SUCCESS) {
      if (ret == SQL_SUCCESS_WITH_INFO) {
        printf("Driver reported the following diagnostics:\n");
        extract_error("SQLBindParameter of type Char.", stmt, SQL_HANDLE_STMT);
        exit(1);
      }
    }
    else {
      fprintf(stderr, "\nFailed to execute the bind parameter stmt!\n\n");
      extract_error("SQLBindCol of type Char.", stmt, SQL_HANDLE_STMT);
      exit(1);
    }
}

void Driver::bindIntParam(SQLHSTMT& stmt, int& param, int pos){
    SQLRETURN ret = SQLBindParameter(stmt, pos, SQL_PARAM_INPUT, SQL_C_DEFAULT, SQL_INTEGER, 0, 0, &param, 0, 0);
    if (ret == SQL_SUCCESS) {
      if (ret == SQL_SUCCESS_WITH_INFO) {
        printf("Driver reported the following diagnostics:\n");
        extract_error("SQLBindParameter of type Int.", stmt, SQL_HANDLE_STMT);
        exit(1);
      }
    }
    else {
      fprintf(stderr, "\nFailed to execute the bind parameter stmt!\n\n");
      extract_error("SQLBindParameter of type Int.", stmt, SQL_HANDLE_STMT);
      exit(1);
    }
}

void Driver::bindDecParam(SQLHSTMT& stmt, double& param, int pos){
    SQLRETURN ret = SQLBindParameter(stmt, pos, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &param, 0, 0);
    if (ret == SQL_SUCCESS) {
      if (ret == SQL_SUCCESS_WITH_INFO) {
        printf("Driver reported the following diagnostics\n");
        extract_error("SQLBindParameter of type Double.", stmt, SQL_HANDLE_STMT);
        exit(1);
      }
    }
    else {
      fprintf(stderr, "\nFailed to execute the bind parameter stmt!\n\n");
      extract_error("SQLBindParameter of type Double.", stmt, SQL_HANDLE_STMT);
      exit(1);
    }
}

void Driver::resetStmt(SQLHSTMT& stmt){
    /* Free handles, frees all resources.*/
    SQLRETURN close = SQLFreeStmt(stmt, SQL_CLOSE);
    SQLRETURN unbind = SQLFreeStmt(stmt, SQL_UNBIND);
    SQLRETURN reset = SQLFreeStmt(stmt, SQL_RESET_PARAMS);
    if ((close == SQL_SUCCESS) && (unbind == SQL_SUCCESS) && (reset == SQL_SUCCESS)){
        if((close == SQL_SUCCESS_WITH_INFO) ||  (unbind == SQL_SUCCESS_WITH_INFO ) || (reset == SQL_SUCCESS_WITH_INFO)){
            printf("Driver reported the following diagnostics\n");
            extract_error("SQLFreeStmt.", stmt, SQL_HANDLE_STMT);
            exit(1);
        }
    }
    else{
      fprintf(stderr, "\nFailed to execute the reset stmt!\n\n");
      extract_error("SQLFreeStmt", stmt, SQL_HANDLE_STMT);
      exit(1);
    }
}

void Driver::disconnectDB(SQLHDBC& dbc){
    SQLDisconnect(dbc);
    SQLFreeHandle(SQL_HANDLE_DBC, &dbc);
    printf("\n---------------------------------\n\tDisconnected from the DB\n---------------------------------\n\n");
}

void Driver::freeStmtHandle(SQLHSTMT& stmt){
    SQLRETURN free = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (free == SQL_SUCCESS) {
      if (free == SQL_SUCCESS_WITH_INFO) {
        printf("Driver reported the following diagnostics\n");
        extract_error("SQLFreeHandle", stmt, SQL_HANDLE_STMT);
        exit(1);
      }
    }
    else {
        fprintf(stderr, "\nFailed to execute free stmt handle.\n\n");
        extract_error("SQLFreeHandle", stmt, SQL_HANDLE_STMT);
        exit(1);
    }
}

void Driver::freeEnvHandle(SQLHENV &env){
    SQLRETURN free = SQLFreeHandle(SQL_HANDLE_ENV, env);
    if (free == SQL_SUCCESS) {
        if (free == SQL_SUCCESS_WITH_INFO) {
            printf("Driver reported the following diagnostics\n");
            extract_error("SQLFreeHandle-env", env, SQL_HANDLE_STMT);
            exit(1);
        }
    }
    else {
        fprintf(stderr, "\nFailed to execute free env handle.\n\n");
        extract_error("SQLFreeHandle-env", env, SQL_HANDLE_STMT);
        exit(1);
    }
}


void Driver::fetchData(SQLHSTMT& stmt){
    SQLRETURN f = SQLFetch(stmt);
    if (f == SQL_SUCCESS) {
      if (f == SQL_SUCCESS_WITH_INFO) {
        printf("Driver reported the following diagnostics:\n");
        extract_error("SQLFetch", stmt, SQL_HANDLE_STMT);
        exit(1);
      }
    }
    else {
        fprintf(stderr, "\nFailed to fetch the data.\n\n");
        extract_error("SQLFetch", stmt, SQL_HANDLE_STMT);
        exit(1);
    }
}

void Driver::getCharData(SQLHSTMT& stmt, int col, SQLCHAR* dataBuf){
    SQLLEN indicator = 0;
    SQLRETURN d = SQLGetData(stmt, col, SQL_C_CHAR, &dataBuf, sizeof(dataBuf), &indicator);
    if (d == SQL_SUCCESS) {
      if (d == SQL_SUCCESS_WITH_INFO) {
        printf("Driver reported the following diagnostics:\n");
        extract_error("SQLGetData", stmt, SQL_HANDLE_STMT);
        exit(1);
      }
    }
    else {
        fprintf(stderr, "\nFailed to get the int data.\n\n");
        extract_error("SQLGetData", stmt, SQL_HANDLE_STMT);
        exit(1);
    }
}

void Driver::getIntData(SQLHSTMT& stmt, int col, int& dataBuf){
    SQLLEN indicator = 0;
    SQLRETURN d = SQLGetData(stmt, col, SQL_C_DEFAULT, &dataBuf, 0, &indicator);
    if (d == SQL_SUCCESS) {
      if (d == SQL_SUCCESS_WITH_INFO) {
        printf("Driver reported the following diagnostics:\n");
        extract_error("SQLGetData", stmt, SQL_HANDLE_STMT);
        exit(1);
      }
    }
    else {
        fprintf(stderr, "\nFailed to get the int data.\n\n");
        extract_error("SQLGetData", stmt, SQL_HANDLE_STMT);
        exit(1);
    }
}

void Driver::getLongData(SQLHSTMT& stmt, int col, int& dataBuf){
    SQLLEN indicator = 0;
    SQLRETURN d = SQLGetData(stmt, col, SQL_C_ULONG, &dataBuf, 0, &indicator);
    if (d == SQL_SUCCESS) {
      if (d == SQL_SUCCESS_WITH_INFO) {
        printf("Driver reported the following diagnostics:\n");
        extract_error("SQLGetData", stmt, SQL_HANDLE_STMT);
        exit(1);
      }
    }
    else {
        fprintf(stderr, "\nFailed to get the int data.\n\n");
        extract_error("SQLGetData", stmt, SQL_HANDLE_STMT);
        exit(1);
    }
}

void Driver::getDoubleData(SQLHSTMT& stmt, int col, double& dataBuf){
    SQLLEN indicator = 0;
    SQLRETURN d = SQLGetData(stmt, col, SQL_C_DOUBLE, &dataBuf, 0, &indicator);
    if (d == SQL_SUCCESS) {
      if (d == SQL_SUCCESS_WITH_INFO) {
        printf("Driver reported the following diagnostics:\n");
        extract_error("SQLGetData", stmt, SQL_HANDLE_STMT);
        exit(1);
      }
    }
    else {
        fprintf(stderr, "\nFailed to get the double data.\n\n");
        extract_error("SQLGetData", stmt, SQL_HANDLE_STMT);
        exit(1);
    }
}

void Driver::autoCommitOff(SQLHDBC& dbc){
    SQLRETURN con = SQLSetConnectAttr(dbc, SQL_ATTR_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF, SQL_NTS);
    if (con == SQL_SUCCESS) {
      if (con == SQL_SUCCESS_WITH_INFO) {
        printf("Driver reported the following diagnostics:\n");
        extract_error("SQLSetConnectAttr", dbc, SQL_HANDLE_DBC);
        exit(1);
      }
    }
    else {
        fprintf(stderr, "\nFailed to set auto-commit.\n\n");
        extract_error("SQLSetConnectAttr", dbc, SQL_HANDLE_DBC);
        exit(1);
    }
}

void Driver::endOfTransaction(SQLHDBC& dbc){
    SQLRETURN t = SQLEndTran(SQL_HANDLE_DBC, dbc, SQL_COMMIT);
    if (t == SQL_SUCCESS) {
      if (t == SQL_SUCCESS_WITH_INFO) {
        printf("Driver reported the following diagnostics:\n");
        extract_error("SQLEndTran", dbc, SQL_HANDLE_DBC);
        exit(1);
      }
    }
    else {
        fprintf(stderr, "\nFailed to set auto-commit.\n\n");
        extract_error("SQLSetConnectAttr", dbc, SQL_HANDLE_DBC);
        exit(1);
    }

}
















