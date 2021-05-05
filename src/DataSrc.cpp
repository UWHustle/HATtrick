#include "DataSrc.h"

const vector<string> DataSrc::partName = {
    "almond", "antique", "aquamarine", "azure", "beige", "bisque", "black", "blanched",  "blue", "blush",
	"brown",   "burlywood",   "burnished",   "chartreuse",   "chiffon",  "chocolate", "coral", "cornflower",
	"cornsilk", "cream", "cyan", "dark", "deep", "dim", "dodger", "drab", "firebrick", "floral", "forest",
	"frosted", "gainsboro", "ghost", "goldenrod", "green", "grey", "honeydew", "hot", "indian", "ivory",
	"khaki", "lace", "lavender", "lawn", "lemon", "light", "lime", "linen", "magenta", "maroon", "medium",
	"metallic", "midnight", "mint", "misty", "moccasin", "navajo", "navy", "olive", "orange", "orchid",
	"pale", "papaya", "peach", "peru", "pink", "plum", "powder", "puff",  "purple", "red",  "rose",  "rosy",
	"royal", "saddle", "salmon", "sandy", "seashell", "sienna", "sky", "slate", "smoke", "snow", "spring",
	"steel", "tan", "thistle", "tomato", "turquoise", "violet", "wheat", "white", "yellow"};  //#92
const vector<string> DataSrc::partType = {         //S1(0-5), S2(6-10), S3(11-15)
                    "STANDARD", "SMALL", "MEDIUM", "LARGE", "ECONOMY", "PROMO", "ANODIZED", "BURNISHED",
                    "PLATED", "POLISHED", "BRUSHED", "TIN", "NICKEL", "BRASS", "STEEL", "COPPER"};
const vector<string> DataSrc::partContainer = {
                    "SM", "LG", "MED", "JUMBO", "WRAP", "CASE", "BOX", "BAG", "JAR",
                    "PKG", "PACK", "CAN", "DRUM"}; //S1(0-4), S2(5-12)
const vector<string> DataSrc::ordPriority = {
                    "1-URGENT", "2-HIGH", "3-MEDIUM", "4-NOT SPECIFIED", "5-LOW"}; //#5
const vector<string> DataSrc::shipMode = {
                    "REG AIR", "AIR", "RAIL", "SHIP", "TRUCK", "MAIL", "FOB"};  //#7
const vector<string> DataSrc::nation = {
                    "ALGERIA", "ARGENTINA", "BRAZIL", "CANADA", "EGYPT", "ETHIOPIA",
					"FRANCE", "GERMANY", "INDIA", "INDONESIA", "IRAN", "IRAQ", "JAPAN",
					"JORDAN", "KENYA", "MOROCCO", "MOZAMBIQUE", "PERU", "CHINA", "ROMANIA",
					"SAUDI ARABIA", "VIETNAM", "RUSSIA", "UNITED KINGDOM", "UNITED STATES"}; //#25
const vector<string> DataSrc::region = {
                    "AFRICA", "AMERICA", "AMERICA", "AMERICA", "AFRICA", "AFRICA",
					"EUROPE", "EUROPE", "ASIA", "ASIA", "MIDDLE EAST", "MIDDLE EAST", "ASIA",
					"AFRICA", "AFRICA", "AFRICA", "AFRICA", "AMERICA", "ASIA", "EUROPE",
					"MIDDLE EAST", "ASIA", "EUROPE", "EUROPE", "AMERICA"};
const vector<string> DataSrc::mktsegment = {
                    "AUTOMOBILE", "BUILDING", "FURNITURE", "MACHINERY", "HOUSEHOLD"}; //#5
const vector<string> DataSrc::year = {
                    "1992", "1993", "1994", "1995", "1996", "1997", "1998"};
const Month DataSrc::months[]={
                    {"January", 31}, {"February", 28}, {"March", 31}, {"April", 30},
                    {"May", 31}, {"June", 30}, {"July", 31}, {"August", 31},
                    {"September", 30}, {"October", 31}, {"November", 30}, {"December", 31}};
