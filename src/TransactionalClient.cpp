#include "TransactionalClient.h"

void TransactionalClient::PrepareTransactionStmt(SQLHDBC &dbc){
    for(unsigned int i=0; i<SQLDialect::transactionalCommands.size(); i++){
        Driver::prepareStmt(dbc, GetTransactionPrepStmt(i), SQLDialect::transactionalCommands[i].c_str());
    }
}

int TransactionalClient::NewOrderTransactionPS(SQLHDBC& dbc){
    // Create a random LO_CUSTNAME.
    int custkey = DataSrc::uniformIntDist(1, UserInput::getCustSize());
    ostringstream ckey;
    ckey << setw(9) << setfill('0') << custkey;
    string c = "Customer#";
    char* custName = &(c.append(ckey.str()))[0];
    // Choose a random number of orders 
    int numOrders = DataSrc::uniformIntDist(1, 7);
    char* partKeys = 0; char* suppNames = 0;  char* dateNames = 0;  char* ordPriorities = 0;  char* shipPriorities = 0;  \
    char* quantities = 0; char* extendedPrices = 0;  char* discounts = 0;  char* revenues = 0;  char* supplyCosts = 0; \
    char* taxes = 0;  char* shipModes = 0;
    string partKeysBuf, suppNamesBuf, dateNamesBuf, ordPrioritiesBuf, shipPrioritiesBuf, quantitiesBuf, \
    extendedPricesBuf, discountsBuf, revenuesBuf, supplyCostsBuf, taxesBuf, shipModesBuf;
    int suppKey = 0, month = 0, quantity = 0, discount = 0;
    int orderKey =  GetLoOrderKey();
    int ret = -1;
    for(int i=0; i<numOrders; i++){
        // Create a random LO_PARTKEY.
        if(i!=0){
            partKeysBuf.append(",");
            suppNamesBuf.append(",");
            dateNamesBuf.append("^");
            ordPrioritiesBuf.append(",");
            shipPrioritiesBuf.append(",");
            quantitiesBuf.append(",");
            extendedPricesBuf.append(",");
            discountsBuf.append(",");
            revenuesBuf.append(",");
            supplyCostsBuf.append(",");
            taxesBuf.append(",");
            shipModesBuf.append(",");
        }
        partKeysBuf.append(to_string(DataSrc::uniformIntDist(1, UserInput::getPartSize())));
        // Create a random LO_SUPPNAME.
        suppKey = DataSrc::uniformIntDist(1, UserInput::getSuppSize());
        ostringstream skey;
        skey << setw(9) << setfill('0') << suppKey;
        suppNamesBuf.append("Supplier#" + skey.str());
        // Create a random LO_DATENAME.
        month = DataSrc::uniformIntDist(1, 12);
        dateNamesBuf.append(DataSrc::getMonthName(month-1) + " " + \
                        to_string(DataSrc::uniformIntDist(1,DataSrc::getMonthDay(month-1))) + \
                        ", " + DataSrc::getYear(DataSrc::uniformIntDist(1, 7)-1));
        // Create the other data of the current lineorder randomly.
        ordPrioritiesBuf.append(DataSrc::getOrdPriority(DataSrc::uniformIntDist(0,4)));
        shipPrioritiesBuf.append(to_string(DataSrc::uniformIntDist(0,1)));
        quantity = DataSrc::uniformIntDist(1, 50);
        quantitiesBuf.append(to_string(quantity));
        extendedPricesBuf.append(to_string(quantity)); 
        discount = DataSrc::uniformIntDist(0, 10);
        discountsBuf.append(to_string(discount));
        revenuesBuf.append(to_string(double(100-discount)/100)); 
        supplyCostsBuf.append(to_string(DataSrc::uniformRealDist(1.00, 1000.00)));
        taxesBuf.append(to_string(DataSrc::uniformIntDist(0,8)));
        shipModesBuf.append(DataSrc::getShipMode(DataSrc::uniformIntDist(0,6)));
    }
    partKeys = &(partKeysBuf)[0];
    suppNames = &(suppNamesBuf)[0];
    dateNames = &(dateNamesBuf)[0];
    ordPriorities =  &(ordPrioritiesBuf)[0];
    shipPriorities = &(shipPrioritiesBuf)[0];
    quantities = &(quantitiesBuf)[0];
    extendedPrices = &(extendedPricesBuf)[0];
    discounts = &(discountsBuf)[0];
    revenues = &(revenuesBuf)[0];
    supplyCosts = &(supplyCostsBuf)[0];
    taxes = &(taxesBuf)[0];
    shipModes = &(shipModesBuf)[0];

    // Call the NewOrder txn
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &GetTransactionStmt());
    Driver::bindIntParam(GetTransactionStmt(), orderKey, 1);
    Driver::bindIntParam(GetTransactionStmt(), numOrders, 2);
    Driver::bindCharParam(GetTransactionStmt(), custName, 25, 3);
    Driver::bindCharParam(GetTransactionStmt(),  partKeys, 0, 4);
    Driver::bindCharParam(GetTransactionStmt(),  suppNames, 0, 5);
    Driver::bindCharParam(GetTransactionStmt(),  dateNames, 0, 6);
    Driver::bindCharParam(GetTransactionStmt(),  ordPriorities, 0, 7);
    Driver::bindCharParam(GetTransactionStmt(),  shipPriorities, 0, 8);
    Driver::bindCharParam(GetTransactionStmt(),  quantities, 0, 9);
    Driver::bindCharParam(GetTransactionStmt(),  extendedPrices, 0, 10);
    Driver::bindCharParam(GetTransactionStmt(),  discounts, 0, 11);
    Driver::bindCharParam(GetTransactionStmt(),  revenues, 0, 12);
    Driver::bindCharParam(GetTransactionStmt(),  supplyCosts, 0, 13);
    Driver::bindCharParam(GetTransactionStmt(),  taxes, 0, 14);
    Driver::bindCharParam(GetTransactionStmt(),  shipModes, 0, 15);
    while(ret != 0)
        ret = Driver::executeStmtDiar(GetTransactionStmt(), SQLDialect::transactionalQueries[UserInput::getdbChoice()][0].c_str());
    Driver::freeStmtHandle(GetTransactionStmt());
    if (ret == 0) return 1;
    else return 0;
}

