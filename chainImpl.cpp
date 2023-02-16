#include <iostream>
#include <cmath>
#include "hashTable.hpp"
using namespace std;

process::~process(){

}
chaining::~chaining(){//causes memory leaks?
    Node *p_temp = *p_table;
    Node *p_head = *p_table;

	for(int i = 0; i < get_size(); i++){
        p_temp = p_table[i];
        p_head = p_table[i];

        while(p_head != NULL){
		    p_temp = p_temp->next;
		    delete p_head;
		    p_head = p_temp;
        }
	}
    delete p_table;
}
Node* chaining::chain_alrExist(Node** table, int tableSize, unsigned int key){
    int pos = hashFunctionOne(key);
    Node* temp = table[pos];
    temp = temp->next;
    while(temp != NULL){
        if(temp->val == key){
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}
void chaining::chain_insert(unsigned int key, Node** table, int tableSize, int pageSize, int& countInsertChain){
    if(countInsertChain >= tableSize){
        cout << "failure" << endl;
        return;
    }

    int pos = hashFunctionOne(key);
    Node* temp = table[pos];
    Node* trav = table[pos];
    Node* new_insert = new Node(key);
    
    while(trav != NULL){
        if(trav->val == key){
            cout << "failure" << endl;
            return;
        }
        trav = trav->next;
    }

    if(pos < tableSize && table[pos]->next==NULL && countInsertChain < tableSize){     //insert at beginning of the list if list is empty
        table[pos]->next = new_insert;
        new_insert->next = NULL;

        //New node now remembers when it was being added
        new_insert->phys_addr = countInsertChain * pageSize;
        countInsertChain++;
        cout << "success" << endl;
    }
    //if linked list isn't empty, consider 3 cases: insert at front, middle and end
    else if(pos < tableSize && table[pos]->next!=NULL && countInsertChain < tableSize){
        while(temp->next != NULL){
            temp = temp->next;
        }
        if(key > table[pos]->next->val){//insert at front, need to have table[pos]->next because table[pos] refers to the head of the chain, which always has value 0
            temp = table[pos]->next;
            //cout << "2" << endl;
            table[pos]->next = new_insert;
            //cout << "3" << endl;
            new_insert->next = temp;
            //cout << "4" << endl;
        }
        else if(temp->val >= key){//insert at end
            temp->next = new_insert;
            new_insert->next = NULL;
        }
        else{//insert in the middle
            temp = table[pos];
            temp = temp->next;
            //stop at the node that points to the next node with val lesser than key val
            while(temp->next->val > key){
                temp = temp->next;
            }
            new_insert->next = temp->next;
            temp->next= new_insert;
        }
        new_insert->phys_addr = countInsertChain * pageSize;
        countInsertChain++;
        cout << "success" << endl;
    }
    else{
        cout << "failure" << endl;
    }
    return;
}
void chaining::chain_search(unsigned int key, Node** table, int tableSize){
    int pos = hashFunctionOne(key);
    Node* temp = table[pos];
    temp = temp->next;
    while(temp != NULL){
        if(temp->val == key){
            cout<< "found " + to_string(key) + " in " + to_string(pos)<< endl;
            return;
        }
        temp = temp->next;
    }
    cout << "not found" << endl;
    return;
}
void chaining::chain_write(unsigned int key, int vir_addr, int x, int pageSize, Node** table, int tableSize, int memory[], int memSize){
    Node* locate = chain_alrExist(table, tableSize, key);
    if(locate != NULL && locate->val == key && vir_addr < pageSize){
        memory[locate->phys_addr + vir_addr] = x;
        cout << "success" << endl;
    }
    else{
        cout << "failure" << endl;
    }
    return;
}
void chaining::chain_read(unsigned int key, Node** table, int tableSize, int vir_addr, int pageSize, int memory[]){
    if(chain_alrExist(table, tableSize, key) == NULL ||  vir_addr >= pageSize){
        cout << "failure" << endl;
    }
    else{
        int pos = chain_alrExist(table, tableSize, key)->phys_addr + vir_addr;
        cout <<  to_string(vir_addr) + " " + to_string(memory[pos]) << endl;
    }
    return;
}
void chaining::chain_del(unsigned int key, Node** table, int tableSize, int& countInsertChain){
    if(chain_alrExist(table, tableSize, key) == NULL){
       cout << "failure" << endl; 
    }
    else{
        //Three cases, delete front, delete middle and delete end
        Node* temp = table[hashFunctionOne(key)];
        Node* trav = temp;
        if(temp->next->val == key){//if key exist next to head
            Node* trav = temp->next->next;
            delete chain_alrExist(table, tableSize, key);
            temp->next = trav;
        }
        else if(chain_alrExist(table, tableSize, key)->next == NULL){//if key exist at the end
            while(trav->next->next != NULL){
                trav = trav->next;
            }
            delete chain_alrExist(table, tableSize, key);
            trav->next = NULL;
        }
        else{
            while(trav->next->val != key){
                trav = trav->next;
            }
            temp = trav->next->next;
            delete chain_alrExist(table, tableSize, key);
            trav->next = temp;
        }
        countInsertChain--;
        cout << "success" << endl; 
    }
    return;
}
void chaining::chain_prt(Node** table, int tableSize, int loc){
    Node* temp = table[loc];
    if(table[loc] -> next == NULL){
        cout << "chain is empty" << endl;
    }
    else{
        while(temp->next != NULL){
            temp = temp->next;
            cout << to_string(temp->val) + " ";
            //temp = temp->next;
        }
        cout << endl;
    }
    return;
}