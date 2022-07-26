#include "Frontier.h"

void  Frontier::deleteTuples(){
    SQLHENV env = 0;
    Driver::setEnv(env);
    SQLHDBC dbc = 0;
    Driver::connectDB(env, dbc);
    SQLHSTMT stmt = 0;
    char c = ';';
    unsigned int num_of_stmt = SQLDialect::deleteTuplesStmt[UserInput::getdbChoice()].size();
    std::string q(1, c);
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    for(unsigned int i=0; i<num_of_stmt; i++){
        if(i!=num_of_stmt-1 && i!=num_of_stmt-2)
		Driver::executeStmtDiar(stmt, SQLDialect::deleteTuplesStmt[UserInput::getdbChoice()][i].c_str());
	else{
		Driver::executeStmtDiar(stmt, (SQLDialect::deleteTuplesStmt[UserInput::getdbChoice()][i]+std::to_string(UserInput::getLoSize())+q).c_str());
	}
    }
    Driver::freeStmtHandle(stmt);
    Driver::disconnectDB(dbc);
}


void Frontier::createFreshnessTable(int& tc){
    SQLHENV env = 0;
    Driver::setEnv(env);
    SQLHDBC dbc = 0;
    Driver::connectDB(env, dbc);
    SQLHSTMT stmt = 0;
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    std::string s_tc;
    for(unsigned int i=1; i<=(unsigned int)tc; i++){
	s_tc = std::to_string(i);
	Driver::executeStmtDiar(stmt, (SQLDialect::deleteFreshnessTableStmt[0].c_str()+s_tc+SQLDialect::deleteFreshnessTableStmt[1]).c_str());
    	Driver::executeStmtDiar(stmt, (SQLDialect::createFreshnessTableStmt[0].c_str()+s_tc+SQLDialect::createFreshnessTableStmt[1]).c_str());
	Driver::executeStmtDiar(stmt, (SQLDialect::populateFreshnessTableStmt[0]+s_tc+SQLDialect::populateFreshnessTableStmt[1]+s_tc+SQLDialect::populateFreshnessTableStmt[2]).c_str());
        if(UserInput::getdbChoice() == tidb){
		Driver::executeStmtDiar(stmt, (SQLDialect::populateFreshnessTableStmt[3]+s_tc+SQLDialect::populateFreshnessTableStmt[4]).c_str());
	}
    }
    Driver::freeStmtHandle(stmt);
    Driver::disconnectDB(dbc);
} 

double Frontier::runBenchmark(int& peak, int& choice){
    double peak_throughput = 0;
    int ac = 0, tc = 0;
    if(choice == 1){   // seeking max for transactional workload 
        tc = peak;
        ac = 0;
    } 
    else if(choice == 2){  // seeking max for analytical workload
        tc = 0;
        ac = peak;
    }
    createFreshnessTable(tc);
    auto startTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
    cout << "\nChoice: [3] Run Benchmark" << endl;
    cout << "START TIME of [3] " << ctime(&startTime) << endl;
    SQLHENV env = 0;
    Driver::setEnv(env);
    UserInput::setAnalyticalClients(ac);
    UserInput::setTransactionalClients(tc);
    auto* g = new Globals();
    GetFromDB::getNumOrders(reinterpret_cast<int &>(g->loOrderKey), env);
    g->barrierW = new Barrier(UserInput::getTranClients()+UserInput::getAnalClients());
    g->barrierT = new Barrier(UserInput::getTranClients()+UserInput::getAnalClients());
    g->typeOfRun = none;
    auto* workload = new Workload();
    workload->ExecuteWorkloads(g);
    auto* r = new Results();
    workload->ReturnResults(r);
    if(choice==1)
        peak_throughput = r->getTransactionalThroughput();
    else if(choice==2)
        peak_throughput = r->getAnalyticalThroughput();
    auto endTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
    cout << "\n[DONE] Choice: [3] Run Benchmark" << endl;
    cout << "START TIME of [3] " << ctime(&startTime) << endl;
    cout << "END TIME of [3] " << ctime(&endTime) << endl;
    delete g;
    g = NULL;
    delete workload;
    workload = NULL;
    delete r;
    r = NULL;
    deleteTuples();
    return peak_throughput; 
}

