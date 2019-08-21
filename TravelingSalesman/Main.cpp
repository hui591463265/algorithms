/*
 * Main.cpp
 *
 *  Created on: Dec 7, 2018
 *      Author: hui59
 */
#include <algorithm>
#include <vector>
#include <iterator>
#include <numeric>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;
class AstarNode{
	public:
		int ID;
		int numEdgeFromStart;
		int gStar=0;
		int hStar=0;
		int fStar=0;
		AstarNode* next;
		AstarNode* parent;
		int** costMatrix;

		AstarNode(int x){
			this->ID=x;
			this->next=NULL;
		}
		AstarNode(int sid, int Hfun, int cm, vector<int>& v){
					this->ID=sid;
					this->gStar=computeGstar();
					this->hStar=computeHstar(Hfun, v);
					this->fStar=this->hStar+this->gStar;
					this->next=NULL;
					this->numEdgeFromStart=0;
		}
		AstarNode(int sid, int Hfun, int** cm, vector<int>& v){
			this->costMatrix=cm;
			this->ID=sid;
			this->parent=NULL;
			this->gStar=computeGstar();
			this->hStar=computeHstar(Hfun, v);
			this->fStar=computeFstar();
			this->next=NULL;
			this->numEdgeFromStart=0;
		}
		int computeHstar(int Hfun, vector<int>& v){
			switch (Hfun){
			case 1:
				return callh1function(v);
			case 2:
				return callh2function(v);
			case 3:
				return callh3function(v);

			}
			return 0;
		}
		int callh1function(vector<int>& v){
			return accumulate(v.begin(), v.end(), 0)/v.size();//*num of nodes left
		}
		int callh2function(vector<int>& v){
			return *v.begin();//*num of nodes left
		}
		int callh3function(vector<int>& v){
			int size=v.size();
			 if (size % 2 == 0)
			    {
			      return (v[size/ 2 - 1] + v[size / 2]) / 2;//*num of nodes left
			    }
			    else
			    {
			      return v[size / 2];//*num of nodes left
			    }
		}

		int computeGstar(){
			if (this->parent!=NULL){
			return this->parent->gStar+costMatrix[gStar+this->parent->ID][ID];
			}
			return 0;
		}
		int computeFstar(){
			return this->hStar+this->gStar;

		}
		friend class AStarSearch;
};
class AStarSearch{

public:
	int numNodes;
	int start;
	int whichHFunction;
	int** costMatrix;
	AstarNode* CloseList;
	AstarNode* OpenList;
	int* childAry;

	AStarSearch(int numNodes, int startID, int whichHFunction){ // @suppress("Class members should be properly initialized")
		this->numNodes=numNodes;
		this->start=startID;
		this->whichHFunction=whichHFunction;
		this->OpenList=new AstarNode(0);
	}

	void loadMatrix(ifstream& in, vector<int>& v){
	//setup matrix size;
	costMatrix= new int*[numNodes+1];
		for(int i = 1; i <= numNodes; ++i){
			costMatrix[i] = new int[numNodes+1];
			costMatrix[i][i]=0;
		}
	childAry= new int[numNodes+1];
	int from;
	int to;
	int cost;
	while(in>>from){
		in>>to;
		in>>cost;
		costMatrix[from][to]=cost;
		costMatrix[to][from]=cost;
		v.push_back(cost);
	}
	sort(v.begin(), v.end());
	}

