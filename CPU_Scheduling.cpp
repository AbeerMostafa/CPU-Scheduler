#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
using namespace std;

struct process
{
	int PID;		int burst;		int prio;
	int arrive;		int start;		int finish;
	int wait;		int turn;		int del;
	int execut;
};

//create a pointer to the struct
process * pArr = new process[50];
int Algorithm; //number of the algorithm

void choose_algorithm();
void take_input(); 	//function that takes the 50 processes from a text file and store them in struct members
void FCFS();		//algorithm FCFS
void priority(); 	//algorithm priority
void SJF();			//algorithm SJF
void round_robin(); //algorithm Round Robin
void compare();		//compre between algorithms performance
void scheduling();  //performing the right algorithm
void calc_output(); //calculate the mean and std Deviation and put in output text file

//the main function is pretty clear that doesn't need to be commented

int main()
{
	choose_algorithm();
	
	take_input();
	
	scheduling();
	
	calc_output();	
}



void choose_algorithm()
{
	//User Interface
	
	cout<<"\t ^ Welcome to CPU Scheduling Algorithms ^ \n \n \t Choose the prefered Algorithm : \n \t 1.FCFS Algorithm \n \t 2.Priority Algorithm \n \t 3.SJF Algorithm \n \t 4.Round Robin Algorithm \n \t 5.Compare between all algorithms to see the highest performance \n \t";
	
	cin>>Algorithm;
	
	//checking the user input
	
	if(Algorithm > 5 || Algorithm < 1)
		cout<<"Not Valid \n";
}

void take_input()
{
	string B, P, D;
	
	ifstream inFile ("input.txt");
	
	inFile >> B >> D >> P; //taking the first line of the input file (Burst,Priority,Delay)
	
		//taking the 50 values from the input text file
	
	for(int i=0; i<50; i++)
	{
			inFile >> pArr[i].burst >> pArr[i].del >> pArr[i].prio; 
			
			pArr[i].PID = i+1;
			//initial values
		if(i==0)
			{
				pArr[i].arrive = pArr[i].del;
				pArr[i].start = pArr[i].arrive;
			}
		else
				pArr[i].arrive = pArr[i-1].arrive + pArr[i].del;	
			}
}
void calc_output()
{
	//variables used to calculate the std Deviation
	
	float std_devW[50]={0}, std_devT[50]={0};
	float maxW=0, minW=0, meanW=0, maxT=0, minT=0, meanT=0, S1=0, S2=0;
	
	//creating the output text file
	
	ofstream outFile ("output.txt");
	
	outFile << "PID  Burst  Priority  Arrival  Start  Finish  Wait  Turnaround \n";
	
	for(int i=0; i<50; i++)
			outFile << pArr[i].PID << "\t" << pArr[i].burst << "\t" << pArr[i].prio << "\t" << pArr[i].arrive << "\t" << pArr[i].start << "\t" 
			<< pArr[i].finish <<"\t" << pArr[i].wait << "\t" << pArr[i].turn << "\n"; 
	outFile << "------------------------------------------------------------------------- \n";
	
	//the mean
	
	for(int i=0; i<50; i++)
	{
		meanT += pArr[i].turn;
		meanW += pArr[i].wait;
	if(i==0)
		{
		maxW=pArr[0].wait;  minW=pArr[0].wait;
		maxT=pArr[0].turn;  minT=pArr[0].turn;
		
		}
	else
		{
		if(pArr[i].wait<minW)
		minW = pArr[i].wait;
		
		if(pArr[i].wait>maxW)
		maxW = pArr[i].wait;
		
		if(pArr[i].turn<minT)
		minT = pArr[i].turn;
		
		if(pArr[i].turn>maxT)
		maxT = pArr[i].turn;
		}
	}
	
	meanT /= 50;  meanW /= 50;
	
	//calculate the standard deviation
	
	for(int i=0; i<50; i++)
	{
		std_devW[i] = pow((pArr[i].wait-meanW),2);
		S1 += std_devW[i];
		std_devT[i] = pow((pArr[i].turn-meanT),2);
		S2 += std_devT[i];
	}
	
	S1 /= 50;  S2 /= 50;
	S1=sqrt(S1);  S2=sqrt(S2);
	
	//display the result
	
	outFile << "\tWait  Turnaround \n"
			<< "Min   \t" << minW <<"\t"<<minT<<"\n"
			<< "Mean   \t" << meanW <<"\t"<<meanT<<"\n"
			<< "Max   \t" << maxW <<"\t"<<maxT<<"\n";
			
			outFile << fixed << showpoint << setprecision(2);
			
			outFile << "StdDev  " << S1 <<" \t"<<S2<<"\n";
}