int TransactionalClient::NewOrderTransactionSS(SQLHDBC& dbc){
    int ret = -1;
    // Get random customer key CUSTKEY.
    int custkey = DataSrc::uniformIntDist(1, UserInput::getCustSize());
    ostringstream ckey;
    ckey << setw(9) << setfill('0') << custkey;
    string custName = "Customer#" + ckey.str();
    char* c_name = &custName[0];    // get random customer name
    int partkey1 = DataSrc::uniformIntDist(1, UserInput::getPartSize());
    int partkey2 = DataSrc::uniformIntDist(1, UserInput::getPartSize());
    int partkey3 = DataSrc::uniformIntDist(1, UserInput::getPartSize());
    int partkey4 = DataSrc::uniformIntDist(1, UserInput::getPartSize());
    int suppkey = DataSrc::uniformIntDist(1, UserInput::getSuppSize());
    int suppkey2 = DataSrc::uniformIntDist(1, UserInput::getSuppSize());
    int suppkey3 = DataSrc::uniformIntDist(1, UserInput::getSuppSize());
    int suppkey4 = DataSrc::uniformIntDist(1, UserInput::getSuppSize());
    ostringstream skey1;
    skey1 << setw(9) << setfill('0') << suppkey;
    string suppName1 = "Supplier#" + skey1.str();
    char* s_name = &suppName1[0];    // get random supplier name
    ostringstream skey2;
    skey2 << setw(9) << setfill('0') << suppkey2;
    string suppName2 = "Supplier#" + skey2.str();
    char* s_name2 = &suppName2[0];    // get random supplier name
    ostringstream skey3;
    skey3 << setw(9) << setfill('0') << suppkey3;
    string suppName3 = "Supplier#" + skey3.str();
    char* s_name3 = &suppName3[0];    // get random supplier name        
    ostringstream skey4;
    skey4 << setw(9) << setfill('0') << suppkey4;
    string suppName4 = "Supplier#" + skey4.str();
    char* s_name4 = &suppName4[0];    // get random supplier name

    //For a random DATE.
    int choice1 = DataSrc::uniformIntDist(1, 12);
    string s = DataSrc::getMonthName(choice1-1) + " " + to_string(DataSrc::getMonthDay(choice1-1))  + ", " \
    + DataSrc::getYear(DataSrc::uniformIntDist(1, 7)-1);
    char* d_date = &s[0];

    int choice2 = DataSrc::uniformIntDist(1, 12);
    string s2 = DataSrc::getMonthName(choice2-1) + " " + to_string(DataSrc::getMonthDay(choice2-1))  + ", " \
    + DataSrc::getYear(DataSrc::uniformIntDist(1, 7)-1);
    char* d_date2 = &s2[0];

    int choice3 = DataSrc::uniformIntDist(1, 12);
    string s3 = DataSrc::getMonthName(choice3-1) + " " + to_string(DataSrc::getMonthDay(choice3-1))  + ", " \
    + DataSrc::getYear(DataSrc::uniformIntDist(1, 7)-1);
    char* d_date3 = &s3[0];

    int choice4 = DataSrc::uniformIntDist(1, 12);
    string s4 = DataSrc::getMonthName(choice4-1) + " " + to_string(DataSrc::getMonthDay(choice4-1))  + ", " \
    + DataSrc::getYear(DataSrc::uniformIntDist(1, 7)-1);
    char* d_date4 = &s4[0];

    // Create the other data of the current lineorder randomly.
    string ordpriority = DataSrc::getOrdPriority(DataSrc::uniformIntDist(0,4));
    char* ord =  &ordpriority[0];
    int shippriority = DataSrc::uniformIntDist(0,1);
    char* shipp = &to_string(shippriority)[0];
    int quantity = DataSrc::uniformIntDist(1, 50);
    double extendedprice = quantity;  
    int discount =  DataSrc::uniformIntDist(0, 10);
    double revenue = (extendedprice*(100-discount))/100;
    double supplycost = DataSrc::uniformRealDist(1.00, 1000.00);
    int tax =  DataSrc::uniformIntDist(0,8);
    string shipmode = DataSrc::getShipMode(DataSrc::uniformIntDist(0,6));
    char* shipm = &shipmode[0];
    // Execute the insertion to the lineorder table
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &GetTransactionStmt());
    Driver::bindIntParam(GetTransactionStmt(), GetLoOrderKey(), 1);
    Driver::bindCharParam(GetTransactionStmt(), c_name, 25, 2);
    Driver::bindIntParam(GetTransactionStmt(), partkey1, 3);
    Driver::bindIntParam(GetTransactionStmt(), partkey2, 4);
    Driver::bindIntParam(GetTransactionStmt(), partkey3, 5);
    Driver::bindIntParam(GetTransactionStmt(), partkey4, 6);
    Driver::bindCharParam(GetTransactionStmt(), s_name, 25, 7);
    Driver::bindCharParam(GetTransactionStmt(), s_name2, 25, 8);
    Driver::bindCharParam(GetTransactionStmt(), s_name3, 25, 9);
    Driver::bindCharParam(GetTransactionStmt(), s_name4, 25, 10);
    Driver::bindCharParam(GetTransactionStmt(), d_date, 18, 11);
    Driver::bindCharParam(GetTransactionStmt(), d_date2, 18, 12);
    Driver::bindCharParam(GetTransactionStmt(), d_date3, 18, 13);
    Driver::bindCharParam(GetTransactionStmt(), d_date4, 18, 14);
    Driver::bindCharParam(GetTransactionStmt(), ord, 15, 15);
    Driver::bindCharParam(GetTransactionStmt(), shipp, 1, 16);
    Driver::bindIntParam(GetTransactionStmt(), quantity, 17);
    Driver::bindDecParam(GetTransactionStmt(), extendedprice, 18);
    Driver::bindIntParam(GetTransactionStmt(), discount, 19);
    Driver::bindDecParam(GetTransactionStmt(), revenue, 20);
    Driver::bindDecParam(GetTransactionStmt(), supplycost, 21);
    Driver::bindIntParam(GetTransactionStmt(), tax, 22);
    Driver::bindCharParam(GetTransactionStmt(), shipm, 10, 23);
    while(ret != 0)
        ret = Driver::executeStmtDiar(GetTransactionStmt(), SQLDialect::transactionalQueries[UserInput::getdbChoice()][0].c_str());
    Driver::freeStmtHandle(GetTransactionStmt());
    if (ret == 0) return 1;
    else return 0;
}

