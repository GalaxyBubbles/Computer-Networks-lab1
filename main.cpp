// EE 465, USC
// Simulation of an M/M/1 system

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <time.h>


using namespace std;

#define INFIN  			999999999

// Function Name:	expon
// Description: 	Generates an exponentially distributed random number 
//					with parameter \lambda. 
// Input: 			lambda (double)
// Output: 			An exponentially distributed random number (double)
//
double expon(double lambda)
{
	double u; // used to store a random number. 
	srand((int)clock());
	do
	{
		u = rand() % 10000 / 10000.0; //uniform number in the interval [0.0, 1.0]
	} while ((u == 0) || (u == 1)); //special cases that we want to avoid

	return -log(1 - u) / lambda;

}

// Function Name:	print_stats
// Description: 	Saves and prints system statistics 
// Input:           stats_file (ostream object): ostream object for the stats file
//					avg_customers (double): average customers in the system
//					avg_service_time (double): average service time 
// Output: 			void (output stored to file and printed in the screen)
//
void print_stats(ostream &stats_file, double* wait_time,int tot_arrival)
{
	for (int i = 0; i < tot_arrival; i++)
	{
		stats_file << wait_time[i] << endl;
	}
}

// The main function of the program that is called at the beginning.
int main() {
	//system variables
	long int tot_arrivals, cur_arrivals = 0;
	double lambda, mu;
	double event1 = 0.0, event2 = INFIN; //event1: time of next arrival, event2: time of next departure
	double service_time, cur_time = 0.0;
	int customers = 0;
	double avg_customers = 0.0, avg_service_time = 0.0; //used for calculating statistics
	ofstream stats_file; //file handler for saving the statistics in a file

	//get input from the user
	cout << "Basic M/M/1 Simulator" << endl;
	cout << "Enter the total number of arrivals: ";
	cin >> tot_arrivals;
	cout << "Enter lambda: ";
	cin >> lambda;
	cout << "Enter mu: ";
	cin >> mu;

	stats_file.open("stats.txt"); //it creates the file, if it does not exist

	double *wait_time = new double[tot_arrivals];
	double *arv_time = new double[tot_arrivals];

	for (int i = 0; i < tot_arrivals; i++)
	{
		wait_time[i] = 0;
		arv_time[i] = 0;
	}

	int cur_serve = 0;

	// discrete event simulator 
	while (cur_arrivals < tot_arrivals)
	{
		cout << "cur_arrivals: " << cur_arrivals << endl;
		if (event1 < event2)	//arrival
		{
			cur_arrivals++;
			avg_customers += customers;
			customers++;
			cur_time = event1;
			arv_time[cur_arrivals] = event1;
			event1 = cur_time + expon(lambda);
			if (customers == 1)	//if there is only 1 customer, he/she goes directly to service
			{
				cur_serve = cur_arrivals;
				service_time = expon(mu);
				wait_time[cur_arrivals] = service_time;
				cout << cur_serve << "  " << wait_time[cur_arrivals] << endl;
				event2 = cur_time + service_time;
				avg_service_time += service_time;
			}
		}
		else					//departure
		{
			customers--;
			cur_time = event2;//server free
			if (customers > 0) // the departure left a non-empty queue behind
			{
				cur_serve++;
				service_time = expon(mu);
				event2 = cur_time + service_time;
				avg_service_time += service_time;
				wait_time[cur_serve] = cur_time - arv_time[cur_serve];
				cout << cur_serve << "  " << wait_time[cur_arrivals] << endl;
			}
			else 				//the departure left the queue empty
			{
				event2 = INFIN; //no departure scheduled
			}
		}
	}
	avg_customers /= cur_arrivals;
	avg_service_time /= (cur_arrivals - customers);
	//print_stats(stats_file, avg_customers, avg_service_time);
	//stats_file.close();
	for (int i = 1; i < tot_arrivals; i++)
		cout << i << " wait time: " << wait_time[i] << endl;
	print_stats(stats_file, wait_time, tot_arrivals);
	stats_file.close();
	int a;
	cin >> a;
	return 0;
}