void SJF()
{
	process  temp; //temporary struct object when swapping processes
	
	/*
	 * the basic idea is about calculating which process starts first
	 * in SJF we depend mainly on the burst time of each process
	 * when the last process finishes executing the scheduler compares between arrived processes
	 * the shortest burst time executes first
	  */
	for(int i=1; i<50; i++) //calculating the start time
		{
			if(pArr[i].arrive > (pArr[i-1].start + pArr[i-1].burst))
			{
				pArr[i].start=pArr[i].arrive;
			}
			else if (pArr[i].arrive <= (pArr[i-1].start + pArr[i-1].burst) && pArr[i+1].arrive > (pArr[i-1].start + pArr[i-1].burst))
			{
				pArr[i].start =pArr[i-1].start + pArr[i-1].burst;
			}
			else if (pArr[i].arrive <= (pArr[i-1].start + pArr[i-1].burst) && (i+1) >49)
			{
				pArr[i].start =pArr[i-1].start + pArr[i-1].burst;
			}
			else 
			{
				if((i+3)<50)
				{
				for(int j=i+1; j<i+4; j++)
				{
					if(pArr[i].arrive <= (pArr[i-1].start + pArr[i-1].burst) && pArr[j].arrive <= (pArr[i-1].start + pArr[i-1].burst))
					{
					if(pArr[i].burst < pArr[j].burst)
						{
						pArr[i].start =pArr[i-1].start + pArr[i-1].burst;
						}
					else if(pArr[i].burst > pArr[j].burst)
						{
						temp=pArr[i];
						pArr[i]=pArr[j];
						pArr[j]=temp;
						pArr[i].start =pArr[i-1].start + pArr[i-1].burst;
						}
					}
					if(pArr[i].arrive > (pArr[i-1].start + pArr[i-1].burst) &&
					 pArr[j].arrive > (pArr[i-1].start + pArr[i-1].burst) &&
					  pArr[i].arrive==pArr[j].arrive)
					{
					if(pArr[i].burst < pArr[j].burst)
						{
						pArr[i].start =pArr[i-1].start + pArr[i-1].burst;
						}
					else if(pArr[i].burst > pArr[j].burst)
						{
						temp=pArr[i];
						pArr[i]=pArr[j];
						pArr[j]=temp;
						pArr[i].start =pArr[i-1].start + pArr[i-1].burst;
						}
					}
				}
				
			}
		}
		}
		
		//reArranging the processes
		
		for(int i=0; i<50; i++)
		{
			for(int j=0; j<49; j++)
			{
				if(pArr[j].PID > pArr[j+1].PID)
				{
					temp=pArr[j];
					pArr[j]=pArr[j+1];
					pArr[j+1]=temp;
				}
			}
		}

		//calculate the finish, wait, turn around time

		for(int i=0; i<50; i++)
		{
			pArr[i].wait = pArr[i].start - pArr[i].arrive;
			pArr[i].finish = pArr[i].start + pArr[i].burst;
			pArr[i].turn = pArr[i].wait + pArr[i].burst;
		}
	
}