void TransactionalClient::NewOrderTransaction(SQLHDBC& dbc){
    // Get random customer key CUSTKEY.
    Driver::autoCommitOff(dbc);
    //Driver::executeStmt(GetTransactionPrepStmt(11));
    int custkey = DataSrc::uniformIntDist(1, UserInput::getCustSize());
    // ----------------------------------------------------------
    ostringstream ckey;
    ckey << setw(9) << setfill('0') << custkey;
    string custName = "Customer#" + ckey.str();
    char* c_name = &custName[0];    // get random customer name
    Driver::bindCharParam(GetTransactionPrepStmt(0), c_name, 23, 1);
    Driver::executeStmt(GetTransactionPrepStmt(0));
    Driver::fetchData(GetTransactionPrepStmt(0));
    Driver::getIntData(GetTransactionPrepStmt(0), 1, custkey);
    Driver::resetStmt(GetTransactionPrepStmt(0));
    // ----------------------------------------------------------

    // Choose a random number for the # of parts of the order.
    int numOrders = DataSrc::uniformIntDist(1, 7);
    // Transaction starts.
    // Set auto commit off, all the commands will commit at the end of the transaction command.
    //Driver::autoCommitOff(dbc);
    int partkey=0, choice=0, datekey=0, shippriority=0, quantity=0, discount=0, tax=0;
    double p_price=0, extendedprice=0, revenue=0, supplycost=0;
    string suppName, s, ordpriority, shipmode;
    for(int i=0; i<numOrders; i++){
        //For a random PART get the P_PRICE.
        partkey = DataSrc::uniformIntDist(1, UserInput::getPartSize());
        Driver::bindIntParam(GetTransactionPrepStmt(1), partkey, 1);
        Driver::executeStmt(GetTransactionPrepStmt(1));
        Driver::fetchData(GetTransactionPrepStmt(1));
        p_price = 0;
        Driver::getDoubleData(GetTransactionPrepStmt(1), 1, p_price);
        Driver::resetStmt(GetTransactionPrepStmt(1));

        // Get random supplier key SUPPKEY.
        int suppkey = DataSrc::uniformIntDist(1, UserInput::getSuppSize());

        // ----------------------------------------------------------
        ostringstream skey;
        skey << setw(9) << setfill('0') << suppkey;
        suppName = "Supplier#" + skey.str();
        char* s_name = &suppName[0];    // get random supplier name
        Driver::bindCharParam(GetTransactionPrepStmt(2), s_name, 23, 1);
        Driver::executeStmt(GetTransactionPrepStmt(2));
        Driver::fetchData(GetTransactionPrepStmt(2));
        Driver::getIntData(GetTransactionPrepStmt(2), 1, suppkey);
        Driver::resetStmt(GetTransactionPrepStmt(2));
        // ----------------------------------------------------------

        //For a random DATE get the DATEKEY.
        choice = DataSrc::uniformIntDist(1, 12);
        s = DataSrc::getMonthName(choice-1) + " " + to_string(DataSrc::getMonthDay(choice-1))  + ", " \
        + DataSrc::getYear(DataSrc::uniformIntDist(1, 7)-1);
        char* d_date = &s[0];
        Driver::bindCharParam(GetTransactionPrepStmt(3), d_date, 18, 1);
        Driver::executeStmt(GetTransactionPrepStmt(3));
        Driver::fetchData(GetTransactionPrepStmt(3));
        datekey = 0;
        Driver::getIntData(GetTransactionPrepStmt(3), 1, datekey);
        Driver::resetStmt(GetTransactionPrepStmt(3));

        // Create the other data of the current lineorder randomly.
        ordpriority = DataSrc::getOrdPriority(DataSrc::uniformIntDist(0,4));
        char* ord =  &ordpriority[0];
        shippriority = DataSrc::uniformIntDist(0,1);
        char* shipp = &to_string(shippriority)[0];
        quantity = DataSrc::uniformIntDist(1, 50);
        extendedprice = quantity * p_price;
        discount =  DataSrc::uniformIntDist(0, 10);
        revenue = (extendedprice*(100-discount))/100;
        supplycost = DataSrc::uniformRealDist(1.00, 1000.00);
        tax =  DataSrc::uniformIntDist(0,8);
        shipmode = DataSrc::getShipMode(DataSrc::uniformIntDist(0,6));
        char* shipm = &shipmode[0];

        // Execute the insertion to the lineorder table.
        Driver::bindIntParam(GetTransactionPrepStmt(4), GetLoOrderKey(), 1);
        Driver::bindIntParam(GetTransactionPrepStmt(4), i, 2);
        Driver::bindIntParam(GetTransactionPrepStmt(4), custkey, 3);
        Driver::bindIntParam(GetTransactionPrepStmt(4), partkey, 4);
        Driver::bindIntParam(GetTransactionPrepStmt(4), suppkey, 5);
        Driver::bindIntParam(GetTransactionPrepStmt(4), datekey, 6);
        Driver::bindCharParam(GetTransactionPrepStmt(4), ord, 15,  7);
        Driver::bindCharParam(GetTransactionPrepStmt(4), shipp, 1,  8);
        Driver::bindIntParam(GetTransactionPrepStmt(4), quantity, 9);
        Driver::bindDecParam(GetTransactionPrepStmt(4), extendedprice, 10);
        Driver::bindIntParam(GetTransactionPrepStmt(4), discount, 11);
        Driver::bindDecParam(GetTransactionPrepStmt(4), revenue, 12);
        Driver::bindDecParam(GetTransactionPrepStmt(4), supplycost, 13);
        Driver::bindIntParam(GetTransactionPrepStmt(4), tax, 14);
        Driver::bindIntParam(GetTransactionPrepStmt(4), datekey, 15);
        Driver::bindCharParam(GetTransactionPrepStmt(4), shipm, 10, 16);
        Driver::executeStmt(GetTransactionPrepStmt(4));
        Driver::resetStmt(GetTransactionPrepStmt(4));
        // End of transaction.
        Driver::endOfTransaction(dbc);
    }
}

