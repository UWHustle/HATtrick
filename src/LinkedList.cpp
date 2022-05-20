#include "LinkedList.h"

LinkedList::LinkedList(){
    int dh = 500;
    long int cth = numeric_limits<long>::max();
    int dt = -1;
    long int ctt = -1;
    head = new Node(dh, cth);
    tail = new Node(dt, ctt);
    head->next = tail;
}

void LinkedList::Insert(int d, long ct){
    Node *new_node = new Node(d, ct);
    Node *right_node, *left_node;
    right_node = Search(ct, d, &left_node);
    new_node->next = right_node;
    //left_node->next = new_node;
    while(!(__sync_val_compare_and_swap(&(left_node->next), right_node, new_node)))
    {}  
}

Node* LinkedList::Search(long& ct, int& d, Node** left_node){
    Node *left_node_next = 0, *right_node;

    do{
        Node *t = head;
        Node * t_next = head->next; 
        while(t->commitTime>ct){
            (*left_node) = t;
            left_node_next = t_next;
            t = t_next;
            if(t == tail)
                break;
            t_next = t->next;
        };
        right_node = t;
        if(left_node_next == right_node)
            return right_node;
    }while(true);
} 

void LinkedList::PrintList(int& client){
    Node *t = head;
    Node *t_next = head->next; 
    do{
        cout << "client: " << client << " txnNum: " << t->data << ", " << "Commit Time: " << t->commitTime << endl;
        if(t == tail)
            break;
        t = t_next;
        t_next = t->next;
    }while(true);
}

long& LinkedList::GetFirstUnseenTxn(long& startTime, int& txnNum){
    Node *t = head;
    Node *t_next = head->next;
    int foundUnseen = 0;
    [[maybe_unused]] long ct;
    [[maybe_unused]] int tn = 0;
    [[maybe_unused]] int option = 0;
    do{

        if(t == tail || (t == head && t_next == tail)){  // reached tail
            if(foundUnseen == 1)
                break;
            else{
                firstUnseen = 0;
                break;
            }
        }
        else if(t == head){    // ignore head and continue to next node
            t = t_next;
            t_next = t->next;
        }
        else if(t->commitTime >= startTime){  // ignore txns that came after the current analytical query
            if(t->data == txnNum){            // not sure if this can happen, but fresh
                firstUnseen = 0;
		option = 1;
                ct = t->commitTime;
                tn = t->data;
                break;
            }
            else{                   // still havent found the txnNUm, continue to next node
                t = t_next;
                t_next = t->next;
            }
        }
        else if (t->commitTime < startTime){   
            if(t->data > txnNum){    // found possible candidate for the firstunseen, continue 
                firstUnseen = (startTime - t->commitTime);
	        //cout << "ST-CT: " << startTime << " ," << t->commitTime << ", txnNum: "  << t->data << "firstUnseen: "  << firstUnseen*1e-9<< endl;	 
		ct = t->commitTime;
                tn = t->data;
                t = t_next;
                t_next = t->next;
                foundUnseen = 1;
            }
            else if(t->data == txnNum){                   // found the txnNum, now break
                if(foundUnseen == 1 ){
	            //ct = t->commitTime;
		    //tn = t->data;
		    //cout << "UNSEEN TXN: " << tn << " CMT TIME: " << ct << " ST: " << startTime << endl;
		    option = 2;
                    break;
		}
                else{
                    firstUnseen = 0;
		    ct = t->commitTime;
		    tn = t->data;
		    option = 3;
                    break;
                }
            }
	    else if(t->data < txnNum){
		firstUnseen = 0;
                ct = t->commitTime;
                tn = t->data;
		option = 4;
		break;
	    }
        }
    }while(true);
//    cout << "Looking for txnNum: " << txnNum << ", found--> ST: " << startTime << ", commit_time: " << ct << "txn: " << tn << " diff: " << firstUnseen << " Case: " << option << endl;
    return firstUnseen;
}
