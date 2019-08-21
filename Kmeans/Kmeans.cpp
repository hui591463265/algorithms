#include <fstream>
#include <string>
#include <iostream>
#include <array>
#include <cmath>
using namespace std;

class Kmean{
public:
	class Point{
		public:
			int Xcoord;
			int Ycoord;
			int Label;
			double Distance;
	};

	class xyCoord{
	public:
		double Xcoord;
		double Ycoord;
		int Label;
	};

	int K;
int numPts;
Point *pointSet;
int numRows;
int numCols;
int minVal;
int maxVal;
int **imgAry;
int change;
xyCoord *Kcentroid;
Kmean(int a, int b, int c, int d, int e){
	//Maybe here
	this->change=-1;
	this->K=e;
	this->numPts=0;
	this->numRows=a;
	this->numCols=b;
	this->minVal=c;
	this->maxVal=d;
	this->Kcentroid=new xyCoord[K+1];
	this->imgAry=new int*[numRows];
	for(int i=0;i<numRows;i++){
		imgAry[i]=new int[numCols];
	}
};
int extractPts(istream& inFile, ofstream &outFile){
	int i, j, x, t=0;
	for(i=0;i<numRows;i++){
		for(j=0;j<numCols;j++){
			inFile>>x;
			imgAry[i][j]=x;
			if(x>=1){
				t++;
				outFile<<i<<" "<<j<<endl;
			}
		}
	}
	pointSet = new Point[t];
	return t;
}
void loadPointSet(istream &outFile1, ofstream &outFile3){
	int  x, y, i=0;
	while(outFile1 >> x >> y){
		pointSet[i].Xcoord=x;
		pointSet[i].Ycoord=y;
		pointSet[i].Distance=99999;
		i++;
	}
}
void Point2Image(){
	for(int i=0;i<numPts;i++){
		int x = pointSet[i].Xcoord;
		int y = pointSet[i].Ycoord;
		imgAry[x][y]=pointSet[i].Label;
	}
}
void printImage(int iter, ofstream& outFile){
	outFile<<"current iteration: "<<iter<<endl;
	for(int i=0;i<numRows;i++){
		for(int j=0;j<numCols;j++){
			if(imgAry[i][j]>0){
				outFile<<imgAry[i][j]<<" ";
			}
			else{
				outFile<<"  ";
			}
		}
		outFile<<endl;
	}
}
void assignLabel(){
	int front=0;
	int back=numPts-1;
	int label=1;
	while(front<=back){
		if(label>K){
			label=1;
		}
		pointSet[front].Label=label;
		front++;
		label=label+1;
		pointSet[back].Label=label;
		back--;
		label=label+1;
	}
}
void computeCentroids(){
	int sumX[K+1]={0};
	int sumY[K+1]={0};
	int totalPt[K+1]={0};
	int index=0;
	while(index<numPts){
		int label=pointSet[index].Label;
		sumX[label]+=pointSet[index].Xcoord;
		sumY[label]+=pointSet[index].Ycoord;
		totalPt[label]++;
		index++;
	}
	int lab=1;
	while(lab<=K){
		Kcentroid[lab].Xcoord=(sumX[lab]/totalPt[lab]);
		Kcentroid[lab].Ycoord=(sumY[lab]/totalPt[lab]);
		lab++;
	}
}
double computeDist(Point& p, xyCoord xy){
	double xDiff=abs(p.Xcoord-xy.Xcoord);
	double yDiff=abs(p.Ycoord-xy.Ycoord);
	double x=xDiff*xDiff;
	double y=yDiff*yDiff;
	double distance=sqrt(x+y);
	return distance;
}
void DistanceMinLable(Point& x){
	double OldDist=x.Distance;
	double newDist;
	int OldLabel=x.Label;
	int label=1;
	while(label<=K){
		newDist=computeDist(x, Kcentroid[label]);
		if (newDist<OldDist){
			OldLabel=label;
			OldDist=newDist;
		}
		label++;
	}
	if(OldDist<x.Distance){
		x.Distance=OldDist;
		x.Label=OldLabel;
		change++;
	}
}
void kMeansClustering(ofstream& outFile){
	int iteration=0;
	assignLabel();

	while(change!=0){
		Point2Image();
		printImage(iteration,outFile);
		change=0;
		computeCentroids();
		int index=0;
		while(index<numPts){
			DistanceMinLable(pointSet[index]);
			index++;
		}
		iteration++;
	}
}
void writePtSet(ofstream& outFile){
	int i=0;
	outFile<<numPts<<endl;
	outFile<<numRows<<" "<<numCols<<endl;
	while(i<numPts){
		outFile<<pointSet[i].Xcoord<<" "<<pointSet[i].Ycoord<<" "<<pointSet[i].Label<<endl;
		i++;
	}
}
};
int main(int argc, char* argv[]){
	ifstream inFile(argv[1]);
	ofstream outFile1(argv[2]);
	ofstream outFile2(argv[3]);
	ofstream outFile3(argv[4]);
	int a, b, c, d, e;
	inFile >> a >> b >> c >> d;
	cout<<"please provide number K"<<endl;
	cin >> e;
	Kmean k=Kmean(a,b,c,d,e);

	k.numPts=k.extractPts(inFile,outFile1);

	outFile1.close();
	ifstream outFile1new(argv[2]);

	k.loadPointSet(outFile1new, outFile3);
	k.kMeansClustering(outFile2);
	k.writePtSet(outFile3);

	inFile.close();
	outFile1new.close();
	outFile2.close();
	outFile3.close();
}
