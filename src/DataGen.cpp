#include "DataGen.h"

ofstream DataGen::lineorderStream;
ofstream DataGen::historyStream;
ofstream DataGen::customerStream;
ofstream DataGen::supplierStream;
ofstream DataGen::partStream;
ofstream DataGen::dateStream;

void DataGen::openDataFiles(){
	lineorderStream.open(UserInput::getDataPath()+"/lineorder.bin", ofstream::out | ofstream::app | ofstream::binary);
	historyStream.open(UserInput::getDataPath()+"/history.bin", ofstream::out | ofstream::app | ofstream::binary);
	customerStream.open(UserInput::getDataPath()+"/customer.bin", ofstream::out | ofstream::app | ofstream::binary);
	supplierStream.open(UserInput::getDataPath()+"/supplier.bin", ofstream::out | ofstream::app | ofstream::binary);
	partStream.open(UserInput::getDataPath()+"/part.bin", ofstream::out | ofstream::app | ofstream::binary);
	dateStream.open(UserInput::getDataPath()+"/date.bin", ofstream::out | ofstream::app | ofstream::binary);
}

void DataGen::partGen(int& pKey){
	partStream << pKey << UserInput::getDelimiter();        		    // value for attribute P_PARTKEY
	string color = DataSrc::getPartName(partStream);  		    // values for attributes P_NAME, P_COLOR
	DataSrc::getPartMFCR(partStream);		    // values for attributes P_MFCR, P_CATEGORY, P_BRAND1
	partStream << color << UserInput::getDelimiter();   // value for attribute P_COLOR
	DataSrc::getPartType(partStream);  		    // value for attribute P_TYPE
	DataSrc::getPartSize(partStream);  		    // value for attribute P_SIZE
	DataSrc::getPartContainer(partStream);  	// value for attribute P_CONTAINER
	DataSrc::getPartPrice(partStream, pKey);		    // value for attribute P_PRICE
	partStream  << endl;
}

void DataGen::lineorderGen(int& loKey){
	int lines = DataSrc::uniformIntDist(1, 7);                                      // number of suborders within the order
	int custkey = DataSrc::uniformIntDist(1, UserInput::getCustSize());             // choose customer randomly
	paymentcnt[custkey-1] += 1;
	historyStream << loKey << UserInput::getDelimiter();							                        // value for attribute H_ORDERKEY
	historyStream << custkey << UserInput::getDelimiter();						                        // value for attribute H_CUSTKEY
	double amount = 0.0;
    for(int i=1; i<=lines; i++){
		lineorderStream << loKey << UserInput::getDelimiter();					                        // value for attribute LO_ORDERKEY
		lineorderStream << i << UserInput::getDelimiter();						                        // value for attribute LO_LINENUMBER
		lineorderStream << custkey << UserInput::getDelimiter();					                        // value for attribute LO_CUSTKEY
		int partkey = DataSrc::uniformIntDist(1, UserInput::getPartSize());         // choose part & supplier randomly
		lineorderStream << partkey << UserInput::getDelimiter();					                        // value for attribute LO_PARTKEY
		int suppkey = DataSrc::uniformIntDist(1, UserInput::getSuppSize());
		lineorderStream << suppkey << UserInput::getDelimiter();					                        // value for attribute LO_SUPPKEY
		int orderDate = DataSrc::getOrderDate(lineorderStream); 				    // value for attribute LO_ORDERDATE
		DataSrc::getOrdPriority(lineorderStream);				                    // value for attribute LO_ORDPRIORITY
		lineorderStream << DataSrc::uniformIntDist(0, 1) << UserInput::getDelimiter();		            // value for attribute LO_SHIPPRIORITY
		int quantity = DataSrc::uniformIntDist(1, 50);
		lineorderStream << quantity << UserInput::getDelimiter();					                        // value for attribute LO_QUANTITY
		double extendedprice = quantity*DataSrc::getPrice(partkey);
		lineorderStream << extendedprice  << UserInput::getDelimiter();				                    // value for attribute LO_EXTENDEDPRICE
		int discount = DataSrc::uniformIntDist(0, 10);
		lineorderStream << discount << UserInput::getDelimiter();					                        // value for attribute LO_DISCOUNT
		double revenue = (extendedprice*(100-discount))/100;
		amount += revenue;
		lineorderStream << revenue << UserInput::getDelimiter();					                        // value for attribute LO_REVENUE
		lineorderStream << DataSrc::uniformRealDist(1.00, 1000.00) << UserInput::getDelimiter();	        // value for attribute LO_SUPPLYCOST
		lineorderStream << DataSrc::uniformIntDist(0, 8) << UserInput::getDelimiter();		            // value for attribute LO_TAX
		DataSrc::getCommitDate(lineorderStream, orderDate); 				        // value for attribute LO_COMMITDATE
		DataSrc::getShipMode(lineorderStream);					                    // value for attribute LO_SHIPMODE
		//lineorderStream <<                                                        // value for attribute TXNTIMESTMP
		lineorderStream << endl;
		ytd[suppkey-1] = ytd[suppkey-1] + revenue;

	}
	historyStream << amount;                                                        // value for attribute H_AMOUNT
	historyStream << endl;
}

void DataGen::customerGen(int cKey){
	customerStream << cKey << UserInput::getDelimiter();        			// value for attribute C_CUSTKEY
	DataSrc::getName(customerStream, cKey, 0);  	// value for attribute C_NAME
	DataSrc::getAddress(customerStream);		    // value for attribute C_ADDRESS
	DataSrc::getNation(customerStream, 0);			    // values for attributes C_CITY, C_NATION, C_REGION, C_PHONE
	DataSrc::getCustMktsegment(customerStream); 	// value for attribute C_MKTSEGMENT
	customerStream << paymentcnt[cKey-1];           // value for attribute C_PAYMENTCNT
	customerStream << endl;
}

void DataGen::supplierGen(int& sKey){
	supplierStream << sKey << UserInput::getDelimiter();				    // value for attribute S_SUPPKEY
	DataSrc::getName(supplierStream, sKey, 1);  	// value for attribute S_NAME
	DataSrc::getAddress(supplierStream);		    // value for attribute S_ADDRESS
	DataSrc::getNation(supplierStream, 1);			    // values for attributes S_CITY, S_NATION, S_REGION, S_PHONE
	supplierStream << to_string(ytd[sKey-1]);          	    // value for attribute S_YTD
	supplierStream << endl;
}

void DataGen::dateGen(){
	DataSrc::getDate(dateStream);
}

void DataGen::closeDataFiles(){
	lineorderStream.close();
	historyStream.close();
	customerStream.close();
	supplierStream.close();
	partStream.close();
	dateStream.close();
}

int DataGen::dbGen(){
    openDataFiles();                                                            // open output files
    for(int pKey=1; pKey<=UserInput::getPartSize(); pKey++)                     // PART generation
		partGen(pKey);
	dateGen();                                                                  // DATE generation
	for(int loKey=1; loKey<=UserInput::getLoSize(); loKey++)                   // LINEORDER & HISTORY generation
		lineorderGen(loKey);
	for(int cKey=1; cKey<=UserInput::getCustSize(); cKey++)                    // CUSTOMER generation
		customerGen(cKey);
	for(int sKey=1; sKey<=UserInput::getSuppSize(); sKey++)                    // SUPPLIER generation
		supplierGen(sKey);
    closeDataFiles();                                                           // close output files
    return 0;
}