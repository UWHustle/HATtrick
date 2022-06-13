
#include "UserInput.h"

const int UserInput::BATCH_SIZE = 64;
const int UserInput::SF=1;  
const int UserInput::custSize =  30000 * UserInput::SF;
const int UserInput::suppSize =  2000  * UserInput::SF;
const int UserInput::partSize =  200000 * static_cast <int>(floor(1+log2(UserInput::SF)));
const int UserInput::loSize =    1500000 * UserInput::SF;
string UserInput::dataPath;
string UserInput::dsn;
string UserInput::dsn2;
string UserInput::dbUser;
string UserInput::dbPwd;
int UserInput::work;
int UserInput::analClients;
int UserInput::tranClients;
//float UserInput::fractionT;
int UserInput::testDuration;
int UserInput::warmUpDuration;
string UserInput::delimiter="!";
database UserInput::dbChoice;
exectype UserInput::execType;

const int UserInput::getBatchSize(){
    return UserInput::BATCH_SIZE;
}

const int  UserInput::getSF(){
    return UserInput::SF;
}

const int UserInput::getCustSize(){
    return UserInput::custSize;
}

const int UserInput::getSuppSize(){
    return UserInput::suppSize;
}

const int UserInput::getPartSize(){
    return UserInput::partSize;
}

const int UserInput::getLoSize(){
    return UserInput::loSize;
}

string UserInput::getDataPath(){
    return UserInput::dataPath;
}

string UserInput::getDSN(){
    return UserInput::dsn;
}

string UserInput::getDSN2(){
    return UserInput::dsn2;
}

string UserInput::getDBUser(){
    return UserInput::dbUser;
}

string UserInput::getDBPwd(){
    return UserInput::dbPwd;
}

int UserInput::getWork(){
    return UserInput::work;
}

int UserInput::getAnalClients(){
    return UserInput::analClients;
}

int UserInput::getTranClients(){
    return UserInput::tranClients;
}


void UserInput::setAnalyticalClients(int& ac){
    UserInput::analClients = ac;
}

void UserInput::setTransactionalClients(int& tc){
    UserInput::tranClients = tc;
}

/*float UserInput::getFractionT(){return UserInput::fractionT;}*/

int UserInput::getTestDuration(){
    return UserInput::testDuration;
}

int UserInput::getWarmUpDuration(){
    return UserInput::warmUpDuration;
}

string UserInput::getDelimiter(){
    return UserInput::delimiter;
}

