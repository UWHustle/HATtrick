#ifndef USERINPUT_H
#define USERINPUT_H

#include <string.h>
#include <iostream>
#include <string>
#include <cmath>

using namespace std;
enum database{postgres, systemx, tidb};
enum exectype{ps, sp}; // prepared statements or stored procedures

class UserInput{

	private:
        static const int BATCH_SIZE;
	    static const int SF;                // scaling factor
	    static const int custSize;          // size of the generated Customer table
        static const int suppSize;          // size of the generated Supplier table
        static const int partSize;          // size of the generated Part table
        static const int loSize;            // size of the generated LineOrder table
	    static string dataPath;             // path of the generated data
	    static string dsn;                  // data source name
	    static string dbUser;               // database user name
	    static string dbPwd;                // database password
        static int work;                    // (-h/--help) 0, generation of data (-gen) 1, run benchmark (-run) 2
        static int analClients;             // #of analytical clients
        static int tranClients;             // #of transactional clients
        //static float fractionT;           // the fraction of transactional clients
	    static int testDuration;            // duration of test in seconds
	    static int warmUpDuration;          // warm up duration in sec
	    static string delimiter;            // selected delimiter
	    static database dbChoice;           // connect to desired database
		static exectype execType; 			// execution type, prepared statements or stored procedures 

	public:
        static const int getBatchSize();
	    static const int getSF();
	    static const int getCustSize();
	    static const int getSuppSize();
	    static const int getPartSize();
	    static const int getLoSize();
        static string getDataPath();
        static string getDSN();
        static string getDBUser();
        static string getDBPwd();
	    static int getWork();
        static int getAnalClients();
        static int getTranClients();
	static void setAnalyticalClients(int& ac);
	static void setTransactionalClients(int& tc);
        static float getFractionT();
        static int getTestDuration();
        static int getWarmUpDuration();
	    static string getDelimiter();
	    static void processUserIn(int argc, char* argv[]);
	    static database& getdbChoice();
		static exectype& getExecType();
};
#endif