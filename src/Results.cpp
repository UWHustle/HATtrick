
#include "Results.h"
#include <filesystem>

namespace fs = std::filesystem;

void Results::setTotalQueries(vector<AnalyticalClient*>& a){
    for(int i=0; i<UserInput::getAnalClients(); i++) {
        totalQueries += a[i]->GetQueriesNum();
    }
}

void Results::setTotalTxns(vector<TransactionalClient*>& t){
    for(int i=0; i<UserInput::getTranClients(); i++) {
        totalTxns += t[i]->GetLocalCounter();
    }
}

double Results::getTransactionalThroughput(){
    return t_throughput;
}

double Results::getAnalyticalThroughput(){
    return a_throughput;
}

void Results::setTransactionalThroughput(double& tt){
    t_throughput = tt;
}

void Results::setAnalyticalThroughput(double& at){
    a_throughput = at;
}

void Results::saveResults() {
    fs::create_directory("results");	
    double tt = 0, at = 0;
    if(totalQueries != 0){
            at = (double)totalQueries/testDuration;
    }	    
    setAnalyticalThroughput(at);
    if(totalTxns != 0){
            tt = (double)totalTxns/UserInput::getTestDuration();
    }
    setTransactionalThroughput(tt);
    resultsStream.open("results/results-SF"+to_string(UserInput::getSF())+".txt", ofstream::out | ofstream::app | ofstream::binary);
    resultsStream << "--------------------------------------------" << endl;
    resultsStream << "Available threads in the system: " << thread::hardware_concurrency() << endl;
    resultsStream << "Anal. Threads #: " << UserInput::getAnalClients() << endl;
    resultsStream << "Tran. Threads #: " << UserInput::getTranClients() << endl;
    resultsStream << "Total # of transactions executed: " << totalTxns << endl;
    resultsStream << "Total # of queries executed: " << totalQueries << endl;
    resultsStream << "Anal. Throughput [queries/sec]: " << getAnalyticalThroughput()  << endl;
    resultsStream << "Tran. Throughput [transactions/sec]: " << getTransactionalThroughput() << endl;
    resultsStream << "Avg. Latency for New-Order Tran: " << txnLatency[0] << endl;
    resultsStream << "Avg. Latency for Payment Tran: " << txnLatency[1] << endl;
    resultsStream << "Avg. Latency for Count-Orders Tran: " << txnLatency[2] << endl;
    resultsStream << "Avg. Execution Time for Q1: " << queryExecTime[0] << endl;
    resultsStream << "Avg. Execution Time for Q2: " << queryExecTime[1] << endl;
    resultsStream << "Avg. Execution Time for Q3: " << queryExecTime[2] << endl;
    resultsStream << "Avg. Execution Time for Q4: " << queryExecTime[3] << endl;
    resultsStream << "Avg. Execution Time for Q5: " << queryExecTime[4] << endl;
    resultsStream << "Avg. Execution Time for Q6: " << queryExecTime[5] << endl;
    resultsStream << "Avg. Execution Time for Q7: " << queryExecTime[6] << endl;
    resultsStream << "Avg. Execution Time for Q8: " << queryExecTime[7] << endl;
    resultsStream << "Avg. Execution Time for Q9: " << queryExecTime[8] << endl;
    resultsStream << "Avg. Execution Time for Q10: " << queryExecTime[9] << endl;
    resultsStream << "Avg. Execution Time for Q11: " << queryExecTime[10] << endl;
    resultsStream << "Avg. Execution Time for Q12: " << queryExecTime[11] << endl;
    resultsStream << "Avg. Execution Time for Q13: " << queryExecTime[12] << endl;
    resultsStream << "Percentage of fresh queries during the test [%]: " << 100-fresh_score << endl;
    resultsStream << "Real test duration: " << testDuration << endl;
    resultsStream.close();
    resultsStream.clear();
    resultsStream.open("results/frontier-SF"+to_string(UserInput::getSF())+".csv", ofstream::out | ofstream::app | ofstream::binary);
    resultsStream << tt << "," << at <<  endl;
    resultsStream.close();
    if(UserInput::getAnalClients()>0){
    	resultsStream.clear();
    	resultsStream.open("results/freshness-SF"+to_string(UserInput::getSF())+"-"+
			to_string(UserInput::getTranClients())+"-"+
			to_string(UserInput::getAnalClients())+".csv", 
			ofstream::out | ofstream::app | ofstream::binary);
    	for(unsigned int i =0; i<(unsigned int)probFresh.size(); i++){
    		resultsStream << probFresh[i] << ", " <<  freshValues[i] <<  endl;
    	}
    	resultsStream.close();
    }
}

void Results::getQueryExecTime(vector<AnalyticalClient*>& a){
    double sum[13] = {0.0};
    int q[13] = {0};
    for(int i=0; i<UserInput::getAnalClients(); i++){
        for(int j=0; j<13; j++){
            sum[j] += a[i]->GetExecutionTimeSum(j);
            q[j] += a[i]->GetExecutionTimeSize(j);
        }
    }
    for(int i=0; i<13; i++)
        queryExecTime[i] = (sum[i]*1e-9)/q[i];
}

void Results::getTxnLatency(vector<TransactionalClient*>& t){
    double sum[3] = {0.0};
    int tran[3] = {0};
    for(int i=0; i<UserInput::getTranClients(); i++) {
        for(int j=0; j<3; j++){
            sum[j] +=  t[i]->GetLatencySum(j+1);
            tran[j] += t[i]->GetLatencySize(j+1);
        }
    }
    for(int i=0; i<3; i++)
        txnLatency[i] = (sum[i]*1e-9)/tran[i];
}

void Results::getFreshness(vector<AnalyticalClient*>& a){
    int index = 0;
    for(int i=0; i<UserInput::getAnalClients(); i++){
	for(unsigned int j=0; j<a[i]->GetFreshness().size(); j++){
            freshness.push_back(a[i]->GetFreshness()[j]);
	}
    }
    sort(freshness.begin(), freshness.end());
    index = ceil(0.95*freshness.size());
    fresh_score = freshness[index-1];

    vector<double> ceil_freshness;
    for(int unsigned i=0; i<(unsigned int)freshness.size(); i++)
	    ceil_freshness.push_back(ceil(freshness[i]));
    double current, previous=-1;
    for(int unsigned i=0; i<(unsigned int)ceil_freshness.size(); i++){
	current  = ceil_freshness[i];
	if(current != previous){
		probFresh.push_back((double)count(ceil_freshness.begin(), ceil_freshness.end(), current)/freshness.size());
		freshValues.push_back(current);
		previous = current;
	}
    }
}

void Results::getTestDuration(vector<AnalyticalClient*>& a){
    for(int i=0; i<UserInput::getAnalClients(); i++){
        if(a[i]->GetTestDuration() > testDuration)
            testDuration  = a[i]->GetTestDuration();
    }
}

void Results::computeResults(vector<TransactionalClient*>& t, vector<AnalyticalClient*>& a){
    setTotalQueries(a);
    setTotalTxns(t);
    getTxnLatency(t);
    getQueryExecTime(a);
    if(UserInput::getAnalClients()>0){
    	getFreshness(a);
    }
    getTestDuration(a);
    saveResults();
}
