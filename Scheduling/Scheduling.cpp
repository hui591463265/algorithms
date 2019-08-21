#include <fstream>
#include <iostream>
using namespace std;
class listNode{
public:
	int jobId;
	int jobTime;
	int dependence;
	listNode* next;
	listNode(int x, int y){
		this->jobId=x;
		this->jobTime=y;
		this->next=NULL;
		this->dependence=0;
	}
};
class linkedList{
public:
	listNode* listHead;
	/*
	void insert(listNode* nNode){
		listNode* spot=listHead;
		while(spot->next!=NULL&&spot->next->dependence>spot->dependence){
			spot=spot->next;
		}
		nNode->next=spot->next;
		spot->next=nNode;
	}*/
	linkedList(){
		this->listHead=new listNode(0, 0);
	}
	bool isEmpty(){
		return listHead->next==NULL;
	}

};
class scheduling{
public:
	int procGiven;
	int numNodes;//c
	int totalJobTimes=0;//c
	int** adjacentMatrix;//c
	int** scheduleTable;
	int* jobTimeAry;//c
	linkedList* OPEN;
	int* processJob;
	int* processTime;
	int* parentCount;
	int* kidCount;
	int* jobDone;
	int* jobMarked;
	scheduling(int x, int procGiven){ // @suppress("Class members should be properly initialized")
		this->numNodes=x;
		this->procGiven=procGiven;
		this->processJob=new int[procGiven+1]();
		this->processTime=new int[procGiven+1]();
		this->parentCount=new int[numNodes+1]();
		this->kidCount=new int[numNodes+1]();
		this->jobDone=new int[numNodes+1]();
		this->jobMarked=new int[numNodes+1]();
		this->OPEN=new linkedList();
	}
void loadMatrix(ifstream& inFile){
	adjacentMatrix=new int* [numNodes+1];
	for(int i = 0; i <=numNodes; i++){
		adjacentMatrix[i] = new int[numNodes+1]();
	}
	int i,j;
	while(inFile>>i>>j){
		//cout<<i<<j<<endl;
		adjacentMatrix[i][j]=1;
		parentCount[j]++;
		kidCount[i]++;
	}
	/*
	for(int i = 1; i <=numNodes; i++){
		cout<<i<<":"<<parentCount[i]<<endl;
	}
	for(int i = 1; i <=numNodes; i++){
			cout<<i<<":"<<kidCount[i]<<endl;
	}*/
}
void computeTotalJobTimes(ifstream& inFile2){
	int s,time,num;
	inFile2>>s;
	jobTimeAry=new int[s+1];
	while(inFile2>>num>>time){
		jobTimeAry[num]=time;
		jobDone[num]=0;
		totalJobTimes+=time;
	}
	//cout<<totalJobTimes<<" tt"<<endl;
	scheduleTable=new int* [numNodes+1];
	for(int i = 0; i <=numNodes; i++){
		scheduleTable[i]=new int[totalJobTimes+1]();
	}
}

int getUnMarkOrphen(){
	/*for (int i=1;i<=numNodes;i++){
		cout<<jobMarked[i]<<endl;
	}*/
	for (int i=1;i<=numNodes;i++){
		if((jobMarked[i]==0)&&(parentCount[i]==0)){
			//cout<<"Orphan "<<i<<endl;
			return i;
		}
	}
	//cout<<"no orphans"<<endl;
	return -1;
}
listNode* RemovefromOpen(){
	listNode* temp=OPEN->listHead->next;
	OPEN->listHead->next=OPEN->listHead->next->next;
	return temp;
}
void Insert2Open(listNode* newNode){
	newNode->dependence=kidCount[newNode->jobId];
	//OPEN->insert(newNode);

	listNode* spot=OPEN->listHead;
	while(spot->next!=NULL&&spot->next->dependence>newNode->dependence){
		spot=spot->next;
	}
	newNode->next=spot->next;
	spot->next=newNode;

}
void printList(listNode* a){
	listNode* temp=a;
	if(a==NULL) {
		cout<<" IS EMPTY"<<endl;
		return;
	}
	if(temp->next==NULL){
		cout<<"-->("<<temp->jobId<<", "<<temp->jobTime<<", "<<temp->dependence<<")-->NULL"<<endl;
		return;
	}
	else{
		cout<< "-->("<<temp->jobId<<", "<<temp->jobTime<<", "<<temp->dependence<<")";
	}
	printList(temp->next);
}
int findProcessor(int procGiven){
	for(int i=1;i<=procGiven;i++){
		if(processTime[i]<=0){
			return i;
		}
	}
	return -1;
}
bool graphisEmpty(){
	for(int i=1;i<=numNodes;i++){
		if(jobDone[i]==0){
			return false;
		}
	}
	return true;
}
bool procIsfinished(){
	for(int i=1;i<=procGiven;i++){
		if(processJob[i]!=0){
			return false;
		}
	}
	return true;
}
int checkCycle(){
	if(OPEN->isEmpty()&&!graphisEmpty()&&procIsfinished()){
		return 1;
	}
	else{
		return 0;
	}
}
void updateTable(int avproc, int cT, listNode* temp){
	for(int i=cT;i<temp->jobTime+cT;i++){
		scheduleTable[avproc][i]=temp->jobId;
	}
}
void printTable(int currt, ofstream& outfile1){
	outfile1<<"Time: "<<currt<<endl;
	outfile1<<"------";
	for(int i=0;i<=totalJobTimes;i++){
		outfile1<<i<<"---";
	}
	outfile1<<endl;
	for(int i=1;i<=procGiven;i++){
		outfile1<<"P("<<i<<")|";
		for(int j=0;j<=currt;j++){
			if(scheduleTable[i][j]>0&&scheduleTable[i][j]<10){
				outfile1<<" "<<scheduleTable[i][j]<<" |";
			}
			else if(scheduleTable[i][j]>9&&scheduleTable[i][j]<100){
				outfile1<<" "<<scheduleTable[i][j]<<"|";
			}
			else{
				outfile1<<" - |";
			}
		}
		outfile1<<endl;
	}
}
int findDoneJob(int i){
	if(processTime[i]<=0){
		int x=processJob[i];
		processJob[i]=0;
		if(x>0){
		return x;
		}
		else{
			return -1;
		}
	}
	return -1;
}
void deleteNode(int node){
	jobDone[node]=1;
}
void deleteEdge(int node){
	for(int i=1;i<=numNodes;i++){
		if(adjacentMatrix[node][i]==1){
			parentCount[i]--;
		}
	}
}
void print(ofstream& outFile2, int currentTime){
	outFile2<<"time "<<currentTime<<endl;
	outFile2<<"jobMarked"<<endl;
	for (int i=1;i<=numNodes;i++){
		outFile2<<jobMarked[i]<<" ";
	}
	outFile2<<endl;
	outFile2<<"processTime"<<endl;
	for (int i=1;i<=procGiven;i++){
		outFile2<<processTime[i]<<" ";
	}
	outFile2<<endl;
	outFile2<<"parentCount"<<endl;
	for (int i=1;i<=numNodes;i++){
		outFile2<<parentCount[i]<<" ";
	}
	outFile2<<endl;
	outFile2<<"processJob"<<endl;
	for (int i=1;i<=procGiven;i++){
		outFile2<<processJob[i]<<" ";
	}
	outFile2<<endl;
	outFile2<<"jobDone"<<endl;
	for (int i=1;i<=numNodes;i++){
		outFile2<<jobDone[i]<<" ";
	}
	outFile2<<endl;
}
};

