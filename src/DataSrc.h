#ifndef DATASRC_H
#define DATASRC_H

#include "UserInput.h"
#include <vector>
#include <random>
#include <cstring>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>

using namespace std;

extern vector<int> paymentcnt;
extern vector<double> ytd;
extern vector<string> dateString;

struct Month{
	string name;
	int days;
};
struct Holiday{
	string name;
	vector<string> date;
};

class DataSrc{
	private:
		static const vector<string> partName;
		static const vector<string> partContainer;
		static const vector<string> partType;
		static const vector<string> ordPriority;
		static const vector<string> shipMode;
  		static mt19937 mt;
		static vector<double> price;
	    static vector<string> c_phone;
        static vector<string> s_phone;
		static const vector<string> nation;
		static const vector<string> region;
		static const vector<string> mktsegment;
		static const Month months[];
		static const vector<string> year;
		static const vector<string> day;
		static const Holiday holidays[];
	public:
		static int uniformIntDist(int minValue, int maxValue);
		static float uniformRealDist(float minValue, float maxValue);
		static string getPartName(ofstream& stream);
		static void getPartMFCR(ofstream& stream);
		static void getPartCategory(string const& s, ofstream& stream);
		static void getPartBrand1(string const& s, ofstream& stream);
		static void getPartType(ofstream& stream);
		static void getPartSize(ofstream& stream);
		static void getPartContainer(ofstream& stream);
		static void getPartPrice(ofstream& stream, int& key);
		static int getOrderDate(ofstream& stream);
		static void getCommitDate(ofstream& stream, int& key);
		static float getPrice(int& partkey);
		static string getCPhone(int key);
		static string getSPhone(int key);
		static void getOrdPriority(ofstream& stream);
		static void getShipMode(ofstream& stream);
		static void getName(ofstream& stream, int& custkey, int choice);
		static void getAddress(ofstream& stream);
		static void getNation(ofstream& stream, int choice);
		static void getCustMktsegment(ofstream& stream);
		static void getDate(ofstream& stream);
		static string getShipMode(int idx);
		static string getOrdPriority(int idx);
		static string getMonthName(int idx);
        static int getMonthDay(int idx);
        static string getYear(int idx);
        static string getSelectedNation(int choice);
};

#endif