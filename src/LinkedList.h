#ifndef HATTRICKBENCH_LINKEDLIST_H
#define HATTRICKBENCH_LINKEDLIST_H
#include <atomic>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <thread>
#include <unistd.h>
#include <limits>
using namespace std;


class Node{
	public:		
	    int data;
		long commitTime;
		Node* next;

		Node(int& d, long& ct){
			data = d;
			commitTime = ct;
		}
}; 

class LinkedList{
	public:
		Node *head;
		Node *tail;
		long firstUnseen;
		LinkedList();
		void Insert(int d, long ct);
		Node* Search(long& ct, int& d, Node** left_node);
		void PrintList(int& client);  // only for debuging 
		long& GetFirstUnseenTxn(long& startTime, int& txnNum);
};
#endif 
