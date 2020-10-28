#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <set>
#include <cassert>
using namespace std;

struct Node {
	Node* next;
	Node* prev;
	int value;
	int key;
	Node(Node* p, Node* n, int k, int val) :prev(p), next(n), key(k), value(val) {};
	Node(int k, int val) :prev(NULL), next(NULL), key(k), value(val) {};
};

class Cache {

protected:
	map<int, Node*> mp; //map the key to the node in the linked list
	int cp;  //capacity
	Node* tail; // double linked list tail pointer
	Node* head; // double linked list head pointer
	virtual void set(int, int) = 0; //set function
	virtual int get(int) = 0; //get function

};
class LRUCache : public Cache
{
private:
	void addFirst(Node* n)
	{
		if (n == nullptr || n == head)
			return;
		if (head == nullptr)
		{
			head = tail = n;
			return;
		}
		n->next = head;
		head->prev = n;
		head = n;
	}
	Node* exclude(Node* n)
	{
		if (n == nullptr)
			return nullptr;

		if (n->prev != nullptr)
			n->prev->next = n->next;
		if (n->next != nullptr)
			n->next->prev = n->prev;
		if (n == head)
			head = n->next;
		if (n == tail)
			tail = n->prev;

		n->next = n->prev = nullptr;
		return n;
	}
public:
	LRUCache(int capacity)
	{
		assert(capacity > 0);
		cp = capacity;
		head = tail = nullptr;
	}
	virtual ~LRUCache()
	{
		Node* n = tail;
		while (n != nullptr)
		{
			tail = n;
			n = n->prev;
			delete tail;
		}
		head = tail = nullptr;
		mp.clear();
		cp = 0;
	}
	virtual void set(int key, int value)
	{
		assert(key > 0);
		assert(value != -1);
		Node* n = nullptr;
		auto c = mp.find(key);
		if (c != mp.end())
		{
			n = exclude(c->second);
			n->value = value;
		}
		else
		{
			if (mp.size() == cp)
			{
				mp.erase(tail->key);
				Node* r = exclude(tail);
				n = new (r) Node(key, value);
			}
			else
			{
				n = new Node(key, value);
			}
			mp[key] = n;
		}
		assert(n != nullptr);
		addFirst(n);
	}
	virtual int get(int key)
	{
		auto c = mp.find(key);
		if (c != mp.end())
			return c->second->value;
		return -1;
	}
};
int main() {
	int n, capacity, i;
	cin >> n >> capacity;
	LRUCache l(capacity);
	for (i = 0; i < n; i++) {
		string command;
		cin >> command;
		if (command == "get") {
			int key;
			cin >> key;
			cout << l.get(key) << endl;
		}
		else if (command == "set") {
			int key, value;
			cin >> key >> value;
			l.set(key, value);
		}
	}
	return 0;
}
