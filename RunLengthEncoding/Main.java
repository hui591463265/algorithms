package encode;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;
import encode.RunLength;

public class Main {

	public static void main(String[] args) throws IOException {
		FileInputStream textFile = new FileInputStream (args[0]);
		PrintWriter outFile = new PrintWriter(new FileWriter(args[1]));
		Scanner inFile = new Scanner (textFile);
		RunLength x= new RunLength(inFile.nextInt(),inFile.nextInt(),inFile.nextInt(),inFile.nextInt());
		int r=0;
		outFile.println(x.numRows+" "+x.numCols+" "+x.minVal+" "+x.maxVal);
		while(inFile.hasNext()){
			int c=0;int count=0;int currVal;
			currVal=inFile.nextInt();
			outFile.print(r+" "+c+" "+currVal+" ");
			//System.out.print(r+" "+c+" "+currVal+" ");
			count++;
			while(c<=x.numCols){
				c++;
				if(c==x.numCols){
					outFile.println(count);
					//System.out.println(count);
					break;
				}
				int nextVal=inFile.nextInt();
				if(nextVal==currVal) {
					count++;
				}
				else{
					outFile.println(count);
					//System.out.println(count);
					currVal=nextVal;
					count=1; 
					outFile.print(r+" "+c+" "+currVal+" ");
					//System.out.print(r+" "+c+" "+currVal+" ");
				}
			}
			r++;
		}
		inFile.close();
		outFile.close();
	}
	

}
