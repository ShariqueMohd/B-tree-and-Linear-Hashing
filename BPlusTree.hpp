#ifndef __BPLUSTREE__
#define __BPLUSTREE__

#include <bits/stdc++.h>

using namespace std;


// data members: data, children, isLeaf, capacity, count;
// functions: Node(T), Node(pair<>, T)
//			  Find(T), LowerBound(T), UpperBound(T)
//			  Insert(T)
template<typename T>
class Node {

public:
	Node(int capacity): capacity(capacity), data(), children(), isLeaf(true), count(0) {}
	
	Node(pair< pair<Node<T>*, Node<T>*>, T> splits): 
		capacity(splits.first.first->capacity), 
		data({splits.second}),
		children({splits.first.first, splits.first.second}),
		isLeaf(false),
		count(0) {
			AdjustCount();
		}

	~Node() {
		for(auto i=0; i<children.size() && !isLeaf; i++) {
			if(children[i] != nullptr)
				delete children[i];
		}
	}

	int getCount() const {
		return count;
	}

	int getCount(T x, T y) {
		int LowerBoundIndex = LowerBound(x, data.begin(), data.size());
		int upperBoundIndex = upperBound(y, data.begin(), data.size());

		if(isLeaf) {
			return upperBoundIndex - LowerBoundIndex;
		}
		int result = 0;
		for(int i=LowerBoundIndex+1; i<upperBoundIndex; i++) {
			result += children[i]->getCount();
		}
		result += children[LowerBoundIndex]->getCount(x, y);
		if(upperBoundIndex > LowerBoundIndex) {
			result += children[upperBoundIndex]->getCount(x, y);
		}
		return result;
	}

	bool Find(const T& element) {
		if(data.empty()) {
			return false;
		}
		if(isLeaf) {
			auto lower = LowerBound(element, data.begin(), data.size());
			auto upper = upperBound(element, data.begin(), data.size());
			return lower != upper;
		}
		else {
			int upper = upperBound(element, data.begin(), data.size());
			return children[upper]->Find(element);
		}
	}


	template<typename M, typename N>
	int LowerBound(const M& element, const N iterator, const int& size) {
		if(size == 1) {
			if(iterator[0] < element) {
				return 1;
			}
			else {
				return 0;
			}
		}
		else {
			int mid = size >> 1;
			if(iterator[mid-1] < element) {
				int newSize = size - mid;
				const N newIterator = iterator + mid;
				int LowerBoundIndex = LowerBound(element, newIterator, newSize);
				return LowerBoundIndex + mid;
			}
			else {
				int newSize = mid;
				const N newIterator = iterator;
				int LowerBoundIndex = LowerBound(element, newIterator, newSize);
				return LowerBoundIndex;
			}
		}
	}

	template<typename M, typename N>
	int upperBound(const M& element, const N iterator, const int& size) {
		if(size == 1) {
			if(iterator[0] > element) {
				return 0;
			}
			else {
				return 1;
			}
		}
		else {
			int mid = size >> 1;
			if(iterator[mid] <= element) {
				int newSize = size - mid;
				const N newIterator = iterator + mid;
				int upperBoundIndex = upperBound(element, newIterator, newSize);
				return upperBoundIndex + mid;
			}
			else {
				int newSize = mid;
				const N newIterator = iterator;
				int upperBoundIndex = upperBound(element, newIterator, newSize);
				return upperBoundIndex;
			}
		}
	}

	bool isOverFlow() const {
		if(children.size() > capacity) {
			return true;
		}
		return false;
	}

	bool isUnderFlow() const {
		if(children.size() < (capacity+1)>>1) {
			return true;
		}
		return false;
	}

	bool atMinCapacity() const {
		if(children.size() == (capacity+1)>>1) {
			return true;
		}
		return false;
	}

	bool atMaxCapacity() const {
		if(children.size() == capacity) {
			return true;
		}
		return false;
	}

	void AdjustCount() {
		if(isLeaf) {
			count = data.size(); 
			return;
		}
		count = 0;
		for(Node* child : children) {
			count += child->getCount();
		}
	}

	#define RT pair< pair< Node<T>*, Node<T>*>, T> 
	
	RT Split() {
		int splitIndex = (capacity+1)>>1 - 1;
		Node* rightNode = new Node<T>(capacity); T key = data[splitIndex];
		
		rightNode->data.insert(rightNode->data.begin(), data.begin()+splitIndex+1, data.end());
		rightNode->children.insert(rightNode->children.begin(), children.begin()+splitIndex+1, children.end());
		
		data.erase(data.begin()+splitIndex, data.end());
		children.erase(children.begin() + splitIndex+1, children.end());
		if(isLeaf) {
			children[children.size()-1] = rightNode;
			rightNode->children.insert(rightNode->children.begin(), nullptr);
			rightNode->data.insert(rightNode->data.begin(),key);
		}
		this->AdjustCount();
		rightNode->AdjustCount();
		return RT(pair<Node<T>*, Node<T>*>(this, rightNode), key);
	}


	void Insert(T element) {
		if(data.empty()){
			data.push_back(element);
			children.push_back(nullptr);	children.push_back(nullptr);
			return;
		}
		int upper = upperBound(element, data.begin(), data.size());
		if(isLeaf) {
			data.insert(data.begin() + upper, element);
			children.insert(children.begin() + upper, nullptr);
		}
		else {
			children[upper]->Insert(element);
			if(children[upper]->isOverFlow()) {
				auto splits = children[upper]->Split();
				data.insert(data.begin()+upper, splits.second);
				children.insert(children.begin() + upper, nullptr);
				children[upper] = splits.first.first;
				children[upper+1] = splits.first.second;
			}	
		}
		AdjustCount();
	}

private:
	bool isLeaf;
	vector<T> data;
	vector<Node*> children;
	int capacity, count;

};


// Class for B+ Tree
// Functions: Constructor, Deconstructor
//			  Find(), Insert()
//			  GetCount(), GetCount(x), GetCount(x,y)
template<typename T>
class BPlusTree {
	Node<T>* root;
	int capacity;

public:
	BPlusTree(int capacity): capacity(capacity), root(new Node<T>(capacity)) {}
	~BPlusTree() {
		if(root != nullptr) {
			delete root;
		}
	}

	bool Find(const T& element) const {
		return root->Find(element);
	}

	void Insert(T element) {
		if(root == nullptr)
			root = new Node<T>(capacity);
		this->root->Insert(element);
		if(this->root->isOverFlow()) {
			auto splits = this->root->Split();
			Node<T> *temp = new Node<T>(splits);
			root = temp;
		}
	}

	int GetCount() const {
		return root == nullptr ? 0 : root->getCount();
	}

	int GetCount(T x) const {
		return root == nullptr ? 0 : root->getCount(x, x);
	}

	int GetCount(T x, T y) const {
		return root == nullptr ? 0 : root->getCount();
	}

};

#endif