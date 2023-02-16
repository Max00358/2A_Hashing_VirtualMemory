#ifndef _HASHTABLE_HPP_
#define _HASHTABLE_HPP_
#include <iostream>
#include <cmath>
using namespace std;

class Node{
public:
    Node(){
        val = 0;
    }
    Node(int newVal){
        val = newVal;
    }
    int val;
    int phys_addr;
    Node* next;
};
class doubleHashNode{
public:
    doubleHashNode(){
        val = 0;
        delFlag = 0;
    }
    doubleHashNode(int newVal){
        val = newVal;
        delFlag = 0;
    }
    int val;
    int delFlag;    //flag = 0 initial condition; flag = 1 after insert; flag = -1 after deletion
    int phys_addr;
    doubleHashNode* next;

    int get_flag(){
        return delFlag;
    }
    void set_flag(int newFlag){
        this->delFlag = newFlag;
        return;
    }
    int get_val(){
        return this->val;
    }
    void set_val(int newVal){
        this->val = newVal;
        //cout<< "set_val: " + to_string(val) << endl;
        return;
    }
    int get_phys_addr(){
        return phys_addr;
    }
    void set_phys_addr(int newNum){
        this->phys_addr = newNum;
        return;
    }
};
/*
class List{
public:
    List(){
        doubleHashNode* head = NULL;
        head->next = NULL;
    }
    ~List();
    doubleHashNode* get_head(){
        return head;
    }
    void set_head(doubleHashNode* newVal){
        head = newVal;
        return;
    }
private:
    doubleHashNode* head;
};
*/
class process{
public:
    int hashFunctionOne(int key){
        //cout << size <<endl;
        return (unsigned) (key % get_size());
    }
    int hashFunctionTwo(int key){
        int result = ( (unsigned)floor(key / get_size()) % get_size() ) % get_size();
        if(result % 2 == 0){//must be odd number, if even, add 1
            return result + 1;
        }
        return result;
    }
    ~process();

    //bool isFull(doubleHashNode array[], int size);
    //bool isMemFull(int memory[], int memSize, int totPage);
    int get_size(){
        return size;
    }
    void set_size(int newSize){
        size = newSize;
        return;
    }
protected:
    int size;
};

class chaining : public process{
public:
    chaining(int newSize){
        size = newSize;
        p_table = new Node*[newSize];
        for(int i = 0; i<newSize; i++){
            p_table[i] = new Node();
            p_table[i]->next = NULL;
        }
    }
    ~chaining();

    Node** p_table;
    Node* chain_alrExist(Node** table, int tableSize, unsigned int key);

    void chain_insert(unsigned int key, Node** table, int tableSize, int pageSize, int& countInsertChain);
    void chain_search(unsigned int key, Node** table, int tableSize);
    void chain_write(unsigned int key, int vir_addr, int x, int pageSize, Node** table, int tableSize, int memory[], int memSize);
    void chain_read(unsigned int key, Node** table, int tableSize, int vir_addr, int pageSize, int memory[]);
    void chain_del(unsigned int key, Node** table, int tableSize, int& countInsertChain);
    void chain_prt(Node** table, int tableSize, int chainNum);

private:
    unsigned int key;     //PID
    //Node** p_table;
};

class doubleHash : public process{
public:
    doubleHash(int newSize){
        this->set_size(newSize);
        table = new doubleHashNode[newSize];
        
        for(int i = 0; i<newSize; i++){
            table[i].set_phys_addr(0);
            table[i].set_flag(0);
            table[i].set_val(0);
        }
    }
    ~doubleHash();

    doubleHashNode *table;
    int searchIndex(unsigned int key, int tableSize);
    bool alrExist(int size, unsigned int key);

    void insert(unsigned int key, int memory[], int tableSize, int memorySize, int pageSize, int& countInsert, int delArr[]);
    void search(unsigned int key, int tableSize);
    void write(unsigned int key, int vir_addr, int x, int pageSize, int tableSize, int memory[], int memSize);
    void read(unsigned int key, int tableSize, int vir_addr, int pageSize, int memory[], int memSize);
    void del(unsigned int key, int tableSize, int memory[], int pageSize, int &countInsert, int delArr[]);

private:
    unsigned int key;     //PID
    //int phys_addr;
};
#endif