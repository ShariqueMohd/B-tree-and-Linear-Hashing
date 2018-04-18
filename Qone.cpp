#include "BPlusTree.hpp"

using namespace std;

int main(int argc, char* argv[]) {
	string filename = argv[1];
	int M = atoi(argv[2]);
	int B = atoi(argv[3]);

	ifstream inputFile;
	string query;

	//cout << "Check.......\n";
	inputFile.open(filename);

	BPlusTree<int> tree((B+4)/12);
	while(inputFile>>query) {
		//cout << "nfefnkeenf\n";
		if(query == "INSERT") {
			int key; inputFile >> key;
			tree.Insert(key);
		}
		else if(query == "FIND") {
			int key; inputFile >> key;
			cout << ((tree.Find(key)) ? "YES" : "NO" )<< "\n";
		}
		else if(query == "COUNT") {
			int x; inputFile >> x;
			cout << tree.GetCount(x) << "\n";
		}
		else if(query == "RANGE") {
			int x, y;
			inputFile >> x >> y;
			if (y < x)
				cout << 0;
			else 
				cout << tree.GetCount(x, y) << "\n";
		}
	}


	return 0;
}