int TransactionalClient::PaymentTransactionSP(SQLHDBC& dbc){
    // Get random customer key CUSTKEY 
    int custkey  = DataSrc::uniformIntDist(1, UserInput::getCustSize());
    // Get random supplier key SUPPKEY
    int suppkey = DataSrc::uniformIntDist(1, UserInput::getSuppSize());
    // Get random paymnet amount X
    double payAmount =  DataSrc::uniformRealDist(1.00, 104950.00);
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &GetTransactionStmt());
    Driver::bindIntParam(GetTransactionStmt(), custkey, 1);
    Driver::bindIntParam(GetTransactionStmt(), suppkey, 2);
    Driver::bindDecParam(GetTransactionStmt(), payAmount, 3);  
    Driver::bindIntParam(GetTransactionStmt(), GetLoOrderKey(), 4);  
    int ret = -1;
    while(ret != 0)
	    ret = Driver::executeStmtDiar(GetTransactionStmt(), SQLDialect::transactionalQueries[UserInput::getdbChoice()][1].c_str());
    Driver::freeStmtHandle(GetTransactionStmt());
    if (ret == 0) return 1;
    else return 0;
}


void TransactionalClient::PaymentTransaction(SQLHDBC& dbc){
    // Get random customer key CUSTKEY
    int custkey = 0;
    int p = DataSrc::uniformIntDist(1, 100);
    if(p<=100)
        custkey =  DataSrc::uniformIntDist(1, UserInput::getCustSize());
    else {
        int custId = 0;
        vector<pair<string, int>> data;
        SQLCHAR custName[26] = {0};
        int index = DataSrc::uniformIntDist(0, 24);
        string nation = DataSrc::getSelectedNation(index);
        char* n_name = &nation[0];    // get random nation name
        Driver::bindCharParam(GetTransactionPrepStmt(5), n_name, 16, 1);
        Driver::executeStmt(GetTransactionPrepStmt(5));
        SQLLEN indicator = 0;
        SQLBindCol(GetTransactionPrepStmt(5), 1, SQL_C_DEFAULT, &custId, 0, &indicator);
        indicator = 0;
        SQLBindCol(GetTransactionPrepStmt(5), 2, SQL_C_CHAR, reinterpret_cast<char*>(custName), 26, &indicator);
        SQLRETURN f;
        while(f == SQL_SUCCESS){
            f = SQLFetch(GetTransactionPrepStmt(5));
            if (f == SQL_SUCCESS){
                string cName(reinterpret_cast<char*>(custName));
                data.push_back(make_pair(cName, custId));
            }
            else
                break;
        }
        sort(data.begin(), data.end());
        unsigned int middle = ceil((double )data.size()/2);
        Driver::resetStmt(GetTransactionPrepStmt(5));
        custkey =  data[middle].second;
    }
    // Get random supplier key SUPPKEY
    int suppkey = DataSrc::uniformIntDist(1, UserInput::getSuppSize());
    // Get random paymnet amount X
    double payAmount = DataSrc::uniformRealDist(1.00, 104950.00);
    // Transaction starts.
    // Set auto commit off, all the commands will commit at the end of the transaction command.
    Driver::autoCommitOff(dbc);
    //Driver::executeStmt(GetTransactionPrepStmt(11));
    // Update in customer's table.
    Driver::bindIntParam(GetTransactionPrepStmt(6), custkey, 1);
    Driver::executeStmt(GetTransactionPrepStmt(6));
    Driver::resetStmt(GetTransactionPrepStmt(6));
    // Update in supplier's table.
    Driver::bindDecParam(GetTransactionPrepStmt(7), payAmount, 1);
    Driver::bindIntParam(GetTransactionPrepStmt(7), suppkey, 2);
    Driver::executeStmt(GetTransactionPrepStmt(7));
    Driver::resetStmt(GetTransactionPrepStmt(7));
    // Insertion in History table.
    Driver::bindIntParam(GetTransactionPrepStmt(8), GetLoOrderKey(), 1);
    Driver::bindIntParam(GetTransactionPrepStmt(8), custkey, 2);
    Driver::bindDecParam(GetTransactionPrepStmt(8), payAmount, 3);
    Driver::executeStmt(GetTransactionPrepStmt(8));
    Driver::resetStmt(GetTransactionPrepStmt(8));
    // End of transaction.
    Driver::endOfTransaction(dbc);
}

