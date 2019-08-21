#include <fstream>
#include <iostream>
#include <string>
using namespace std;
class treeNode{
public:
	string chStr;
	int prob;
	treeNode* next;
	treeNode* left;
	treeNode* right;
	string code;

	treeNode(string z, int x){
		this->chStr=z;
		this->prob=x;
		this->next=0;
		this->left=0;
		this->right=0;
		this->code="dummy";
	}
	treeNode(){
		this->chStr="dummy";
		this->prob=0;
		this->next=0;
		this->left=0;
		this->right=0;
		this->code="dummy";
	}
	friend class HuffmanLinkedList;
	friend class HuffmanBinaryTree;
};

class HuffmanBinaryTree{
public:
	treeNode* root;
	treeNode* listHead= new treeNode("dummy",0 );
treeNode* findSpot(treeNode* LHead, treeNode* nNode){
			treeNode* spot=LHead;
			while(spot->next!=NULL &&spot->next->prob<nNode->prob){
				spot=spot->next;
			}
			return spot;
};

void insertOneNode(treeNode* spot, treeNode* nNode){
	nNode->next=spot->next;
	spot->next=nNode;
};
void printList(treeNode* LHead, ofstream& outFile5){
	treeNode* temp=LHead;
	if(temp->next==NULL){
		outFile5 << "-->('"<<temp->chStr<<"', "<<temp->prob<<", NULL)-->NULL"<<endl;
		return;
	}
	else{
		outFile5 << "-->('"<<temp->chStr<<"', "<<temp->prob<<", '"<<temp->next->chStr<<"')";
	}
	printList(temp->next, outFile5);
}

void constructHuffmanLList(ifstream &inFile, ofstream &outFile){
	string word;
	int data;
	if (inFile.is_open() && inFile.good()) {
		while(!inFile.eof()){
			treeNode* nNode=new treeNode();
			inFile >> word;
			inFile >> data;
			nNode->chStr=word;
			nNode->prob=data;
			nNode->next=NULL;
			treeNode* spot=findSpot(listHead,nNode);
			insertOneNode(spot,nNode);
			outFile << "ListHead";
			printList(listHead, outFile);
		}
		inFile.close();
		}
	else {
		cout << "Failed to open file";
	}
}
void printNode(treeNode* Lh, ofstream &outFile){
	treeNode* temp=Lh;
		outFile<<"("<<temp->chStr<<", "<<temp->prob<<", ";
		if(temp->next!=0) outFile<<temp->next->chStr<<", ";
			else outFile<<"NULL, ";
		if(temp->left!=0) outFile<<temp->left->chStr<<", ";
			else outFile<<"NULL, ";
		if(temp->right!=0) outFile<<temp->right->chStr<<")";
			else outFile<<"NULL)";
		outFile<<endl;
}
void constructHuffmanBinTree(treeNode* Lh, ofstream &outFile){
	while(Lh->next->next!=0){
		treeNode* nNode=new treeNode();
		nNode->prob=Lh->next->prob+Lh->next->next->prob;
		nNode->chStr=Lh->next->chStr+Lh->next->next->chStr;
		nNode->left=Lh->next;
		nNode->right=Lh->next->next;
		treeNode* spot=findSpot(Lh, nNode);
		insertOneNode(spot, nNode);
		Lh->next=Lh->next->next->next;
		printList(Lh, outFile);
	}
	root=Lh->next;
}
void preOrderTraversal(treeNode* Root,ofstream &outFile2){
	if(Root==0) return;
	printNode(Root, outFile2);
	preOrderTraversal(Root->left,outFile2);
	preOrderTraversal(Root->right,outFile2);
}
void inOrderTraversal(treeNode* Root,ofstream &outFile3){
	if(Root==0) return;
	preOrderTraversal(Root->left,outFile3);
	printNode(Root, outFile3);
	preOrderTraversal(Root->right,outFile3);
}
void postOrderTraversal(treeNode* Root,ofstream &outFile4){
	if(Root==0) return;
	preOrderTraversal(Root->left,outFile4);
	preOrderTraversal(Root->right,outFile4);
	printNode(Root, outFile4);
}
bool isLeaf(treeNode* Root){
	if(Root->left==0&&Root->right==0) return true;
	else return false;
}
void getCode(treeNode* Root,string x, ofstream &outFile1){
	if (isLeaf(Root)){
		Root->code=x;
		outFile1<<"("<<Root->chStr<<","<<Root->code<<")"<<endl;
	}
	else {
		getCode(Root->left, x+"0", outFile1);
		getCode(Root->right, x+"1", outFile1);
	}
}
};
int main(int argc, char* argv[]){
	ifstream inFile(argv[1]);
	ofstream outFile1(argv[2]);
	ofstream outFile2(argv[3]);
	ofstream outFile3(argv[4]);
	ofstream outFile4(argv[5]);
	ofstream outFile5(argv[6]);
	HuffmanBinaryTree t;
	t.constructHuffmanLList(inFile, outFile5);
	t.constructHuffmanBinTree(t.listHead, outFile5);
	treeNode* Root=t.root;
	t.preOrderTraversal(Root,outFile2);
	t.inOrderTraversal(Root,outFile3);
	t.postOrderTraversal(Root,outFile4);
	t.getCode(Root," ", outFile1);
	outFile1.close();
	outFile2.close();
	outFile3.close();
	outFile4.close();
	outFile5.close();

}