const vector<string> DataSrc::day = {
                    "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
const Holiday DataSrc::holidays[] = {
                {"Christmas", {"2512"}}, {"Thanksgiving", {"26101992", "25101993", "24101994", "23101995", "28101996",\
                "27101997", "26101998"}}, {"Easter", {"19041992", "11041993", "03041994", "16041995", "07041996", "30031997", "12041998"}}, \
                {"Valentine's Day", {"1402"}}, {"Fourth of July", {"0407"}}};

mt19937 DataSrc::mt(123456789);
vector<double> DataSrc::price;
vector<string> DataSrc::c_phone;
vector<string> DataSrc::s_phone;
vector<string> dateString; //(2557);
vector<double> ytd(UserInput::getSuppSize(), 0.0);
vector<int> paymentcnt(UserInput::getCustSize(), 0);

string DataSrc::getShipMode(int idx){
    return DataSrc::shipMode[idx];
}

string DataSrc::getOrdPriority(int idx){
    return DataSrc::ordPriority[idx];
}

string DataSrc::getMonthName(int idx){
    return DataSrc::months[idx].name;
}

int DataSrc::getMonthDay(int idx){
    return DataSrc::months[idx].days;
}

string DataSrc::getYear(int idx){
    return DataSrc::year[idx];
}

int DataSrc::uniformIntDist(int minValue, int maxValue){
    /* Discrete uniform distribution. */
	uniform_int_distribution<int> uniDisc(minValue, maxValue);
	return uniDisc(DataSrc::mt);
}

float DataSrc::uniformRealDist(float minValue, float maxValue){
    /* Continuous uniform distribution. */
	uniform_real_distribution<float> uniCont(minValue, maxValue);
	return uniCont(DataSrc::mt);
}

string DataSrc::getPartName(ofstream& stream){
    /* Generation of P_NAME and P_COLOR for the current entry. */
	string c1 = DataSrc::partName[DataSrc::uniformIntDist(0, 91)];
	string c2 = DataSrc::partName[DataSrc::uniformIntDist(0, 91)];
	stream <<  c1 + " " + c2 + UserInput::getDelimiter();
	if(DataSrc::uniformIntDist(1, 2) == 1)
	    return c1;
	else
	    return c2;
}

void DataSrc::getPartMFCR(ofstream& stream){
    /* Generation of P_MFCR, P_CATEGORY and P_BRAND1 for the current entry. */
	string mfcr = "MFGR#" + to_string(DataSrc::uniformIntDist(1, 5));
	stream <<  mfcr + UserInput::getDelimiter();
	DataSrc::getPartCategory(mfcr, stream);
}

void DataSrc::getPartCategory(string const& s, ofstream& stream){
	string category = s + to_string(DataSrc::uniformIntDist(1, 5));
	stream << category + UserInput::getDelimiter();
	DataSrc::getPartBrand1(category, stream);
}

void DataSrc::getPartBrand1(string const& s, ofstream& stream){
	stream << s + to_string(DataSrc::uniformIntDist(1, 40)) + UserInput::getDelimiter();
}

void DataSrc::getPartType(ofstream& stream){
    /* Generation of P_TYPE for the current entry. */
    stream << partType[DataSrc::uniformIntDist(0, 5)] + " " + partType[DataSrc::uniformIntDist(6, 10)] + " " + \
        partType[DataSrc::uniformIntDist(11, 15)] + UserInput::getDelimiter();
}

void DataSrc::getPartSize(ofstream& stream){
    /* Generation of P_SIZE for the current entry. */
	stream << to_string(DataSrc::uniformIntDist(1, 50)) + UserInput::getDelimiter();
}

void DataSrc::getPartContainer(ofstream& stream){
    /* Generation of P_CONTAINER for the current entry. */
    stream << partContainer[DataSrc::uniformIntDist(0, 4)] + " " + partContainer[DataSrc::uniformIntDist(5, 12)]  + \
        UserInput::getDelimiter();
}

void DataSrc::getPartPrice(ofstream& stream, int& key){
	double p = (90000+ fmod((double)key/10,20001)+100*fmod(key,1000))/100;
    DataSrc::price.push_back(p);
	stream << p;
}

float DataSrc::getPrice(int& partkey){
	return DataSrc::price[partkey-1];
}

string DataSrc::getCPhone(int key){
    return DataSrc::c_phone[key-1];
}

string DataSrc::getSPhone(int key){
    return DataSrc::s_phone[key-1];
}

int DataSrc::getOrderDate(ofstream& stream){
    /* Generate LO_ORDERDATE for the current entry. */
    int key = DataSrc::uniformIntDist(0, 2000);
    stream << dateString[key] + UserInput::getDelimiter();
    return key;
}

void DataSrc::getCommitDate(ofstream& stream, int& key){
    /* Generate LO_COMMITDATE for the current entry. */
    int nkey = key + DataSrc::uniformIntDist(30, 90);
    stream << dateString[nkey] + UserInput::getDelimiter();
}

void DataSrc::getOrdPriority(ofstream& stream){
    /* Generate LO_ORDPRIORITY for the current entry. */
	stream << DataSrc::ordPriority[DataSrc::uniformIntDist(0, 4)] + UserInput::getDelimiter();
}

void DataSrc::getShipMode(ofstream& stream){
    /* Generate LO_SHIPPRIORITY for the current entry. */
	stream << DataSrc::shipMode[DataSrc::uniformIntDist(0, 6)] ;
}

void DataSrc::getName(ofstream& stream, int& custkey, int choice){
    /* Generate C_/S_NAME for the cusrrent entry. */
	ostringstream key;
    key << setw(9) << setfill('0') << custkey;
	string type = "";
	if(choice == 0)
	    type = "Customer#";
	else if(choice == 1)
	    type = "Supplier#";
	stream << type + key.str() + UserInput::getDelimiter();
}

void DataSrc::getAddress(ofstream& stream){
    /* Generate the C_ADDRESS or S_ADDRESS for the current entry. */
	int length = DataSrc::uniformIntDist(10, 25);
	string address = "";
	for(int i=0; i<length; i++)
		address += static_cast<char>(DataSrc::uniformIntDist(48, 122));
	stream << address + UserInput::getDelimiter();
}

void DataSrc::getNation(ofstream& stream, int choice){
    /* Generate C_/S_CITY, C_/S_NATION, C_/S_REGION, C_/S_PHONE for the current entry. */
	int index = DataSrc::uniformIntDist(0, 24);
	string nation = DataSrc::nation[index];
	string city = DataSrc::nation[index];
	int length = nation.length();
	string nationOrig = nation;
	if( length >=9)
	    city = city.substr(0, 9);
	else
	    city = city.append(9-length, ' ');
	int countryCode =  index + 10;
	string tel = to_string(countryCode);
	tel += '-' + to_string(DataSrc::uniformIntDist(100, 999)) + '-' + \
	to_string(DataSrc::uniformIntDist(100, 999)) + '-' + to_string(DataSrc::uniformIntDist(1000, 9999));
	if (choice == 0)
	    DataSrc::c_phone.push_back(tel);
	else if(choice == 1)
	    DataSrc::s_phone.push_back(tel);
	stream << city + to_string(DataSrc::uniformIntDist(0, 9)) + UserInput::getDelimiter() + nation + \
	    UserInput::getDelimiter() + DataSrc::region[index] + UserInput::getDelimiter() + tel + UserInput::getDelimiter();
}

void DataSrc::getCustMktsegment(ofstream& stream){
    /* Generate C_MKTSEGMENT for the current entry. */
	stream << DataSrc::mktsegment[DataSrc::uniformIntDist(0, 4)] << UserInput::getDelimiter();
}

void DataSrc::getDate(ofstream& stream){
    /* Generate the Date table. */
	int weekDay = 2;
	int weekcnt = 1;
	int daycnt = 1;
	bool holidayF = 0;
	int holidayscnt = sizeof(holidays) / sizeof(holidays[0]);
	int cnt = 0;
	for(int i=0; i<=6; i++){
		for(int j=0; j<12; j++){
			int md = 0;
			if( (DataSrc::year[i].compare("1992") == 0 ||  DataSrc::year[i].compare("1996") == 0) \
			    && DataSrc::months[j].name.compare("February") == 0)
				md = 29;
			else
			    md = DataSrc::months[j].days;
			for(int d=1; d<=md; d++){
				ostringstream dos;
    			dos << setw(2) << setfill('0') << d;
				ostringstream month;
    			month << setw(2) << setfill('0') << (j+1);
				stream << DataSrc::year[i] + month.str() + dos.str() + UserInput::getDelimiter();   // D_DATEKEY
				dateString.push_back(DataSrc::year[i] + month.str() + dos.str());
				stream << DataSrc::months[j].name + " " + to_string(d) + ", " + \
				    DataSrc::year[i] + UserInput::getDelimiter();    // D_DATE
			    stream << DataSrc::day[weekDay] + UserInput::getDelimiter();                        // D_DAYOFWEEK
                stream << DataSrc::months[j].name + UserInput::getDelimiter();                      // D_MONTH
				stream << DataSrc::year[i] + UserInput::getDelimiter();                             // D_YEAR
				stream << DataSrc::year[i] + month.str()  + UserInput::getDelimiter();              // D_YEARMONTHNUM
				stream << DataSrc::months[j].name.substr(0, 3) + DataSrc::year[i] + \
				    UserInput::getDelimiter();                                                      // D_YEARMONTH
				stream << to_string(weekDay+1) + UserInput::getDelimiter();                    // D_DAYNUMINWEEK
				stream << to_string(d) + UserInput::getDelimiter();                            // D_DAYNUMINMONTH
				stream << to_string(daycnt) + UserInput::getDelimiter();                       // D_DAYNUMINYEAR
				stream << to_string(j+1) + UserInput::getDelimiter();                          // D_MONTHNUMINYEAR
				stream << to_string(weekcnt) + UserInput::getDelimiter();                      // D_WEEKNUMINYEAR
				for(int k=0; k<holidayscnt; k++){
				    if( (strcmp(holidays[k].name.c_str(), "Thanksgiving") == 0)  or (strcmp(holidays[k].name.c_str(), "Easter") == 0) ) {
                        for (int l = 0; l < static_cast<int>(DataSrc::holidays[k].date.size()); l++) {
                            if (strcmp(DataSrc::holidays[k].date[l].c_str(), (dos.str() + month.str() + \
                                DataSrc::year[i]).c_str()) == 0) {
                                holidayF = 1;
                                stream << DataSrc::holidays[k].name + UserInput::getDelimiter();    // D_SELLINGSEASON
                            }
                        }
                    }
	                else
                        if(strcmp(DataSrc::holidays[k].date[0].c_str(), (dos.str()+month.str()).c_str()) == 0){
                            holidayF = 1;
                            stream << DataSrc::holidays[k].name + UserInput::getDelimiter();    // D_SELLINGSEASON
                        }
				}
				if(holidayF == 0) stream << string("None") + UserInput::getDelimiter();    // D_SELLINGSEASON
				if(weekDay == 6){
					weekDay = 0;
					weekcnt++;
					stream << to_string(1) + UserInput::getDelimiter();                    // D_LASTDAYINWEEKFL
				}
				else{
					weekDay++;
					stream << to_string(0) + UserInput::getDelimiter();                    // D_LASTDAYINWEEKFL
				}
				if(daycnt == DataSrc::months[j].days)
				    stream << to_string(1) + UserInput::getDelimiter();                    // D_LASTDAYINMONTHFL
				else
				    stream << to_string(0) + UserInput::getDelimiter();                    // D_LASTDAYINMONTHFL
				if(holidayF == 1)
				    stream << to_string(1) + UserInput::getDelimiter();                    // D_HOLIDAYFL
				else
				    stream << to_string(0) + UserInput::getDelimiter();                    // D_HOLIDAYFL
				if(weekDay != 5 && weekDay != 6)
				    stream << to_string(1);                                                // D_WEEKDAYFL
				else
				    stream << to_string(0);                                                // D_WEEKDAYFL
				daycnt++;
				holidayF = 0;
				stream << endl;
				cnt ++;
			}
		}
        weekcnt = 0;
	}

}

string DataSrc::getSelectedNation(int choice){
    return DataSrc::nation[choice];
}