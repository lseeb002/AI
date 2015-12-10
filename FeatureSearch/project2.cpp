#include <iostream>
#include <cmath>
#include <set>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <time.h>
using namespace std;


struct instance{
	float type;
	vector<float> features;
};

double calcdist(instance a, instance b, set<int>currFeats){
	double dist = 0.0;
	for(auto it : currFeats){
		dist += pow((a.features.at(it)-b.features.at(it)), 2.0);
	}
	return sqrt(dist);
}

double validation(vector<instance>data, set<int>currFeats, int feat_to_add){
	/*Random Number to Test
	double acc = rand()%100+1;
	cout << "Accuracy: " << acc << endl
	return acc; */
	int nn = 0;
	double accuracy, curr = 0;
	double bestdist = 100.0;
	currFeats.insert(feat_to_add);
	cout << "Features currently testing: {";
	for (auto it : currFeats){
		cout << it+1 << ",";
	}
	cout << "}" << endl;
	for(int i=0; i<data.size(); ++i){
		for(int j=0; j<data.size(); ++j){
			if( i != j ){
				curr = calcdist(data.at(i),data.at(j),currFeats);
				if(curr < bestdist){
					nn = j;
					bestdist = curr;
				}
			}
		}
		if (data.at(nn).type == data.at(i).type)
			++accuracy;
	}
	return accuracy/data.size();
}

int main(int argc, char *argv[]){

	//srand(time(0));
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
	set<int> currFeatures;
	set<int> bestFeatures;
	int numFeatures = 0;
	double bestAccuracy = 0.0;
	
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
	
	numFeatures = data.front().features.size();
	for (int i=0; i<numFeatures; ++i){
		cout << "On the " << i+1 << "th level of the search tree" 
			<< endl;
		int feature_to_add;
		double best_sofar_accuracy = 0.0;
		
		for (int j=0; j<numFeatures; ++j){
			if (currFeatures.count(j) == 0){
				cout << "Considering adding the " << j+1 
					<< " feature" << endl;
				double currAccuracy = validation(data,currFeatures,j);
				cout << "Accuracy with new feature: " 
					<< currAccuracy << endl;
				if (currAccuracy >= best_sofar_accuracy){
					best_sofar_accuracy = currAccuracy;
					feature_to_add = j;
				}
			}
		}
		
		if (feature_to_add >= 0){
			currFeatures.insert(feature_to_add);
			cout << "On level " << i+1 << " added feature " 
				<< feature_to_add+1 << " to current set" << endl;
		}
		if (best_sofar_accuracy >= bestAccuracy){
                	bestAccuracy = best_sofar_accuracy;
                	bestFeatures = currFeatures;
                }

		cout << "Best Features: {";
		for (auto it : bestFeatures){
			cout << it+1 << ",";
		} cout << "}" << endl;
	}	
	
	/*Print Data For Testing
	for (int i=0; i < data.size(); ++i){
		cout << i << ": ";
		cout << data.at(i).type << " ";
		for (int j=0; j < data.at(i).features.size(); ++j){
			cout << data.at(i).features.at(j) << " ";
		}
		cout << endl;
	}
	*/
	return 0;
}	