void priority()
{
	process  temp; //temporary struct object when swapping processes
	
	/*
	 * the basic idea is about calculating which process starts first
	 * in  priority algorithm we depend mainly on the priority of each process
	 * when the last process finishes executing the scheduler compares between arrived processes
	 * the most priority executes first
	  */
	
	for(int i=1; i<50; i++) //calculate start time
		{
			if(pArr[i].arrive > (pArr[i-1].start + pArr[i-1].burst))
			{
				pArr[i].start=pArr[i].arrive;
			}
			else if (pArr[i].arrive <= (pArr[i-1].start + pArr[i-1].burst) && pArr[i+1].arrive > (pArr[i-1].start + pArr[i-1].burst))
			{
				pArr[i].start =pArr[i-1].start + pArr[i-1].burst;
			}
			else if (pArr[i].arrive <= (pArr[i-1].start + pArr[i-1].burst) && (i+1) >49)
			{
				pArr[i].start =pArr[i-1].start + pArr[i-1].burst;
			}
			else if(pArr[i].prio == pArr[i+1].prio && pArr[i].arrive <= (pArr[i-1].start + pArr[i-1].burst))
						{
						pArr[i].start =pArr[i-1].start + pArr[i-1].burst;
						}
			else 
			{
				if((i+3)<50)
				{
				for(int j=i+1; j<i+4; j++)
				{
					if(pArr[i].arrive <= (pArr[i-1].start + pArr[i-1].burst) && pArr[j].arrive <= (pArr[i-1].start + pArr[i-1].burst))
					{
					if(pArr[i].prio < pArr[j].prio)
						{
						pArr[i].start =pArr[i-1].start + pArr[i-1].burst;
						}
					else if(pArr[i].prio > pArr[j].prio)
						{
						temp=pArr[i];
						pArr[i]=pArr[j];
						pArr[j]=temp;
						pArr[i].start =pArr[i-1].start + pArr[i-1].burst;
						}
					}
					if(pArr[i].arrive > (pArr[i-1].start + pArr[i-1].burst) &&
					 pArr[j].arrive > (pArr[i-1].start + pArr[i-1].burst) &&
					  pArr[i].arrive==pArr[j].arrive)
					{
					if(pArr[i].prio < pArr[j].prio)
						{
						pArr[i].start =pArr[i-1].start + pArr[i-1].burst;
						}
					else if(pArr[i].prio > pArr[j].prio)
						{
						temp=pArr[i];
						pArr[i]=pArr[j];
						pArr[j]=temp;
						pArr[i].start =pArr[i-1].start + pArr[i-1].burst;
						}
					}
				}
				
			}
		}
		}
		
		//reArranging the processes
		
		for(int i=0; i<50; i++)
		{
			for(int j=0; j<49; j++)
			{
				if(pArr[j].PID > pArr[j+1].PID)
				{
					temp=pArr[j];
					pArr[j]=pArr[j+1];
					pArr[j+1]=temp;
				}
			}
		}

		//calculate the finish, wait, turn around time

		for(int i=0; i<50; i++)
		{
			pArr[i].wait = pArr[i].start - pArr[i].arrive;
			pArr[i].finish = pArr[i].start + pArr[i].burst;
			pArr[i].turn = pArr[i].wait + pArr[i].burst;
		}
}

void FCFS()
{
	/* the easiest algorithm,
	 * which process comes first, executes first
	 */
	for(int i=1; i<50; i++)
	{
		if( pArr[i].arrive < (pArr[i-1].start + pArr[i-1].burst) )
		pArr[i].start = pArr[i-1].start + pArr[i-1].burst;
		else
		 pArr[i].start = pArr[i].arrive; 
	}
		//calculate the finish, wait, turn around time
	
	for(int i=0; i<50; i++)
	{
		pArr[i].wait = pArr[i].start - pArr[i].arrive;
		pArr[i].finish = pArr[i].start + pArr[i].burst;
		pArr[i].turn = pArr[i].wait + pArr[i].burst;
	}
			
}

