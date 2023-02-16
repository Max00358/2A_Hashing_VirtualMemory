#include <iostream>
#include "hashTable.hpp"
#include "chainImpl.cpp"
#include "doubleHashImpl.cpp"
using namespace std;

//The hash table stores the process objects. 
//A second array stores the actual memory pages. 
//The process object lookup is fast due to hashing, and that gives you the physical address.

//The memory array is an array of integers of size N. It is a single array and is NOT a vector
//g++ -o main hashMain.cpp chainImpl.cpp doubleHashImpl.cpp -std=c++11
int main(){
    string input;
    int N;          //memory size
    int P;          //page size
    int m;          //hash table size
    int numCount = 0;
    //int i = 0;

    int countInsertDouble = 0;
    int countInsertChain = 0;

    cin >> input;
    if(input == "OPEN"){//use double hashing
        cin >> input;
        cin >> N;
        cin >> P;
        m = N/P;
        
        doubleHash doubleHashTable(m);

        int doubleHashMemory[N];

        int delArr[m];
        for(int i = 0; i<m; i++){
            delArr[i] = i * P;   //store available phys_addr
        }

        cout << "success" << endl;

        while(cin>>input){
            if(input == "END"){
                break;
            }
            if(input == "INSERT"){
                unsigned int key;
                cin>>key;
                doubleHashTable.insert(key, doubleHashMemory, m, N, P, countInsertDouble, delArr);
            }
            if(input == "SEARCH"){
                unsigned int searchKey;
                cin>>searchKey;
                doubleHashTable.search(searchKey, m);
            }
            if(input == "WRITE"){
                unsigned int key;
                int vir_addr;
                int x;

                cin >> key;
                cin >> vir_addr;
                cin >> x;
                doubleHashTable.write(key, vir_addr, x, P, m, doubleHashMemory, N);
            }
            if(input == "READ"){
                unsigned int key;
                int vir_addr;

                cin >> key;
                cin >> vir_addr;
                doubleHashTable.read(key, m, vir_addr, P, doubleHashMemory, N);
            }
            if(input == "DELETE"){
                unsigned int key;
                cin >> key;
                doubleHashTable.del(key, m, doubleHashMemory, P, countInsertDouble, delArr);
            }
        }
    }
    else{//use chaining
        //Put "M" into input, and then take in other numbers
        cin >> input;
        cin >> N;
        cin >> P;
        m = N/P;
        chaining chainArray(m);

        int memory[N];//declare memory with size N
        cout << "success" << endl;

        while(cin>>input){
            if(input == "END"){
                break;
            }
            if(input == "INSERT"){
                unsigned int key;
                cin >> key;
                chainArray.chain_insert(key, chainArray.p_table, m, P, countInsertChain);
            }
            if(input == "SEARCH"){
                unsigned int findKey;
                cin>>findKey;
                chainArray.chain_search(findKey, chainArray.p_table, m);
            }
            if(input == "WRITE"){
                unsigned int writeKey;
                int vir_addr;
                int x;

                cin>>writeKey;
                cin>>vir_addr;
                cin>>x;
                chainArray.chain_write(writeKey, vir_addr, x, P, chainArray.p_table, m, memory, N);
            }
            if(input == "READ"){
                unsigned int readKey;
                int vir_addr;

                cin>>readKey;
                cin>>vir_addr;
                chainArray.chain_read(readKey, chainArray.p_table, m, vir_addr, P, memory);
            }
            if(input == "DELETE"){
                unsigned int delKey;
                cin>>delKey;
                chainArray.chain_del(delKey, chainArray.p_table, m, countInsertChain);
            }
            if(input == "PRINT"){
                int targetChain;
                cin>>targetChain;
                chainArray.chain_prt(chainArray.p_table, m, targetChain);
            }
        }
    }

}