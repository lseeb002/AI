#include <iostream>
#include <set>
#include <cmath>
#include <vector>
#include <list>
#include <queue>
#include <iomanip>
#include <utility>
using namespace std;

struct Puzzle {
	vector<vector<int> > p; //puzzle vector (3x3)
	int g_n; //steps taken
	int h_n; //heuristic
	int f_n; //A* heuristic (g_n + h_n)
	pair<int,int> blank; //position of blank (row, column)

	Puzzle(const vector<vector<int> >& vec, int g, int h, int f, pair<int,int> b)
		: p(vec), g_n(g), h_n(h), f_n(f), blank(b) {}

	Puzzle(const Puzzle& puz)
		: p(puz.p), g_n(puz.g_n), h_n(puz.h_n), f_n(puz.f_n), 
		  blank(puz.blank) {}
};

class Compare {
	public:
		bool operator()(Puzzle& p1, Puzzle& p2)
		{
			if (p1.f_n > p2.f_n) return true;
			return false;
		}
};

void printVec(const vector<vector<int> >& vec)
{
	int sz = vec.size();
	for(int i = 0; i < sz; ++i)
	{
		for(int j = 0; j < sz; ++j)
		{
			cout << setw(3) << vec.at(j).at(i) << " ";
		}
		cout << endl;
	}
}

int calcMisplaced(vector<vector<int> >& v)
{
	vector<vector<int> > g = {{1,4,7},{2,5,8},{3,6,0}};
	int sz = v.size();
	int h = 0; //heuristic value to be returned
	for(int i = 0; i < sz; ++i)
	{
		for(int j = 0; j < sz; ++j)
		{
			if(v.at(i).at(j) != g.at(i).at(j))
			{
				++h;
			}
		}
	}
	return h;
}

int calcManhat(vector<vector<int> >& v)
{
	int sz = v.size();
	int h = 0; //heuristic value to be returned
	int num = -1;
	for(int i = 0; i < sz; ++i)
	{
		for(int j = 0; j < sz; ++j)
		{
			num = v.at(j).at(i);
			switch(num) {
				case 1:
					h += i + j; break;
				case 2:
					h += i + abs(1-j); break;
				case 3:
					h += i + abs(2-j); break;
				case 4:
					h += abs(1-i) + j; break;
				case 5:
					h += abs(1-i) + abs(1-j); break;
				case 6:
					h += abs(1-i) + abs(2-j); break;
				case 7:
					h += abs(2-i) + j; break;
				case 8:
					h += abs(2-i) + abs(1-j); break;
				default:
					h += abs(2-i) + abs(2-j); break;
			}
		}
	}
	return h;
}

void expand_left(Puzzle node, priority_queue<Puzzle,vector<Puzzle>,Compare>& pq,
     int alg, set<vector<vector<int>>>& states)
{
	set<vector<vector<int>>>::iterator it;
	int i = node.blank.first;  //row of zero
	int j = node.blank.second; //column of zero
	if (j == 0)
	{
		return;
	}
	swap(node.p.at(j-1).at(i), node.p.at(j).at(i));
	if((it = states.find(node.p)) != states.end()) return;
	else states.insert(node.p);
	if(alg == 2) node.h_n = calcMisplaced(node.p);
	else if(alg == 3) node.h_n = calcManhat(node.p);
	++(node.g_n);
	node.f_n = node.g_n + node.h_n;
	--(node.blank.second);
	pq.push(node);
}

void expand_right(Puzzle node, priority_queue<Puzzle,vector<Puzzle>,Compare>& pq,
     int alg, set<vector<vector<int>>>& states)
{
	set<vector<vector<int>>>::iterator it;
	int i = node.blank.first;  //row of zero
	int j = node.blank.second; //column of zero
	if (j == 2)
	{
		return;
	}
	swap(node.p.at(j+1).at(i), node.p.at(j).at(i));
	if((it = states.find(node.p)) != states.end()) return;
	else states.insert(node.p);
	if(alg == 2) node.h_n = calcMisplaced(node.p);
	else if(alg == 3) node.h_n = calcManhat(node.p);
	++(node.g_n);
	node.f_n = node.g_n + node.h_n;
	++(node.blank.second);
	pq.push(node);
}

void expand_up(Puzzle node, priority_queue<Puzzle,vector<Puzzle>,Compare>& pq,
     int alg, set<vector<vector<int>>>& states)
{
	set<vector<vector<int>>>::iterator it;
	int i = node.blank.first;  //row of zero
	int j = node.blank.second; //column of zero
	if (i == 0) return;
	swap(node.p.at(j).at(i-1), node.p.at(j).at(i));
	if((it = states.find(node.p)) != states.end()) return;
	else states.insert(node.p);
	if(alg == 2) node.h_n = calcMisplaced(node.p);
	else if(alg == 3) node.h_n = calcManhat(node.p);
	++(node.g_n);
	node.f_n = node.g_n + node.h_n;
	--(node.blank.first);
	pq.push(node);
}

