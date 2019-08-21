#include <fstream>
#include <string>
#include <iostream>
using namespace std;
void printAry(ofstream& outFile4,int charCounts[]){
	for(int index=0;index<256;index++){
		if (charCounts[index]>0){
			char symbol=(char)index;
			outFile4<<symbol<<" "<<charCounts[index]<<endl;
		}
	}
}
void computeCount(istream& inFile1, ofstream& outFile4, int charCounts[]){
	unsigned char charIn;
	unsigned int index;
	while(!inFile1.eof()){
		charIn=inFile1.get();
		index=(int)charIn;
		charCounts[index]++;
	}
	printAry(outFile4, charCounts);
}

class treeNode{
public:
	string chStr;
	int prob;
	treeNode* next;
	treeNode* left;
	treeNode* right;
	string code;
treeNode(){
	this->chStr="dummy";
	this->prob=0;
	this->next=0;
	this->left=0;
	this->right=0;
}
friend class HuffmanBinaryTree;
};
class HuffmanBinaryTree{
public:
	treeNode* Root;
	treeNode* listHead=new treeNode();

treeNode* findSpot(treeNode* LHead, treeNode* nNode){
	treeNode* spot=LHead;
	while(spot->next!=NULL &&spot->next->prob<nNode->prob){
		spot=spot->next;
	}
	return spot;
}

void insertOneNode(treeNode* spot, treeNode* nNode){
	nNode->next=spot->next;
	spot->next=nNode;
}
void printList(treeNode* LHead, ofstream& outFile4){
	treeNode* temp=LHead;
	if(temp->next==NULL){
		outFile4 << "-->('"<<temp->chStr<<"', "<<temp->prob<<", NULL)-->NULL"<<endl;
		return;
	}
	else{
		outFile4 << "-->('"<<temp->chStr<<"', "<<temp->prob<<", '"<<temp->next->chStr<<"')";
	}
	printList(temp->next, outFile4);
}
void constructHuffmanBinTree(treeNode* Lh, ofstream &outFile4){
	while(Lh->next->next!=0){
		treeNode* nNode=new treeNode();
		nNode->prob=Lh->next->prob+Lh->next->next->prob;
		nNode->chStr=Lh->next->chStr+Lh->next->next->chStr;
		nNode->left=Lh->next;
		nNode->right=Lh->next->next;
		treeNode* spot=findSpot(Lh, nNode);
		insertOneNode(spot, nNode);
		Lh->next=Lh->next->next->next;
		outFile4<<"listHead";
		printList(Lh, outFile4);
	}
	Root=Lh->next;
}
void constructHuffmanLList(int charCounts[], ofstream& outFile){
	unsigned char chr;
	int counts;
	for(int index=0;index<=255;index++){
		if(charCounts[index]>0){
			treeNode* newNode=new treeNode();
			chr=(char)index;
			counts=charCounts[index];
			newNode->chStr=chr;
			newNode->prob=counts;
			treeNode* spot=findSpot(listHead,newNode);
			insertOneNode(spot,newNode);
		}
	}
	outFile<<"ListHead";
	printList(listHead,outFile);
}
bool isLeaf(treeNode* Root){
	if(Root->left==0&&Root->right==0) return true;
	else return false;
}
void getCode(treeNode* Root,string x, ofstream &outFile1, string charCode[]){
	if (isLeaf(Root)){
		Root->code=x;
		unsigned char a=char(Root->chStr[0]);
		unsigned int index = int(a);
		charCode[index]=Root->code;
		outFile1<<"("<<Root->chStr<<","<<Root->code<<")"<<endl;
	}
	else {
		getCode(Root->left, x+"0", outFile1,charCode);
		getCode(Root->right, x+"1", outFile1,charCode);
	}
}
void decode(istream& inFile, ofstream& outFile, treeNode* root){
	if(isLeaf(root)){
		outFile<<root->chStr;
		decode(inFile,outFile,Root);
	}
	else if(inFile.eof()){
		cout<<"the encode file is a corrupted file";
		return;
	}
	else{
		char nextBit=inFile.get();
		if(nextBit=='0'){
			decode(inFile,outFile,root->left);
		}
		if(nextBit=='1'){
			decode(inFile,outFile,root->right);
		}
	}
}
void encode(istream& inFile2, ofstream& outFile2, string charCode[]){
	unsigned char charIn;
	unsigned int index;
	while(!inFile2.eof()){
		charIn=inFile2.get();
		index=(int)charIn;
		string code1=charCode[index];
		outFile2<<code1;
	}
}
};

int main(int argc, char* argv[]){
	int charCounts[256]={0};
	string charCode[256]={""};
	ifstream inFile1(argv[1]);
	ofstream outFile1(argv[3]);
	ofstream outFile2(argv[4]);
	ofstream outFile3(argv[5]);
	ofstream outFile4(argv[6]);
	computeCount(inFile1, outFile4, charCounts);
	HuffmanBinaryTree t;
	outFile4<<"HuffmanLList"<<endl;
	t.constructHuffmanLList(charCounts, outFile4);
	outFile4<<"HuffmanBinTree"<<endl;
	t.constructHuffmanBinTree(t.listHead,outFile4);
	treeNode* Root=t.Root;
	t.getCode(Root,"", outFile1, charCode);

	ifstream inFile2(argv[2]);
	t.encode(inFile2, outFile2, charCode);

	ifstream decodeFile;
	string decodeFileName;
	cout<<"Name your File"<<endl;
	cin>>decodeFileName;
	decodeFile.open(decodeFileName.c_str());

	t.decode(decodeFile,outFile3,Root);
	inFile1.close();
	outFile1.close();
	outFile4.close();
	inFile2.close();
	outFile2.close();
	outFile3.close();
	decodeFile.close();
}