int TransactionalClient::CountOrdersTransactionSP(SQLHDBC& dbc){    
    int custkey =  DataSrc::uniformIntDist(1, UserInput::getCustSize());
    ostringstream ckey;
    int ret = -1;
    ckey << setw(9) << setfill('0') << custkey;
    string custName = "Customer#" + ckey.str();
    char* c_name = &custName[0];    // get random customer name
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &GetTransactionStmt());
    Driver::bindCharParam(GetTransactionStmt(), c_name, 26, 1);
    while(ret != 0)
        ret = Driver::executeStmtDiar(GetTransactionStmt(), SQLDialect::transactionalQueries[UserInput::getdbChoice()][2].c_str());
    Driver::freeStmtHandle(GetTransactionStmt());
    if (ret == 0) return 1;
    else return 0;
}


void TransactionalClient::CountOrdersTransaction(SQLHDBC& dbc){    
    int custkey = DataSrc::uniformIntDist(1, UserInput::getCustSize());
    ostringstream ckey;
    ckey << setw(9) << setfill('0') << custkey;
    string custName = "Customer#" + ckey.str();
    char* c_name = &custName[0];    // get random customer name
    Driver::autoCommitOff(dbc);
    //Driver::executeStmt(GetTransactionPrepStmt(11));
    Driver::bindCharParam(GetTransactionPrepStmt(0), c_name, 23, 1);
    Driver::executeStmt(GetTransactionPrepStmt(0));
    Driver::fetchData(GetTransactionPrepStmt(0));
    Driver::getIntData(GetTransactionPrepStmt(0), 1, custkey);
    Driver::resetStmt(GetTransactionPrepStmt(0));
    // Transaction starts.
    // Set auto commit off, all the commands will commit at the end of the transaction command.
    Driver::bindIntParam(GetTransactionPrepStmt(9), custkey, 1);
    Driver::executeStmt(GetTransactionPrepStmt(9));
    Driver::fetchData(GetTransactionPrepStmt(9));
    int count = 0;
    Driver::getLongData(GetTransactionPrepStmt(9), 1, count);
    Driver::resetStmt(GetTransactionPrepStmt(9));
    // End of transaction.
    Driver::endOfTransaction(dbc);
}

