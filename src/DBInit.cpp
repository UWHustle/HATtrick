#include "DBInit.h"

int DBInit::createSchema(SQLHSTMT& stmt){
    for(unsigned int i=0; i<SQLDialect::createSchemaStmt[UserInput::getdbChoice()].size(); i++){
        Driver::executeStmtDiar(stmt, SQLDialect::createSchemaStmt[UserInput::getdbChoice()][i].c_str());
    }
    return 0;
}

int DBInit::dropSchema(SQLHSTMT& stmt){
    for(unsigned int i=0; i<SQLDialect::dropSchemaStmt[UserInput::getdbChoice()].size(); i++){
        Driver::executeStmtDiar(stmt, SQLDialect::dropSchemaStmt[UserInput::getdbChoice()][i].c_str());
    }
    return 0;
}

int DBInit::bulkLoad(SQLHSTMT& stmt){
    for(unsigned int i=0; i<SQLDialect::bulkLoadStmt[UserInput::getdbChoice()].size()-1; i+=2){
        Driver::executeStmtDiar(stmt, (SQLDialect::bulkLoadStmt[UserInput::getdbChoice()][i]+\
        UserInput::getDataPath()+SQLDialect::bulkLoadStmt[UserInput::getdbChoice()][i+1]).c_str());
    }
    //Driver::executeStmtDiar(stmt, (SQLDialect::bulkLoadStmt[UserInput::getdbChoice()].back()).c_str());      // freshness table initial insertion
    return 0;
}

int DBInit::indexCreation(SQLHSTMT& stmt){
    for(unsigned int i=0; i<SQLDialect::createIndexStmt[UserInput::getdbChoice()].size(); i++){
        Driver::executeStmtDiar(stmt, SQLDialect::createIndexStmt[UserInput::getdbChoice()][i].c_str());
    }
    return 0;


}