void expand_down(Puzzle node, priority_queue<Puzzle,vector<Puzzle>,Compare>& pq,
     int alg, set<vector<vector<int>>>& states)
{
	set<vector<vector<int>>>::iterator it;
	int i = node.blank.first;  //row of zero
	int j = node.blank.second; //column of zero
	if (i == 2) return;
	swap(node.p.at(j).at(i+1), node.p.at(j).at(i));
	if((it = states.find(node.p)) != states.end()) return;
	else states.insert(node.p);
	if(alg == 2) node.h_n = calcMisplaced(node.p);
	else if(alg == 3) node.h_n = calcManhat(node.p);
	++(node.g_n);
	node.f_n = node.g_n + node.h_n;
	++(node.blank.first);
	pq.push(node);
}


int main() {
	int algorithm = 0;
	int puzChoice = 0;
	pair<int,int> ublank;
	int maxqsize = 0;
	int totalNodes = 0;
	set<vector<vector<int> > > repeats;

	//Vectors are initialized by columns
	vector<vector<int> > goalvec = { {1, 4, 7},    // |1 2 3|
		                         {2, 5, 8},    // |4 5 6|
					 {3, 6, 0} };  // |7 8 0|
	
	vector<vector<int> > myvec = { {1, 4, 7},      // |1 2 3|
		                       {2, 8, 6},      // |4 8 0|
				       {3, 0, 5} };    // |5 6 7|

	vector<vector<int> > uservec(3, vector<int>());


	cout << "Welcome to Luis Seeback's 8-puzzle solver." << endl;
	cout << "Type \"1\" to use a default puzzle, or \"2\" to enter your own" 
		<< " puzzle." << endl;

	cin >> puzChoice;
	
	if (puzChoice == 2)
	{
		cout << "Enter your puzzle, use a zero to represent the blank" << endl;
		for(int j = 0; j < 3; ++j)
		{
			if (j == 0)
			{
				cout << "Enter the first row, use a space or tab between numbers: ";
			}
			else if (j == 1)
			{
				cout << "Enter the second row, use a space or tab between numbers: ";
			}
			else
			{
				cout << "Enter the third row, use a space or tab between numbers: ";
			}

			for(int i = 0; i < 3; ++i)
			{
				int userTemp = -1;
				cin >> userTemp;
				if (userTemp == 0)
				{
					ublank.first = j;
					ublank.second = i;
				}
				uservec.at(i).push_back(userTemp);
			}
		}	
		if(uservec == goalvec) cout << "same" << endl;
		else cout << "not same" << endl;
	}

	//Choose an algorithm
	cout << "Enter your choice of algorithm: " << endl;
	cout << "\t1.  Uniform Cost Search" << endl;
	cout << "\t2.  A* with the Misplaced Tile heuristic" << endl;
	cout << "\t3.  A* with the Manhattan Distance heuristic" << endl;
	cin >> algorithm;

	//Initialize puzzle
	priority_queue<Puzzle, vector<Puzzle>, Compare> nodes_q;
	if(puzChoice == 1)
	{
		Puzzle myPuzz(myvec, 0, 0, 0, make_pair(1,2));
		if(algorithm == 2)
		{
			myPuzz.h_n = calcMisplaced(myvec);
		}
		else if(algorithm == 3)
		{
			myPuzz.h_n = calcManhat(myvec);
		}
		myPuzz.f_n = myPuzz.h_n;
		nodes_q.push(myPuzz);
	}
	else
	{
		Puzzle userPuzz(uservec, 0, 0, 0, ublank);
	        if(algorithm == 2)                                 	
                {
                	userPuzz.h_n = calcMisplaced(uservec);
                }
                else if(algorithm == 3)
                {
                	userPuzz.h_n = calcManhat(uservec);
                }
		userPuzz.f_n = userPuzz.h_n;
		nodes_q.push(userPuzz);
	}

	//Begin Algorithm
	cout << "Begin Algorithm: " << endl;
	for(;;)
	{
		if (nodes_q.empty())
		{
			cout << "No solution!" << endl;
			return 0;
		}
		Puzzle curr = nodes_q.top();
		nodes_q.pop();
		if (curr.p == goalvec)
		{
			cout << "Success!" << endl;
			printVec(curr.p);
			cout << "To solve this problem the search algorithm "
				<< "expanded a total of " << totalNodes
				<< " nodes" << endl;
			cout << "The maximum number of nodes in the queue at "
				<< "any one time was " << maxqsize << endl;
			cout << "The depth of the goal node was " << curr.g_n
				<< endl;
			return 0;
		}
		cout << "Expanding state with g(n)=" << curr.g_n 
		       << " and h(n)=" << curr.h_n << endl;
		printVec(curr.p);

		//Queing
		expand_left(curr, nodes_q, algorithm, repeats);
		expand_up(curr, nodes_q, algorithm, repeats);
		expand_right(curr, nodes_q, algorithm, repeats);
		expand_down(curr, nodes_q, algorithm, repeats);
		++totalNodes;
		if (nodes_q.size() > maxqsize)
		{
			maxqsize = nodes_q.size();
		}
	}

	
	return 0;
}
	
