#include "pch.h"
#include <queue>
#include <ctime>
#include <math.h>
#include <iostream>
#include <Windows.h>

#define BUSY 1
#define FREE 0

using namespace std;
clock_t _time = clock();
/*
Assignment 1£º

Multi-queue simulation
	  Description: a transmission system subject to 3 inputs and only one server. All inputs are characterized by Poisson process and the service time follows an exponential distribution.

	  Goal: explore a scheduling scheme which is able to allocate the service fairly to each of the input in terms of any performance measurement; obtaining the mean queue length, mean waiting time, queue length distribution and waiting time distribution for each queue. Any necessary explaination and statement should be included in the report.
*/

//generate possion distribution
int possion(double lambda)
{
	int i, x;
	double a, b, u;

	srand((int)clock());
	a = exp(-lambda);
	i = 0;
	b = 1.0;
	do {
		u = rand() / double(RAND_MAX);
		b *= u;
		i++;
	} while (b >= a);
	x = i - 1;
	return x;
}

double exponent(double lambda)
{
	srand((int)clock());
	double pV = 0.0;
	while (true)
	{
		pV = (double)rand() / (double)RAND_MAX;
		if (pV != 1)
		{
			break;
		}
	}
	pV = (-10000.0 / lambda)*log(1 - pV);
	return pV;
}


class packet
{
public:
	packet()
	{
		itv_time = (int)exponent(lambda);
		Sleep(itv_time);
		arv_time = clock();
		wat_time = (int)exponent(beta);
	}
	double get_arv_time() { return arv_time; }
	double get_wat_time() { return wat_time; }
	double get_sev_time() { return sev_time; }
	double get_itv_time() { return itv_time; }
	void set_sev_time(int t) { sev_time = t; }
private:
	double arv_time;
	double itv_time;//interval of arrive time
	double sev_time;//the time a packet get service
	double wat_time;//total serve time
	int lambda = 4;
	int beta = 8;
};

class server
{
public:
	bool free() { return state; }
	void serve(packet *p,int count)
	{ 
		pac = p; 
		p->set_sev_time(_time);
		cout << count << "is served now  " << p->get_sev_time() << "  wait time:  " << p->get_wat_time() << endl;
	}
	void change()
	{
		if (state == BUSY) pac = NULL;
		state = !state;
	}
	void check(queue<packet> &Q, int count)
	{
		if (pac == NULL) return; 
		_time = clock();
		if (_time - pac->get_sev_time() >= pac->get_wat_time() && state == BUSY)
		{//end service
			state = FREE;
			cout << count << "end serve  "<<endl;
		}
		if (state == FREE && !Q.empty())
		{//continue serve new packet
			packet p = Q.front();
			Q.pop();
			serve(&p,count);
		}
	}
	void set_pac(packet *p) { pac = p; }
	bool get_state() { return state; }
private:
	bool state;//busy:1 free:0
	packet *pac;
};

void Arrive(queue<packet> &Q, server &s,int count)
{
	packet p;
	if (!Q.empty())
	{
		Q.push(p);
		cout << count << "  enter queue"
			<< "  arrive time:  " << p.get_arv_time()
			<< "  interval time:  " << p.get_itv_time()
			<< endl;
		cout << "Queue size: " << Q.size() << endl;
	}
	else
	{
		if (s.free())
		{
			s.serve(&p,count);
			cout << count << "  on serve  "<<endl;
			s.change();//server change state
		}
		else
		{
			Q.push(p);
			cout << count << "  enter queue(queue empty and server busy)  " 
				<<"arrive time:  "<< p.get_arv_time() 
				<< "  interval time:" << p.get_itv_time() 
				<< endl;
			cout << "Queue size: " << Q.size() << endl;

		}
	}
}

int main()
{
	int pac_num = 10;//packet number
	server s;
	queue<packet> Q;
	int count = 0;

	Arrive(Q, s, count);//renew the packet data

	while (!s.free() || !Q.empty())
	{
		count++;
		if (count < 10)
		{
			Arrive(Q, s, count);
			s.check(Q, count);
		}		

		s.check(Q, count);
		//cout << "s state: " << s.get_state() << endl;
		//cout << Q.size() << endl;
	}


	//Depart(&s);
	//clear a packet from queue
	
	getchar();
	return 0;
}