void TransactionalClient::FreshnessTransactionSP(SQLHDBC& dbc){
    int ret = -1;
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &GetTransactionStmt());
    while(ret != 0)
        ret = Driver::executeStmtDiar(GetTransactionStmt(), SQLDialect::transactionalQueries[UserInput::getdbChoice()][3].c_str());
    Driver::freeStmtHandle(GetTransactionStmt());
}

void TransactionalClient::FreshnessTransaction(SQLHDBC& dbc){
    Driver::autoCommitOff(dbc);
    //Driver::executeStmt(GetTransactionPrepStmt(11));
    Driver::executeStmt(GetTransactionPrepStmt(10));
    Driver::resetStmt(GetTransactionPrepStmt(10));
    // End of transaction.
    Driver::endOfTransaction(dbc);

}

SQLHSTMT& TransactionalClient::GetTransactionStmt(){
    return tStmt;
}

SQLHSTMT& TransactionalClient::GetTransactionPrepStmt(int idx){
    return ptStmt[idx];
}

void TransactionalClient::FreeTransactionStmt(){
    for(unsigned int i=0; i<SQLDialect::transactionalCommands.size(); i++)
        Driver::freeStmtHandle(ptStmt[i]);
}
void TransactionalClient::SetLoOrderKey(int& key){
    loOrderKey = key;
}

int& TransactionalClient::GetLoOrderKey(){
    return loOrderKey;
}

void TransactionalClient::SetClientNum(int& num){
    clientNum = num;
}

int TransactionalClient::GetClientNum(){
    return clientNum;
}
void TransactionalClient::IncrementTransactionNum(){
    transactionsNum++;
}

int TransactionalClient::GetTransactionNum(){
    return transactionsNum;
}

void TransactionalClient::SetThreadNum(thread::id num){
    threadNum = num;
}

thread::id TransactionalClient::GetThreadNum(){
    return threadNum;
}

void TransactionalClient::SetLatency(double latency, int tType){
    latencyVector[tType-1].push_back(latency);
}

double TransactionalClient::GetLatencySum(int tType){
    double sum=0.0;
    for(int i=0; i<GetLatencySize(tType); i++)
        sum += latencyVector[tType-1][i];
    return sum;
}

int TransactionalClient::GetLatencySize(int tType){
    return latencyVector[tType-1].size();
}