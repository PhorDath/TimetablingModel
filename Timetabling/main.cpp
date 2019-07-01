#include <iostream>
#include <fstream>
#include "Timetabling.h"

using namespace std;

const string dir = "D:/Victor/Pos-Graduacao/UFV/2 periodo/INF682/Trabalhos/02/Instances_ORLibrary_Timetabling/";

void generate() {
	fstream all;
	all.open(dir + "all.txt", ios::out | ios::ate);
	if (all.is_open() == false) {
		cout << "Error cleaning all.txt\n";
		exit(1);
	}
	all.close();
	Timetabling::generateInstances(dir, 5, 10, 6, 8, 4, 6, 10);
	Timetabling::generateInstances(dir, 15, 30, 6, 8, 4, 6, 10);
	Timetabling::generateInstances(dir, 30, 30, 6, 8, 4, 6, 10);
	Timetabling::generateInstances(dir, 45, 30, 6, 8, 6, 6, 10);
	Timetabling::generateInstances(dir, 50, 60, 6, 8, 6, 6, 10);
	Timetabling::generateInstances(dir, 60, 30, 6, 8, 6, 6, 10);
	Timetabling::generateInstances(dir, 134, 110, 6, 8, 6, 6, 20); // ufv florestal
	Timetabling::generateInstances(dir, 134, 104, 6, 8, 6, 6, 20); // ufv paranaiba
	Timetabling::generateInstances(dir, 1018, 778, 6, 8, 6, 6, 20); // ufv vicosa
	Timetabling::generateInstances(dir, 1286, 993, 6, 8, 6, 6, 20); // ufv
}

void run() {
	fstream file;
	file.open(dir + "all.txt");
	if (file.is_open() == false) {
		cout << "Error opening file all.txt\n";
		exit(1);
	}

	fstream solution;
	solution.open(dir + "solution.csv", ios::out | ios::ate);
	if (solution.is_open() == false) {
		cout << "Error opening solution.csv\n";
		exit(1);
	}
	solution << "Instncia ; FO ; Gap (%) ; Tempo (s) \n";
	string f;
	int cont = 0;
	while (getline(file, f)) {
		try {
			Timetabling t(dir, f);
			t.model();
			if (t.feaseble == true) {
			}
			else {
				//break;
			}
			//cout << t.info << endl;
			solution << f << " ; ";
			solution << t.info;
			solution << endl;
		}
		catch (exception e) {
			cout << "Error executing model with instance " << f << endl;
			cout << e.what() << endl;
		}
	}
	solution.close();
}

void test() {
	Timetabling t(dir, "stt_15_30_6_8_4_6_10.txt");//"stt_50_100_6_8_2_6_50.txt");
	t.printData();
	t.model();
}

int main() {
	int op;
	cin >> op;
	if (op == 1)
		run();
	if(op == 2)
		generate();
	if (op == 3)
		test();
	return 0;
}
