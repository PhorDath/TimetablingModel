#include "Timetabling.h"

Timetabling::Timetabling(string fileName)
{
	this->directory = "";
	this->fileName = fileName;
	readInstance();
}

Timetabling::Timetabling(string directory, string fileName)
{
	this->directory = directory;
	this->fileName = fileName;
	readInstance();
}

void Timetabling::model()
{
	GRBEnv env = GRBEnv(true);
	env.start();

	try {
		GRBModel model = GRBModel(env);
		model.set(GRB_StringAttr_ModelName, "LRP_" + fileName);

		
		var_x(model);
		fo(model);
		c1(model);
		c2(model);
		c3(model);
		c4(model);

		model.write("teste.lp");
		model.getEnv().set(GRB_DoubleParam_TimeLimit, TMAX);
		cout << "Solving Timetabling with input " << fileName << endl;
		model.optimize();

		getInfo(model);
		result(model);
		model.write("teste.sol");
		getData(model);
		getCSV(model);
		getLatex(model);
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
}

void Timetabling::printData()
{
	cout << P << " " << T << " " << D << " " << H << " " << q << " " << cred << " " << prob;
	cout << endl << endl;
	for (int i = 0; i < P; i++) {
		//file << "Professor " << i << ": \n";
		for (int k = 0; k < D; k++) {
			//file << "Day " << k << ": \n";
			for (int l = 0; l < H; l++) {
				//file << "Hour " << l << ": ";
				//file << rand() % 11 << "\n";
				cout << c.at(i).at(k).at(l) << " ";
			}
			cout << endl;
		}
		//file << endl;
	}
	//file << "CH: \n";
	cout << endl;
	for (int i = 0; i < P; i++) {
		for (int j = 0; j < T; j++) {
			cout << ch.at(i).at(j) << " ";
		}
		cout << endl;
	}
}

Timetabling::~Timetabling()
{
}

void Timetabling::generateInstances(string dir, int P, int T, int D, int H, int q, int cred , float prob)
{
	fstream all;
	all.open(dir + "all.txt", ios::out | ios::app);
	if (all.is_open() == false) {
		cout << "Error opening all.txt\n";
		exit(1);
	}

	fstream file;
	string fn = "stt_" + to_string(P) + "_" + to_string(T) + "_" + to_string(D) + "_" + to_string(H) + "_" + to_string(q) + "_" + to_string(cred) + "_" + to_string(int(prob)) + ".txt";
	all << fn << endl;
	file.open(dir + fn, ios::out | ios::ate);
	if (file.is_open() != true) {
		cout << "Error creating file " + fn << endl;
		exit(1);
	}
	file << P << " " << T << " " << D << " " << H << " " << q << " " << cred << " " << prob << endl;
	file << endl;
	srand(time(NULL));
	for (int i = 0; i < P; i++) {
		//file << "Professor " << i << ": \n";
		for (int k = 0; k < D; k++) {
			//file << "Day " << k << ": \n";
			for (int l = 0; l < H; l++) {
				//file << "Hour " << l << ": ";
				//file << rand() % 11 << "\n";
				file << rand() % 11 << " ";
			}
			file << endl;
		}
		//file << endl;
	}
	//file << "CH: \n";
	file << endl;
	for (int i = 0; i < P; i++) {
		for (int j = 0; j < T; j++) {
			if (rand() % 100 <= prob) {
				file << rand() % cred + 1 << " ";
			}
			else {
				file << 0 << " ";
			}
		}
		file << endl;
	}
	file.close();
	all.close();
}

void Timetabling::readInstance()
{
	fstream file;
	cout << directory + fileName << endl;
	file.open(directory + fileName, ios::in);
	if (file.is_open() == false) {
		cout << "Error opening file " << fileName << endl;
		cout << "On directory " << directory << endl;
		exit(1);
	}
	file >> P >> T >> D >> H >> q >> cred >> prob;
	c.resize(P);
	for (int i = 0; i < P; i++) {
		c.at(i).resize(D);
		for (int k = 0; k < D; k++) {
			c.at(i).at(k).resize(H);
		}
	}
	for (int i = 0; i < P; i++) {
		for (int k = 0; k < D; k++) {
			for (int l = 0; l < H; l++) {
				file >> c.at(i).at(k).at(l);
			}
		}
	}
	ch.resize(P);
	for (int i = 0; i < P; i++) {
		ch.at(i).resize(T);
	}
	for (int i = 0; i < P; i++) {
		for (int j = 0; j < T; j++) {
			file >> ch.at(i).at(j);
		}
	}
}

void Timetabling::result(GRBModel & model)
{
	int status = model.get(GRB_IntAttr_Status);
	if (status == GRB_UNBOUNDED) {
		cout << "O modelo nao pode ser resolvido porque e ilimitado." << endl;
		feaseble = false;
	}
	if (status == GRB_OPTIMAL) {
		cout << "Solucao otima encontrada!" << endl;
		cout << "O valor da solucao otima eh: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
		feaseble = true;
	}
	if (status == GRB_TIME_LIMIT) {
		cout << "Tempo limite!" << endl;
		cout << "O valor da melhot solucao ate o momento e: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
		feaseble = true;
	}
	if (status == GRB_INFEASIBLE) {
		cout << "O modelo nao pode ser resolvido porque e inviavel." << endl;
		feaseble = false;
	}
	else {
		cout << "Status: " << status << endl;
	}
}

void Timetabling::getData(GRBModel & model)
{
	fstream file;
	file.open(directory + fileName + ".sol", ios::out | ios::ate);
	if (file.is_open() == false) {
		cout << "Error opening file " << fileName << endl;
		cout << "On directory " << directory << endl;
		exit(1);
	}
	file << "fo: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
	file << "gap: " << model.get(GRB_DoubleAttr_MIPGap) * 100 << endl;
	file << "time: " << model.get(GRB_DoubleAttr_Runtime) << endl;
	for (int i = 0; i < P; i++) {
		file << "Professor " << i+1 << ": " << endl;
		for (int k = 0; k < D; k++) {
			file << "Day " << k+1 << ": " << endl;
			for (int j = 0; j < T; j++) {
				for (int l = 0; l < H; l++) {
					int value = x.at(i).at(j).at(k).at(l).get(GRB_DoubleAttr_X);
					if (value == 1) {
						file << l+1 << " - " << j+1 << endl;
					}
				}
			}
		}
	}
	
}

void Timetabling::getCSV(GRBModel & model)
{
	fstream file;
	file.open(directory + fileName + ".csv", ios::out | ios::ate);
	if (file.is_open() == false) {
		cout << "Error opening file " << fileName << endl;
		cout << "On directory " << directory << endl;
		exit(1);
	}
	file << "fo: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
	for (int i = 0; i < P; i++) {
		file << "Professor " << i + 1 << endl;
		file << "hour | day ; ";
		for (int k = 0; k < D; k++) {
			file << "D" << k + 1 << " ; ";
		}
		file << endl;
		for (int l = 0; l < H; l++) {
			file << "H" << l+1 << " ; ";
			for (int k = 0; k < D; k++) {
				int w = 0;
				int j = 0;
				for (j = 0; j < T; j++) { 
					w = x.at(i).at(j).at(k).at(l).get(GRB_DoubleAttr_X);
					if (w == 1) {
						break;
					}
				}
				if (w == 0) {
					file << " - ; ";
				}
				else if (w == 1) {
					file << "T" << j + 1 << " ; ";
				}
			}
			file << endl;
		}
	}
	file.close();
}

void Timetabling::getLatex(GRBModel & model)
{
	fstream file;
	file.open(directory + fileName + ".tex", ios::out | ios::ate);
	if (file.is_open() == false) {
		cout << "Error opening file " << fileName << endl;
		cout << "On directory " << directory << endl;
		exit(1);
	}
	file << "fo: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
	file << "\t\\begin{table}[H]\n";
	file << "\t\t\\centering\n";
	file << "\t\t\\begin{tabular}{|";
	for (int i = 0; i < D + 1; i++) {
		file << "c|";
	}
	file << "}\n";
	file << "\t\t\t\\hline\n";
	for (int i = 0; i < P; i++) {
		file << "\t\t\t\\multicolumn{" << D+1 <<"}{|c|}{Professor " << i+1 << "} \\\\ \\hline \n"; //file << "Professor " << i + 1 << endl;
		file << "\t\t\tHorário / Dia & ";
		for (int k = 0; k < D; k++) {
			if(k == D-1)
				file << "D" << k + 1 << " ";
			else
				file << "D" << k + 1 << " & ";
		}
		file << "\\\\ \\hline" << endl;
		for (int l = 0; l < H; l++) {
			file << "\t\t\tH" << l + 1 << " & ";
			for (int k = 0; k < D; k++) {
				int w = 0;
				int j = 0;
				for (j = 0; j < T; j++) {
					w = x.at(i).at(j).at(k).at(l).get(GRB_DoubleAttr_X);
					if (w == 1) {
						break;
					}
				}
				if (k < D - 1) {
					if (w == 0) {

						file << " - & ";
					}
					else if (w == 1) {
						file << "T" << j + 1 << " & ";
					}
				}
				else {
					if (w == 0) {

						file << " -";
					}
					else if (w == 1) {
						file << "T" << j + 1 << "";
					}
				}
				
			}
			file << "\\\\ \\hline" << endl;
		}
		//file << "\\\\ \\hline" << endl;
	}
	file << "\t\t\\end{tabular}\n";
	file << "\t\t\\caption{Resultados para a instância " + fileName +"}\n";
	file << "\t\t\\label{tab:sol_" << fileName << "}\n";
	file << "\t\\end{table}\n";
	file.close();
}

void Timetabling::getInfo(GRBModel & model)
{
	string ret = "";
	ret += to_string(model.get(GRB_DoubleAttr_ObjVal)) + " ; ";
	ret += to_string(model.get(GRB_DoubleAttr_MIPGap) * 100) + " ; ";
	ret += to_string(model.get(GRB_DoubleAttr_Runtime));
	info = ret;
}

void Timetabling::var_x(GRBModel & model)
{
	cout << "Adding var x\n";
	x.resize(P);
	for (int i = 0; i < P; i++) {
		x.at(i).resize(T);
		for (int j = 0; j < T; j++) {
			x.at(i).at(j).resize(D);
			for (int k = 0; k < D; k++) {
				x.at(i).at(j).at(k).resize(H);
			}
		}
	}
	for (int i = 0; i < P; i++) {
		for (int j = 0; j < T; j++) {
			for (int k = 0; k < D; k++) {
				for (int l = 0; l < H; l++) {
					x.at(i).at(j).at(k).at(l) = model.addVar(0, 1, 0, GRB_BINARY, "x(" + to_string(i) + "," + to_string(j) + "," + to_string(k) + "," + to_string(l) + ")");
				}
			}
		}
	}
	model.update();
}

void Timetabling::fo(GRBModel & model)
{
	cout << "Adding fo\n";
	GRBLinExpr fo{ 0 };
	for (int i = 0; i < P; i++) {
		for (int j = 0; j < T; j++) {
			for (int k = 0; k < D; k++) {
				for (int l = 0; l < H; l++) {
					fo += c.at(i).at(k).at(l) * x.at(i).at(j).at(k).at(l);
				}
			}
		}
	}
	model.setObjective(fo, GRB_MAXIMIZE);
	model.update();
}

void Timetabling::c1(GRBModel & model)
{
	cout << "Adding c1\n";
	for (int i = 0; i < P; i++) {
		for (int k = 0; k < D; k++) {
			for (int l = 0; l < H; l++) {
				GRBLinExpr c1{ 0 };
				for (int j = 0; j < T; j++) {
					c1 += x.at(i).at(j).at(k).at(l);
				}
				model.addConstr(c1 <= 1, "c1(" + to_string(i) + "," + to_string(k) + "," + to_string(l) + ")");
			}
		}
		
	}
	model.update();
}

void Timetabling::c2(GRBModel & model)
{
	cout << "Adding c2\n";
	for (int j = 0; j < T; j++) {
		for (int k = 0; k < D; k++) {
			for (int l = 0; l < H; l++) {			
				GRBLinExpr c2{ 0 };
				for (int i = 0; i < P; i++) {
					c2 += x.at(i).at(j).at(k).at(l);
				}
				model.addConstr(c2 <= 1, "c2(" + to_string(j) + "," + to_string(k) + "," + to_string(l) + ")");
			}
		}

	}
	model.update();
}

void Timetabling::c3(GRBModel & model)
{
	cout << "Adding c3\n";
	for (int i = 0; i < P; i++) {
		for (int j = 0; j < T; j++) {
			GRBLinExpr c3{ 0 };
			for (int k = 0; k < D; k++) {
				for (int l = 0; l < H; l++) {
					c3 += x.at(i).at(j).at(k).at(l);
				}				
			}
			model.addConstr(c3 == ch.at(i).at(j), "c3(" + to_string(i) + "," + to_string(j) + ")");
		}

	}
	model.update();
}

void Timetabling::c4(GRBModel & model)
{
	cout << "Adding c4\n";
	for (int i = 0; i < P; i++) {
		for (int j = 0; j < T; j++) {
			for (int k = 0; k < D; k++) {
				GRBLinExpr c4{ 0 };
				for (int l = 0; l < H; l++) {
					c4 += x.at(i).at(j).at(k).at(l);
				}
				model.addConstr(c4 <= q, "c4(" + to_string(i) + "," + to_string(j) + "," + to_string(k) + ")");
			}
		}
	}
	model.update();
}


