#include <iostream>
#include <cmath>
#include "hashTable.hpp"
using namespace std;

doubleHash::~doubleHash(){
	delete[] table;
    table = NULL;
}
bool doubleHash::alrExist(int size, unsigned int key){
    for(int i = 0; i<size; i++){
        if(table[i].get_val() == key){
            return true;
        }
    }
    return false;
}
void doubleHash::insert(unsigned int key, int memory[], int tableSize, int memorySize, int pageSize, int& countInsert, int delArr[]){
    int finLoc = ( hashFunctionOne(key) + countInsert * hashFunctionTwo(key) ) % tableSize;
    int store[pageSize];
    for(int i = 0; i<pageSize; i++){
        store[i] = i * pageSize;
    }

    if(countInsert >= tableSize || searchIndex(key, tableSize) != -1){  //alrExist(tableSize, key) == true node does exist
        cout<< "failure" <<endl;                                       //searchIndex(key, tableSize) != -1
        return;
    }
    else{
        //Do double hashing here
        if(table[hashFunctionOne(key)].get_val() == 0 && countInsert < tableSize){
            table[hashFunctionOne(key)].set_val(key);
            for(int i = 0; i<tableSize; i++){
                if(delArr[i] != -1){
                    table[hashFunctionOne(key)].set_phys_addr(delArr[i]);
                    delArr[i] = -1;
                    break;
                }
            }
            //table[hashFunctionOne(key)].set_phys_addr(countInsert * pageSize);
            table[hashFunctionOne(key)].set_flag(1);

            countInsert++;
            cout<< "success" <<endl;
            return;
        }
        /*
        else if(table[( hashFunctionOne(key) + countInsert * hashFunctionTwo(key) ) % tableSize].get_val() == 0 && countInsert < tableSize){
            table[( hashFunctionOne(key) + countInsert * hashFunctionTwo(key) ) % tableSize].set_val(key);
            table[( hashFunctionOne(key) + countInsert * hashFunctionTwo(key) ) % tableSize].set_phys_addr(countInsert * pageSize);
            table[( hashFunctionOne(key) + countInsert * hashFunctionTwo(key) ) % tableSize].set_flag(1);

            countInsert++;
            cout << "5" << endl;
            cout<< "success" <<endl;
            return;
        }
        */
        else{
            while(countInsert > 0 && countInsert < tableSize){
                if(table[( hashFunctionOne(key) + countInsert * hashFunctionTwo(key) ) % tableSize].get_val() == 0){
                    table[( hashFunctionOne(key) + countInsert * hashFunctionTwo(key) ) % tableSize].set_val(key);
                    
                    for(int i = 0; i<tableSize; i++){
                        if(delArr[i] != -1){
                            table[( hashFunctionOne(key) + countInsert * hashFunctionTwo(key) ) % tableSize].set_phys_addr(delArr[i]);
                            delArr[i] = -1;
                            break;
                        }
                    }
                    //table[( hashFunctionOne(key) + countInsert * hashFunctionTwo(key) ) % tableSize].set_phys_addr(countInsert * pageSize);
                    table[( hashFunctionOne(key) + countInsert * hashFunctionTwo(key) ) % tableSize].set_flag(1);

                    countInsert++;
                    cout<< "success" <<endl;
                    return;
                }
            }
        }
        cout<< "failure" << endl;
        return;
    }
}
void doubleHash::search(unsigned int key, int tableSize){
    /*
    //Using linear search
    for(int i = 0; i<tableSize; i++){
        if(table[i].get_val() == key){
           cout << "found " + to_string(key) + " in " + to_string(i) << endl; 
           return;
        }
    }
    cout << "not found" << endl;
    return;
    */
    int count = 0;
    if(table[hashFunctionOne(key)].get_val() == key){
        cout << "found " + to_string(key) + " in " + to_string(hashFunctionOne(key)) << endl; 
        return;
    }
    else{
        while(count < tableSize){
            count++;
            if(table[( hashFunctionOne(key) + count * hashFunctionTwo(key) ) % tableSize].get_flag() == -1){
                count++;
            }
            else{
                if(table[( hashFunctionOne(key) + count * hashFunctionTwo(key) ) % tableSize].get_val() == key 
                && table[( hashFunctionOne(key) + count * hashFunctionTwo(key) ) % tableSize].get_flag() != -1){
                    cout << "found " + to_string(key) + " in " + to_string(( hashFunctionOne(key) + count * hashFunctionTwo(key) ) % tableSize) << endl;
                    return;
                }
            }
        }
    }
    cout << "not found" << endl;
    return;
    /*
    if(searchIndex(key, tableSize) == -1){
        cout << "not found" << endl;
        return;
    }
    else{
        cout << "found " + to_string(key) + " in " + to_string(searchIndex(key, tableSize)) << endl; 
        return;
    }
    */
}
int doubleHash::searchIndex(unsigned int key, int tableSize){
    int count = 0;

    if(table[hashFunctionOne(key)].get_val() == key){ 
        //cout << "without collision: " + to_string(hashFunctionOne(key))<< endl;
        return hashFunctionOne(key);
    }
    else{
        while(count < tableSize){
            count++;
            if(table[( hashFunctionOne(key) + count * hashFunctionTwo(key) ) % tableSize].get_val() == key){
                return ( hashFunctionOne(key) + count * hashFunctionTwo(key) ) % tableSize;
            }
            else if(table[( hashFunctionOne(key) + count * hashFunctionTwo(key) ) % tableSize].get_flag() != 1){
                //return -1;
            } else{
                //cout << "Collision: " + to_string(( hashFunctionOne(key) + count * hashFunctionTwo(key) ) % tableSize)<< endl;
            }
        }
    }
    return -1;
}
void doubleHash::write(unsigned int key, int vir_addr, int x, int pageSize, int tableSize, int memory[], int memSize){
    if(searchIndex(key, tableSize) != -1 && table[searchIndex(key, tableSize)].get_val() != 0    // alrExist(tableSize, key) == true node does exist
    && table[searchIndex(key, tableSize)].get_phys_addr() + vir_addr < memSize                  // searchIndex(key, tableSize) != -1
    && vir_addr < pageSize){        
        memory[ table[searchIndex(key, tableSize)].get_phys_addr() + vir_addr ] = x;

        //cout << "x: " + to_string(x) << endl;
        //cout << to_string(key)+ " memory written: " + to_string(memory[ table[searchIndex(key, tableSize)].get_phys_addr() + vir_addr ]) << endl;
        cout << "success" << endl;
    }
    else{
        cout << "failure" << endl;
    }
    return;
}
void doubleHash::read(unsigned int key, int tableSize, int vir_addr, int pageSize, int memory[], int memSize){
    if(searchIndex(key, tableSize) == -1 ||                 //alrExist(tableSize, key) == false key doesn't exist 
    table[searchIndex(key, tableSize)].get_val() <= 0 ||    //searchIndex(key, tableSize) == -1
    table[searchIndex(key, tableSize)].get_phys_addr() + vir_addr >= memSize ||
    vir_addr >= pageSize){
        cout << "failure" << endl;
    }
    else{
        int pos = table[searchIndex(key, tableSize)].get_phys_addr() + vir_addr;
        //cout << to_string(key) + " " + to_string(pos) << endl;
        cout << to_string(vir_addr) + " " + to_string(memory[pos]) << endl;
    }
    return;
}
void doubleHash::del(unsigned int key, int tableSize, int memory[], int pageSize, int& countInsert, int delArr[]){
    if(searchIndex(key, tableSize) != -1){       //alrExist(tableSize, key) == true node does exist, delete
        int loc = searchIndex(key, tableSize);  //searchIndex(key, tableSize) != -1

        for(int i = 0; i < tableSize; i++){
            if(delArr[i] == -1){
                delArr[i] = table[loc].get_phys_addr();
                break;
            }
        }
        table[loc].set_val(0);
        table[loc].set_phys_addr(-1);
        table[loc].set_flag(-1); //-1 is condition flag for deletion
        countInsert--;

        cout << "success" << endl;
    }
    else{
        if(searchIndex(key, tableSize) == -1 || table[searchIndex(key, tableSize)].get_flag() != 1){
            cout << "failure" << endl;
        }
    }
    return;
}