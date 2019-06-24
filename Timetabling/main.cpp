#include <iostream>
#include <fstream>
#include "Timetabling.h"

using namespace std;

const string dir = "D:/Victor/Pos-Graduacao/UFV/2 periodo/INF682/Trabalhos/02/Instances_ORLibrary_Timetabling/";

void generate() {
	Timetabling::generateInstances(dir, 50, 100, 6, 8, 2, 6, 50);
	Timetabling::generateInstances(dir, 50, 200, 6, 8, 2, 6, 50);
	Timetabling::generateInstances(dir, 100, 200, 6, 8, 2, 6, 50);
	Timetabling::generateInstances(dir, 100, 400, 6, 8, 2, 6, 50);
	Timetabling::generateInstances(dir, 200, 800, 6, 8, 2, 6, 50);
	Timetabling::generateInstances(dir, 200, 400, 6, 8, 2, 6, 50);
	Timetabling::generateInstances(dir, 400, 800, 6, 8, 2, 6, 50);
	Timetabling::generateInstances(dir, 400, 1600, 6, 8, 2, 6, 50);
	Timetabling::generateInstances(dir, 134, 110, 6, 8, 2, 6, 1); // ufv florestal
	Timetabling::generateInstances(dir, 134, 104, 6, 8, 2, 6, 1); // ufv paranaiba
	Timetabling::generateInstances(dir, 1018, 778, 6, 8, 2, 6, 1); // ufv vicosa
	Timetabling::generateInstances(dir, 1286, 993, 6, 8, 2, 6, 1); // ufv
}

void run() {
	fstream file;
	file.open("all.txt");
	if (file.is_open() == false) {
		cout << "Error opening file all.txt\n";
		exit(1);
	}
	string f;
	while (getline(file, f)) {
		try {
			Timetabling t(dir + f);
			t.model();
		}
		catch (exception e) {
			cout << "Error executing model with instance " << f << endl;
			cout << e.what() << endl;
		}
	}
}

void test() {
	Timetabling t(dir, "test01.txt");//"stt_50_100_6_8_2_6_50.txt");
	t.printData();
	t.model();
}

int main() {
	//Timetabling::generateInstances(dir, 1286, 993, 6, 8, 2, 6, 1);
	//generate();
	//test();
	run();
	return 0;
}