void round_robin()
{
	float cycles[50];
	const int time_slice =10;
	/* the basic idea of round robin algorithm is that is specifies a quantum
	 * the quantum is the time slice for each process
	 * mainly performed as FCFS but with quantum
	 */
	for(int i=0; i<50; i++)
	{
		cycles[i]=float(pArr[i].burst)/float(time_slice); //casting integer variables into float
		cycles[i]=ceil(cycles[i]);
	}
	
	for(int i=1; i<50; i++)
	{
		if( pArr[i].arrive < (pArr[i-1].start + pArr[i-1].burst) )
		pArr[i].start = pArr[i-1].start + pArr[i-1].burst;
		else
		 pArr[i].start = pArr[i].arrive; 
	}
		//calculate the finish, wait, turn around time
	
	for(int i=0; i<50; i++)
	{
		pArr[i].wait = pArr[i].start - pArr[i].arrive;
		pArr[i].finish = pArr[i].start + pArr[i].burst;
		pArr[i].turn = pArr[i].wait + cycles[i] * time_slice;
	}
	
}

void compare()
{
	
	float std_devW[50]={0}, std_devT[50]={0};
	float maxW=0, minW=0, meanW=0, maxT=0, minT=0, meanT=0, S1=0, S2=0;
	
	//creating the output text file
	
	ofstream outFile ("comparison.txt");
	
	FCFS();
	
	outFile<<"FCFS Algorithm \n";

	
	for(int i=0; i<50; i++)
	{
		meanT += pArr[i].turn;
		meanW += pArr[i].wait;
	if(i==0)
		{
		maxW=pArr[0].wait;  minW=pArr[0].wait;
		maxT=pArr[0].turn;  minT=pArr[0].turn;
		
		}
	else
		{
		if(pArr[i].wait<minW)
		minW = pArr[i].wait;
		
		if(pArr[i].wait>maxW)
		maxW = pArr[i].wait;
		
		if(pArr[i].turn<minT)
		minT = pArr[i].turn;
		
		if(pArr[i].turn>maxT)
		maxT = pArr[i].turn;
		}
	}
	
	meanT /= 50;  meanW /= 50;
	
	//calculate the standard deviation
	
	for(int i=0; i<50; i++)
	{
		std_devW[i] = pow((pArr[i].wait-meanW),2);
		S1 += std_devW[i];
		std_devT[i] = pow((pArr[i].turn-meanT),2);
		S2 += std_devT[i];
	}
	
	S1 /= 50;  S2 /= 50;
	S1=sqrt(S1);  S2=sqrt(S2);
	
	//display the result
	
	outFile << "\tWait  Turnaround \n"
			<< "Min   \t" << minW <<"\t"<<minT<<"\n"
			<< "Mean   \t" << meanW <<"\t"<<meanT<<"\n"
			<< "Max   \t" << maxW <<"\t"<<maxT<<"\n";
			
			outFile << fixed << showpoint << setprecision(2);
			
			outFile << "StdDev  " << S1 <<" \t"<<S2<<"\n";
			outFile << "-------------------------------- \n";
			
			priority();
			
	outFile<<"Priority Algorithm \n";
	
	for(int i=0; i<50; i++)
	{
		meanT += pArr[i].turn;
		meanW += pArr[i].wait;
	if(i==0)
		{
		maxW=pArr[0].wait;  minW=pArr[0].wait;
		maxT=pArr[0].turn;  minT=pArr[0].turn;
		
		}
	else
		{
		if(pArr[i].wait<minW)
		minW = pArr[i].wait;
		
		if(pArr[i].wait>maxW)
		maxW = pArr[i].wait;
		
		if(pArr[i].turn<minT)
		minT = pArr[i].turn;
		
		if(pArr[i].turn>maxT)
		maxT = pArr[i].turn;
		}
	}
	
	meanT /= 50;  meanW /= 50;
	
	//calculate the standard deviation
	
	for(int i=0; i<50; i++)
	{
		std_devW[i] = pow((pArr[i].wait-meanW),2);
		S1 += std_devW[i];
		std_devT[i] = pow((pArr[i].turn-meanT),2);
		S2 += std_devT[i];
	}
	
	S1 /= 50;  S2 /= 50;
	S1=sqrt(S1);  S2=sqrt(S2);
	
	//display the result
	
	outFile << "\tWait  Turnaround \n"
			<< "Min   \t" << minW <<"\t"<<minT<<"\n"
			<< "Mean   \t" << meanW <<"\t"<<meanT<<"\n"
			<< "Max   \t" << maxW <<"\t"<<maxT<<"\n";
			
			outFile << fixed << showpoint << setprecision(2);
			
			outFile << "StdDev  " << S1 <<" \t"<<S2<<"\n";
			outFile << "-------------------------------- \n";
			
	outFile<<"SJF Algorithm \n";
	
	for(int i=0; i<50; i++)
	{
		meanT += pArr[i].turn;
		meanW += pArr[i].wait;
	if(i==0)
		{
		maxW=pArr[0].wait;  minW=pArr[0].wait;
		maxT=pArr[0].turn;  minT=pArr[0].turn;
		
		}
	else
		{
		if(pArr[i].wait<minW)
		minW = pArr[i].wait;
		
		if(pArr[i].wait>maxW)
		maxW = pArr[i].wait;
		
		if(pArr[i].turn<minT)
		minT = pArr[i].turn;
		
		if(pArr[i].turn>maxT)
		maxT = pArr[i].turn;
		}
	}
	
	meanT /= 50;  meanW /= 50;
	
	//calculate the standard deviation
	
	for(int i=0; i<50; i++)
	{
		std_devW[i] = pow((pArr[i].wait-meanW),2);
		S1 += std_devW[i];
		std_devT[i] = pow((pArr[i].turn-meanT),2);
		S2 += std_devT[i];
	}
	
	S1 /= 50;  S2 /= 50;
	S1=sqrt(S1);  S2=sqrt(S2);
	
	//display the result
	
	outFile << "\tWait  Turnaround \n"
			<< "Min   \t" << minW <<"\t"<<minT<<"\n"
			<< "Mean   \t" << meanW <<"\t"<<meanT<<"\n"
			<< "Max   \t" << maxW <<"\t"<<maxT<<"\n";
			
			outFile << fixed << showpoint << setprecision(2);
			
			outFile << "StdDev  " << S1 <<" \t"<<S2<<"\n";
			outFile << "-------------------------------- \n";
			
			FCFS();
	
	outFile<<"Round Robin Algorithm \n";

	
	for(int i=0; i<50; i++)
	{
		meanT += pArr[i].turn;
		meanW += pArr[i].wait;
	if(i==0)
		{
		maxW=pArr[0].wait;  minW=pArr[0].wait;
		maxT=pArr[0].turn;  minT=pArr[0].turn;
		
		}
	else
		{
		if(pArr[i].wait<minW)
		minW = pArr[i].wait;
		
		if(pArr[i].wait>maxW)
		maxW = pArr[i].wait;
		
		if(pArr[i].turn<minT)
		minT = pArr[i].turn;
		
		if(pArr[i].turn>maxT)
		maxT = pArr[i].turn;
		}
	}
	
	meanT /= 50;  meanW /= 50;
	
	//calculate the standard deviation
	
	for(int i=0; i<50; i++)
	{
		std_devW[i] = pow((pArr[i].wait-meanW),2);
		S1 += std_devW[i];
		std_devT[i] = pow((pArr[i].turn-meanT),2);
		S2 += std_devT[i];
	}
	
	S1 /= 50;  S2 /= 50;
	S1=sqrt(S1);  S2=sqrt(S2);
	
	//display the result
	
	outFile << "\tWait  Turnaround \n"
			<< "Min   \t" << minW <<"\t"<<minT<<"\n"
			<< "Mean   \t" << meanW <<"\t"<<meanT<<"\n"
			<< "Max   \t" << maxW <<"\t"<<maxT<<"\n";
			
			outFile << fixed << showpoint << setprecision(2);
			
			outFile << "StdDev  " << S1 <<" \t"<<S2<<"\n";
			outFile << "-------------------------------- \n";
			
}

	//determinig the suitable algorithm
	
void scheduling()
{
	
	if(Algorithm==1)
	FCFS();
	else if(Algorithm==2)
	priority();
	else if(Algorithm==3)
	SJF();
	else if(Algorithm==4)
	round_robin();
	else if(Algorithm==5)
	compare();
}


