
#include "Globals.h"
Globals::Globals() {
    for(int i=0; i<UserInput::getTranClients(); i++){
        containers.push_back(new LinkedList());
    }
}

int Globals::getLoOrderKey() {
    return ++loOrderKey;
}

/*int  Globals::IncrementTxnNum(){
    return ++txnNum;
}

int Globals::GetTxnNum(){
    return txnNum.load();
}*/

time_t Globals::GetEpochTime(){
    return epoch_time;
}