int main(int argc, char* argv[]) {
	ifstream inFile1(argv[1]);
	ifstream inFile2(argv[2]);
	ofstream outFile(argv[3]);
	ofstream outFile2(argv[4]);
	int numNodes,procGiven,orphan;
	inFile1 >> numNodes;
	cout<<"how many processors needed?"<<endl;
	cin>>procGiven;
	if(procGiven>numNodes){
		procGiven=numNodes;
	}
	scheduling s=scheduling(numNodes,procGiven);
	s.loadMatrix(inFile1);
	s.computeTotalJobTimes(inFile2);

	int procUsed=0,currentTime=0;
	//initialization done
	//for loop here
	while(!s.graphisEmpty()){
	orphan=s.getUnMarkOrphen();
	//step1-2
	while(orphan!=-1){
	s.jobMarked[orphan]=1;
	listNode* newNode=new listNode(orphan,s.jobTimeAry[orphan]);
	s.Insert2Open(newNode);
	orphan=s.getUnMarkOrphen();
	}
	//step3
	cout<<"OPEN";
	s.printList(s.OPEN->listHead->next);
	//step4+5
	while((!s.OPEN->isEmpty())&&procUsed<procGiven){
	int availProc=s.findProcessor(procGiven);
		if(availProc>0){
			procUsed++;
			listNode* newJob=s.RemovefromOpen();
			s.processJob[availProc]=newJob->jobId;
			s.processTime[availProc]=newJob->jobTime;
			s.updateTable(availProc, currentTime, newJob);
		}
	}
	//step6
	if(s.checkCycle()==1){
		cout<<"there is a cycle";
		return 0;
	}
	//step 7
	s.printTable(currentTime, outFile);
	//step 8
	currentTime++;
	//cout<<"TIME"<<currentTime<<endl;
	//step 9
	for(int i=1;i<=procGiven;i++){
		s.processTime[i]--;
	}
	//step10-12
	int job;
	for(int i=1;i<=procGiven;i++){
		job=s.findDoneJob(i);
		if(job>0){
			//cout<<job<<" this is node"<<endl;
			s.deleteNode(job);
			s.deleteEdge(job);
			procUsed--;
		}
	}
	//step 13
	s.print(outFile2, currentTime);
	}
	s.printTable(currentTime, outFile);
	return 0;
}
