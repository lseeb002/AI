#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

struct instance{
	float type;
	vector<float> features;
};

int main(int argc, char *argv[]){
	if (argc != 2){
		cout << "Incorrect # of args" << endl;
		return 0;
	}

	ifstream fin(argv[1]);
	if (!fin.is_open()){
		cout << "Could not open file" << endl;
		return 0;
	}
	string line;
	vector<instance> data;
	
	//Read input from file and create data vector
	while (getline(fin,line)){
		stringstream line_data(line);
		float temp;
		instance curr;
		line_data >> curr.type;
		while (line_data >> temp){
			curr.features.push_back(temp);
		}
		data.push_back(curr);
	}

	for (int i=0; i < data.size(); ++i){
		cout << i << ": ";
		cout << data.at(i).type << " ";
		for (int j=0; j < data.at(i).features.size(); ++j){
			cout << data.at(i).features.at(j) << " ";
		}
		cout << endl;
	}
	
	return 0;
}	