void Frontier::findMaxTCAC(){
    int choice[] = {1,2};
    double current_throughput, previous_throughput;
    int init_peak, previous_peak, final_peak;
    bool init_peak_found ;
    bool final_peak_found;
    int step;
    int i;
    int clients;
    for(int c=1; c >-1; c--){
        init_peak_found = false;
        final_peak_found = false;
        step = 8;
        i = 1;
        clients = 0;
        previous_peak = i;
        previous_throughput = runBenchmark(i, choice[c]);
        while(init_peak_found==false){
            clients = i*step;
            current_throughput = runBenchmark(clients, choice[c]);
            if(current_throughput - previous_throughput > 0.05*previous_throughput){
                previous_throughput = current_throughput;
                previous_peak = clients;
            }
            else {
                init_peak_found = true;
                init_peak = previous_peak;
            }
            i++;
        }
        i=1;
        while(final_peak_found==false){
            clients = init_peak+i;
            current_throughput = runBenchmark(clients, choice[c]);
            if(current_throughput - previous_throughput > 0.05*previous_throughput){
                previous_throughput = current_throughput;
                previous_peak = clients;
            }
            else{
                final_peak_found = true;
                final_peak = previous_peak;
            }
            i++;
        }
        if(choice[c]==1) 
            setMaxTC(final_peak);
        else
            setMaxAC(final_peak);
    }
    
}

void Frontier::findFrontier(){
    int max_ac = getMaxAC();
    int max_tc = getMaxTC();
    int tc, ac = 0;
    constexpr double ratios[6] = {0, 0.1, 0.2, 0.5, 0.8, 1};
    constexpr int ratios_num = sizeof(ratios)/sizeof(ratios[0]);
    for(int i=0; i<ratios_num; i++){
    	    for(int j=0; j<ratios_num; j++){
	        if(floor(ratios[i]*max_tc) == 0 && ratios[i]!=0)
		        tc = 1;
	        else
		        tc = floor(ratios[i]*max_tc);
	        if(floor(ratios[j]*max_ac) == 0 && ratios[j]!=0)
		        ac = 1;
	        else
		        ac = floor(ratios[j]*max_ac);	    
	    createFreshnessTable(tc);         // the freshness table is recreated everytime the number of T clients changes 
            auto startTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
            cout << "\nChoice: [3] Run Benchmark" << endl;
            cout << "START TIME of [3] " << ctime(&startTime) << endl;
            SQLHENV env = 0;
            Driver::setEnv(env);
            UserInput::setAnalyticalClients(ac);
            UserInput::setTransactionalClients(tc);
            auto* g = new Globals();
            GetFromDB::getNumOrders(reinterpret_cast<int &>(g->loOrderKey), env);
            g->barrierW = new Barrier(UserInput::getTranClients()+UserInput::getAnalClients());
            g->barrierT = new Barrier(UserInput::getTranClients()+UserInput::getAnalClients());
            g->typeOfRun = none;
            auto* workload = new Workload();
            workload->ExecuteWorkloads(g);
            auto* r = new Results();
            workload->ReturnResults(r);
            auto endTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
            cout << "\n[DONE] Choice: [3] Run Benchmark" << endl;
            cout << "START TIME of [3] " << ctime(&startTime) << endl;
            cout << "END TIME of [3] " << ctime(&endTime) << endl;
            delete g;
            g = NULL;
            delete workload;
            workload = NULL;
            delete r;
            r = NULL;
	    deleteTuples();
	    sleep(2);
      }
    }
}

void Frontier::setMaxTC(int& tc){
    max_tc = tc;
}

void Frontier::setMaxAC(int& ac){
    max_ac = ac;
}

int Frontier::getMaxTC(){
    return max_tc;
}

int Frontier::getMaxAC(){
    return max_ac;
}
