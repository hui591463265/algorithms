#include <fstream>
#include <string>
#include <iostream>
using namespace std;
class listNode{
public:
    int data;
    listNode* next;
    listNode(){
        this->data=-1;
        this->next=NULL;
    }
    listNode(int p){
        this->data=p;
        this->next=NULL;
    }
    friend class linkedListStack;
    friend class RadixSort;
    friend class linkedListQueue;
};
class linkedListStack{
public:
    listNode* top;

    linkedListStack(){
        top=new listNode();
        this->top=NULL;
    };
    bool isEmpty(){
        return top==NULL;
    };
    void push(listNode* nNode){
        if(isEmpty()){
            top=nNode;
        }
        else{
        	nNode->next=top;
        	top=nNode;
        }
    };
    listNode* pop(){
        if(isEmpty()) return NULL;
        listNode* pop=top;
        top=top->next;
        return pop;
    };
    void printStack(listNode* node, ofstream& outFile2){
    	outFile2<<"Top";
    	while(node!=NULL){
    		outFile2<<"->("<<node->data<<",";
    		if(node->next==NULL){
    			outFile2<<"NULL)";
    			break;
    		}
    		outFile2<<node->next->data<<")";
    		node=node->next->next;
    	}
    	outFile2<<"-> NULL"<<endl;

    };
    friend class RadixSort;
};
class linkedListQueue{
public:
    listNode* head;
    listNode* tail;
linkedListQueue(){
    listNode* dummy=new listNode(-1);
    head=tail=dummy;
};
    void addTail(listNode* nNode){
        nNode->next=NULL;
        tail->next=nNode;
        tail=nNode;
    };
    bool isEmpty(){
        if(head->next==NULL&&head->data==-1) return 1;
        return 0;
    };
    listNode* deleteFront(){
        if(isEmpty()) return NULL;
        listNode* temp=head->next;
        head->next=head->next->next;
        if(head->next==NULL) tail=head;
        return temp;
    };
    void printQueue(int index, ofstream& outFile2){
        listNode* topp=head->next;
        outFile2<<"Front("<<index<<")->";
        while(topp!=NULL){
        	outFile2<<topp->data<<"->";
            topp=topp->next;
        }
        outFile2<<"NULL"<<endl;
        outFile2<<"Tail("<<index<<")->"<<tail->data<<"->NULL"<<endl;
    };
    friend class RadixSort;
};
class RadixSort{
public:
    static const int tableSize=10;
    linkedListQueue* hashTable[2][tableSize];
    int data;
    int currentTable;
    int previousTable;
    int maxDigits;
    int currentDigit;
    int largestNum;
    int currentQueue;
    RadixSort(){ // @suppress("Class members should be properly initialized")
        for(int i=0;i<tableSize;i++){
            hashTable[0][i] = new linkedListQueue();
            hashTable[1][i] = new linkedListQueue();
        }
    }
    void dumpStack(linkedListStack* Top,int currentTable, ofstream& outFile2){
            while(Top->top->next!=NULL){
                listNode* node=Top->pop();
                int digit=getDigit(node,currentDigit);
                int hashIndex=digit;
                hashTable[currentTable][hashIndex]->addTail(node);
            }
            printTable(currentTable, outFile2);
        }
        int getMaxDigit(int i){
            int maxD=0;
            while(i!=0){
                maxD++;
                i=i/10;
            }
            maxDigits=maxD;
            return maxDigits;
        }
        int getDigit(listNode* p, int currD){
        	int d=p->data;
        	for(int i=0;i<currD;i++){
        		d=d/10;
        	}
            return d%10;
        }
        //int hashIndex(int digit){}
        void printTable(int j, ofstream& outFile2){
        	for(int i=0;i<tableSize;i++){
        		if(!hashTable[j][i]->isEmpty()){
        			hashTable[j][i]->printQueue(i, outFile2);
        		}
        	}
        }
    linkedListStack* loadStack(istream& inFile, ofstream& outfile2){
		linkedListStack*Top=new linkedListStack();
		largestNum=0;

		while(inFile>>data){
			if(data>largestNum){
				largestNum=data;
			}
			listNode* newNode=new listNode(data);
			Top->push(newNode);
		}
		Top->printStack(Top->top, outfile2);
		return Top;

	};
    void OutputSortedData(ofstream& outFile1){
    	for(int i=0;i<tableSize;i++){
			if(!hashTable[0][i]->isEmpty()){
				hashTable[0][i]->printQueue(i, outFile1);
			}
			if(!hashTable[1][i]->isEmpty()){
				hashTable[1][i]->printQueue(i, outFile1);
			}
		}
    }
    void radixSort(linkedListStack* top, ofstream& outFile2){
            maxDigits=getMaxDigit(largestNum);
            currentDigit=0;
            currentTable=0;
            dumpStack(top,currentTable,outFile2);
            currentDigit++;
            previousTable=0;
            currentTable=1;
            currentQueue=0;
            while(currentDigit<=maxDigits){
            	while(currentQueue<=tableSize-1){
					while(!hashTable[previousTable][currentQueue]->isEmpty()){
						listNode* node=hashTable[previousTable][currentQueue]->deleteFront();
						int hashIndex=getDigit(node,currentDigit);
						hashTable[currentTable][hashIndex]->addTail(node);
					}
					currentQueue++;
            	}
            printTable(currentTable, outFile2);
            int temp=currentTable;
            currentTable=previousTable;
            previousTable=temp;
            currentQueue=0;
            currentDigit++;
            }
            //printTable(currentTable, outFile2);
    };
    friend class listNode;
};
int main(int argc, char* argv[]){
    ifstream inFile1(argv[1]);
    ofstream outFile1(argv[2]);
    ofstream outFile2(argv[3]);
    RadixSort rs;
    linkedListStack* topp;
    topp=rs.loadStack(inFile1, outFile2);
    rs.radixSort(topp,outFile2);
    rs.OutputSortedData(outFile1);
    return 0;
}

