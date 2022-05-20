#include "DataGen.h"
#include "Driver.h"
#include "DBInit.h"
#include "Barrier.h"
#include "Workload.h"
#include "GetFromDB.h"
#include "Globals.h"
#include "Results.h"
#include "Frontier.h"
#include <sql.h>
#include <sqltypes.h>
#include <iostream>
#include <functional>
#include <ctime>
using namespace std;

int main(int argc, char* argv[]){
    /* Read user input for flag initialization. */
    UserInput::processUserIn(argc, argv);
    if(UserInput::getWork() == 1){             // User selected data generation.
        auto startTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
        cout << "\nChoice: [1] Data generation" << endl;
        cout << "START TIME of [1] " << ctime(&startTime) << endl;
        /* Generating initial data. */
        if(DataGen::dbGen() == 0)
            cout << "\nData generation completed!\n\n";
        else
            cout << "\nData generation failed.\n\n";
        auto endTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
        cout << "\n[DONE] Choice: [1] Data generation" << endl;
        cout << "END TIME of [1] " << ctime(&endTime) << endl;
    }
    else if(UserInput::getWork() == 2){        // User selected to initiate the database.
        auto startTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
        cout << "\nChoice: [2] Init database" << endl;
        cout << "START TIME of [2] " << ctime(&startTime) << endl;
        /* Connect to the DB. */
        SQLHENV env = 0;
        Driver::setEnv(env);
        SQLHDBC dbc = 0;
        Driver::connectDB(env, dbc);
        /* Create Schema. */
        SQLHSTMT stmt = 0;
        SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
        if(DBInit::dropSchema(stmt) == 0)
            cout << "\nStep1: Previous schema was deleted!\n\n";
        else
            cout << "\n\tStep1: Previous schema deletion failed!\n\n";

        if(DBInit::createSchema(stmt) == 0)
            cout << "\nStep2: New schema creation completed!\n\n";
        else
            cout << "\n\tStep2: New schema creation failed!\n\n";

        if(DBInit::bulkLoad(stmt) == 0)
            cout << "\nStep3: Bulk loading completed!\n\n";
        else
            cout << "\n\tStep3: Bulk loading failed!\n\n";

        if(DBInit::indexCreation(stmt) == 0)
            cout << "\nStep4: Index creation completed!\n\n";
        else
            cout << "\n\tStep4: Index creation failed!\n\n";
        Driver::freeStmtHandle(stmt);
        Driver::disconnectDB(dbc);
        //Driver::freeEnvHandle(env);
        cout << "\nDB initialization was successfully completed!\n\n";
        auto endTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
        cout << "\n[DONE] Choice: [2] Init database" << endl;
        cout << "END TIME of [2] " << ctime(&endTime) << endl;

    }

    else if(UserInput::getWork() == 3){         // User selected to run the benchmark
        auto* frontier = new Frontier();
        frontier->findMaxTCAC();
        int ac = frontier->getMaxAC();
        int tc = frontier->getMaxTC();
        frontier->setMaxTC(tc);
        frontier->setMaxAC(ac);
        frontier->findFrontier();
    }

    else if(UserInput::getWork() == 4){         // User selected to run the benchmark
        auto startTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
        cout << "\nChoice: [3] Run Benchmark" << endl;
        cout << "START TIME of [3] " << ctime(&startTime) << endl;
        SQLHENV env = 0;
        Driver::setEnv(env);
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
    }
    return 0;

}
