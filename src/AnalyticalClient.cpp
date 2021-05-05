#include "AnalyticalClient.h"

AnalyticalClient::AnalyticalClient() {
    qStmt = vector<SQLHSTMT>(SQLDialect::analyticalQueries.size(), 0);
    queriesNum = 0;
    freshnessVector = {0};  // freshness score for each analytical query executed in the current thread
    executionTime = vector<vector<double>>(13);   // execution time of each query
    testDuration = 0;
}

void AnalyticalClient::SetThreadNum(thread::id num){
    threadNum =  num;
}

void AnalyticalClient::IncrementQueriesNum(){
    queriesNum++;
}

int AnalyticalClient::GetQueriesNum() const{
    return queriesNum;
}

thread::id AnalyticalClient::GetThreadNum(){
    return threadNum;
}

void AnalyticalClient::PrepareAnalyticalStmt(SQLHDBC& dbc){
    for(unsigned int q=0; q<SQLDialect::analyticalQueries.size(); q++)
    	Driver::prepareStmt(dbc, qStmt[q], SQLDialect::analyticalQueries[q].c_str());
}

int AnalyticalClient::ExecuteQuery(int& q) {
    int ret = -1;
    int cnt = 0;
    while(ret != 0 && cnt < 5){
        cnt += 1;
        ret = Driver::executeStmt(qStmt[q]);
    }
    Driver::resetStmt(qStmt[q]);
    return ret;
    
}

void AnalyticalClient::SetFreshness(int& freshness){
    int ret = -1;
    while(ret != 0)
        ret = Driver::executeStmt(qStmt[13]);       // get freshness score that current query sees before execution
    Driver::fetchData(qStmt[13]);
    int freshnessDB = 0;
    Driver::getIntData(qStmt[13], 1, freshnessDB);
    freshnessVector.push_back(freshnessDB - freshness);
    Driver::resetStmt(qStmt[13]);
}


void AnalyticalClient::FreeQueryStmt(){
    for(unsigned int j=0; j<SQLDialect::analyticalQueries.size(); j++)
        Driver::freeStmtHandle(qStmt[j]);
}


void AnalyticalClient::SetExecutionTime(double execTime, int qType){
    executionTime[qType].push_back(execTime);
}

double AnalyticalClient::GetExecutionTimeSum(int qType){
    double sum=0.0;
    int size = GetExecutionTimeSize(qType);
    for(int i=0; i<size; i++)
        sum += executionTime[qType][i];
    return sum;
}

int AnalyticalClient::GetExecutionTimeSize(int qType){
    return executionTime[qType].size();
}

double AnalyticalClient::GetFreshness(){
    int size = freshnessVector.size();
    int count = 0;
    for(int i=0; i<size; i++)
        if(freshnessVector[i] >= 0)
            count++;
    return (double)count/size;
}

void AnalyticalClient::SetTestDuration(int duration){
    testDuration = duration;
}

int AnalyticalClient::GetTestDuration(){
    return testDuration;
}