void UserInput::processUserIn(int argc, char* argv[]){
    int found = 0;
    for(int i=0; i<argc; i++){
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            cout << "Options:"
                    "\n1. Generate initial data:\n"
                    "    [-gen]\n"
                    "    -pa <output path>\n"
                    "    -d <delimiter>\n"
                    "\n2. Initialize DB:\n"
                    "    [-init]\n"
                    "    -dsn <data source name>\n"
                    "    -usr <DB username>\n"
                    "    -pwd <DB password>\n"
                    "    -pa <data path>\n"
                    "    -db <DB choice [postgres, system-x, tidb, mysql]>\n"
                    "    -t  <prepared statements or stored procedures [ps, sp]>\n"
                    "\n3. Compute frontier:\n"
                    "    [-frontier]\n"
                    "    -dsn <data source name>\n"
                    "    -usr <DB username>\n"
                    "    -pwd <DB password>\n"
                    "    -wd <warm up duration in sec>\n"
                    "    -td <test duration in sec>\n"
                    "    -t  <prepared statements or stored procedures [ps, sp]>\n"
                    "    -db <DB choice [postgres, system-x, tidb, mysql]>\n"                    
                    "\n4. Run benchmark [one experiment]:\n"
                    "    [-run]\n"
                    "    -dsn <data source name>\n"
		    "    -dsn2 <data source name2>\n"
                    "    -usr <DB username>\n"
                    "    -pwd <DB password>\n"
                    "    -ac <number of analytical clients>\n"
                    "    -tc <number of transactional clients>\n"
                    "    -wd <warm up duration in sec>\n"
                    "    -td <test duration in sec>\n"
                    "    -t  <prepared statements or stored procedures [ps, sp]>\n"
                    "    -db <DB choice [postgres, system-x, tidb, mysql]>\n\n";
            UserInput::work = 0;
            break;
        }

        else if (strcmp(argv[i], "-gen") == 0) {
            found = 1;
            UserInput::work = 1;
            break;
        }
        else if (strcmp(argv[i], "-init") == 0) {
                    found = 1;
                    UserInput::work = 2;
                    break;
        }
        else if (strcmp(argv[i], "-frontier") == 0) {
                    found = 1;
                    UserInput::work = 3;
                    break;
        }
        else if (strcmp(argv[i], "-run") == 0) {
                    found = 1;
                    UserInput::work = 4;
                    break;
        }
    }
    if (found ==1 ){
        if(UserInput::work == 1){
            for(int i=0; i<argc; i++){
                    if(strcmp(argv[i], "-pa") == 0 )  UserInput::dataPath = string(argv[i+1]);
                    //else if(strcmp(argv[i], "-d") == 0 )  UserInput::delimiter = string(argv[i+1]);
            }
            found = 0;
        }
        else if (UserInput::work == 2){
            for(int i=0; i<argc; i++){
                    if(strcmp(argv[i], "-dsn") == 0 )  UserInput::dsn = string(argv[i+1]);
                    else if(strcmp(argv[i], "-usr") == 0 )  UserInput::dbUser = string(argv[i+1]);
                    else if(strcmp(argv[i], "-pwd") == 0 )  UserInput::dbPwd = string(argv[i+1]);
                    else if(strcmp(argv[i], "-pa") == 0 )  UserInput::dataPath = string(argv[i+1]);
                    else if(strcmp(argv[i], "-db") == 0 ) {
                        if(strcmp(argv[i+1], "postgres") == 0 ) UserInput::dbChoice = postgres;
                        else if(strcmp(argv[i+1], "system-x") == 0 ) UserInput::dbChoice = systemx;
                        else if(strcmp(argv[i+1], "tidb") == 0 ) UserInput::dbChoice = tidb;
                        else if(strcmp(argv[i+1], "mysql") == 0) UserInput::dbChoice = mysql;
                    }
                    else if(strcmp(argv[i], "-t") == 0 ) {
                        if(strcmp(argv[i+1], "ps") == 0 ) UserInput::execType = ps;
                        else if(strcmp(argv[i+1], "sp") == 0 ) UserInput::execType = sp;
                    }
            }
            found = 0;
        }
        else if (UserInput::work == 3){
            for(int i=0; i<argc; i++){
                    if(strcmp(argv[i], "-dsn") == 0 )  UserInput::dsn = string(argv[i+1]);
                    else if(strcmp(argv[i], "-usr") == 0 )  UserInput::dbUser = string(argv[i+1]);
                    else if(strcmp(argv[i], "-pwd") == 0 )  UserInput::dbPwd = string(argv[i+1]);
                    else if(strcmp(argv[i], "-dsn2") == 0 )  UserInput::dsn2 = string(argv[i+1]);
                    else if(strcmp(argv[i], "-wd") == 0 )  UserInput::warmUpDuration = atoi(argv[i+1]);
                    else if(strcmp(argv[i], "-td") == 0 )  UserInput::testDuration= atoi(argv[i+1]);
                    else if(strcmp(argv[i], "-db") == 0 ) {
                        if(strcmp(argv[i+1], "postgres") == 0 ) UserInput::dbChoice = postgres;
                        else if(strcmp(argv[i+1], "system-x") == 0 ) UserInput::dbChoice = systemx;
                        else if(strcmp(argv[i+1], "tidb") == 0 ) UserInput::dbChoice = tidb;
                        else if(strcmp(argv[i+1], "mysql") == 0) UserInput::dbChoice = mysql;

                    }
                    else if(strcmp(argv[i], "-t") == 0 ) {
                        if(strcmp(argv[i+1], "ps") == 0 ) UserInput::execType = ps;
                        else if(strcmp(argv[i+1], "sp") == 0 ) UserInput::execType = sp;
                    }
            }
            found = 0;
        }
        else if (UserInput::work == 4){
            for(int i=0; i<argc; i++){
                    if(strcmp(argv[i], "-dsn") == 0 )  UserInput::dsn = string(argv[i+1]);
		    else if(strcmp(argv[i], "-dsn2") == 0 )  UserInput::dsn2 = string(argv[i+1]);
                    else if(strcmp(argv[i], "-usr") == 0 )  UserInput::dbUser = string(argv[i+1]);
                    else if(strcmp(argv[i], "-pwd") == 0 )  UserInput::dbPwd = string(argv[i+1]);
                    else if(strcmp(argv[i], "-ac") == 0 )  UserInput::analClients = atoi(argv[i+1]);
                    else if(strcmp(argv[i], "-tc") == 0 )  UserInput::tranClients = atoi(argv[i+1]);
                    else if(strcmp(argv[i], "-wd") == 0 )  UserInput::warmUpDuration = atoi(argv[i+1]);
                    else if(strcmp(argv[i], "-td") == 0 )  UserInput::testDuration= atoi(argv[i+1]);
                    else if(strcmp(argv[i], "-db") == 0 ) {
                        if(strcmp(argv[i+1], "postgres") == 0 ) UserInput::dbChoice = postgres;
                        else if(strcmp(argv[i+1], "system-x") == 0 ) UserInput::dbChoice = systemx;
                        else if(strcmp(argv[i+1], "tidb") == 0 ) UserInput::dbChoice = tidb;
                        else if(strcmp(argv[i+1], "mysql") == 0) UserInput::dbChoice = mysql;
                    }
                    else if(strcmp(argv[i], "-t") == 0 ) {
                        if(strcmp(argv[i+1], "ps") == 0 ) UserInput::execType = ps;
                        else if(strcmp(argv[i+1], "sp") == 0 ) UserInput::execType = sp;
                    }
            }
            found = 0;
        }
    }
}

database& UserInput::getdbChoice(){
    return UserInput::dbChoice;
}

exectype& UserInput::getExecType(){
    return UserInput::execType;
}

