#pragma once
#include <fstream>
#include <stdio.h> 
#include <time.h>
#include <stdlib.h> 
#include <gurobi_c++.h>

#define TMAX 3600

using namespace std;

class Timetabling
{
private:
	string directory;
	string fileName;
	int cred;
	float prob;

	int P, T, D, H;
	int q;
	vector<vector<vector<int>>> c;
	vector<vector<int>> ch;

	void readInstance();
	void result(GRBModel &model);
	void getData(GRBModel &model);
	void getCSV(GRBModel &model);
	void getLatex(GRBModel &model);

	// decision variables
	vector<vector<vector<vector<GRBVar>>>> x;

	void var_x(GRBModel &model);
	void fo(GRBModel &model);
	void c1(GRBModel &model);
	void c2(GRBModel &model);
	void c3(GRBModel &model);
	void c4(GRBModel &model);

public:
	Timetabling(string fileName);
	Timetabling(string directory, string fileName);
	void model();
	void printData();
	static void generateInstances(string dir, int P, int T, int D, int H, int q, int cred, float prob);
	~Timetabling();
};