	void openInsert(AstarNode* sn){
		cout<<"start";
		AstarNode* spot=OpenList;
		if(spot->next==NULL){
			spot->next=sn;
		}else{
			while(spot->next!=NULL&&(spot->next->fStar<sn->fStar)){
				spot=spot->next;
			}
			sn->next=spot->next;
			spot->next=sn;
		}

	}
	AstarNode* OpenRemove(){
		if(OpenList->next==NULL){
			cout<<"nothing in list"<<endl;
			return NULL;
		}
		AstarNode* temp=OpenList->next;
		if(OpenList->next->next==NULL){
			OpenList->next=NULL;
			return temp;
		}else{
		OpenList->next=OpenList->next->next;
		return temp;
		}

	}
	void printOpen(ofstream& out){

	}
	void printClose(ofstream& out){

	}
	void copyChildList(int mi){
		for(int i = 1; i <= numNodes; ++i){
			childAry[i]=costMatrix[mi][i];
		}
	}
	bool IsInCloseList(AstarNode* cn, AstarNode* on){
		AstarNode* temp=CloseList->next;
		while(temp!=NULL){
			if(temp->ID==cn->ID&&temp->parent!=cn->parent){
				on=temp;
				return true;
			}
			temp=temp->next;
		}
		return false;
	}
	void removeClose( AstarNode* on){
		AstarNode* temp=CloseList;
		while(temp->next!=NULL){
			if(temp->next==on){
				temp->next=temp->next->next;
			}
		}
	}
	void closePush(AstarNode* cn){
		AstarNode* temp=CloseList->next;
		CloseList->next=cn;
		cn->next=temp;
	}
	bool checkPath(AstarNode* cn){
		if(cn->numEdgeFromStart==numNodes){
			return true;
		}
		return false;
	}
	friend class AStarNode;
	void trace(AstarNode* cn, ofstream& o){

	}
};
int main(int argc, char **argv) {
	ifstream infile(argv[1]);
	ofstream outfile(argv[2]);
	ofstream outfile2(argv[3]);
	outfile<<"i tried my best but there are too much Projects i have to do and with exams and finals coming up this week so i can't finish"<<endl;
	int numNodes, startID, whichHFunction;
	vector <int> nodesv;
	infile>>numNodes;
	do{
		cout<<"what is the startID"<<endl;
		cin>>startID;
		if(startID>numNodes||startID<1){
			cout<<"numNodes can't be larger than numNodes or less than 1"<<endl;
		}
		cout<<"what HFunction you want to use?"<<endl;
		cin>>whichHFunction;
		if(whichHFunction>3 || whichHFunction<1){
			cout<<"HFunction can only be 1 or 2 or 3"<<endl;
		}
	}while((whichHFunction>3 || whichHFunction<1)||(startID>numNodes||startID<1));
	AStarSearch AsS=AStarSearch(numNodes, startID, whichHFunction);
	AsS.loadMatrix(infile, nodesv);
	AstarNode* SNode=new AstarNode(startID,whichHFunction,AsS.costMatrix, nodesv);

	AsS.openInsert(SNode);
	AsS.printOpen(outfile2);
	AstarNode* currNode;

	do{
	currNode=AsS.OpenRemove();
	//get rid of node from temp vector
	int matrixIndex=currNode->ID;
	AsS.copyChildList(matrixIndex);
	for(int childIndex=1;childIndex<=numNodes;childIndex++){
	if (AsS.childAry[childIndex]>0){
		AstarNode* childNode=new AstarNode(childIndex,whichHFunction,AsS.childAry[childIndex],nodesv);
		AstarNode* oldNode;

		if(!AsS.IsInCloseList(childNode, oldNode)){
			childNode->parent=currNode;
			currNode->numEdgeFromStart++;
			AsS.openInsert(childNode);
			AsS.printOpen(outfile2);
		}else{
			if(childNode->fStar<oldNode->fStar){
				AsS.removeClose(oldNode);
				childNode->parent=currNode;
				AsS.openInsert(childNode);
				AsS.printOpen(outfile2);
			}
		}

	}
	}
	AsS.closePush(currNode);
	AsS.printClose(outfile);
	}while(!AsS.checkPath(currNode));
	AsS.trace(currNode, outfile);
	/*
	for( int i =0; i< nodesv.size();i++){

	cout<<nodesv[i]<<" ";

	}
	*/

	cout<<"start"<<endl;
}




