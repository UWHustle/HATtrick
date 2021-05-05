#ifndef DATAGEN_H
#define DATAGEN_H

#include "DataSrc.h"

class DataGen{
	private:
		static ofstream lineorderStream;
		static ofstream historyStream;
		static ofstream customerStream;
		static ofstream supplierStream;
		static ofstream partStream;
		static ofstream dateStream;
	public:
		static void openDataFiles();
		static void closeDataFiles();
		static void partGen(int& pKey);
		static void customerGen(int cKey);
		static void supplierGen(int& sKey);
		static void lineorderGen(int& loKey);	
		static void dateGen();	
		static int dbGen();
};
#endif