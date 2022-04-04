#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
using namespace std;
struct bucket
{
	set<int> nums;
	int localDepth;
};
class extendibleHash
{
private:
	map<int,bucket*> directory;
	int globalDepth;
	int capacity;
	vector<bucket*> buckets;
public:
	extendibleHash(int n,int N)
	{
		globalDepth = n;
		capacity = N;
		for(int i = 0;i<(1<<n);++i)
		{
			bucket *b = new bucket();
			buckets.push_back(b);
			b->localDepth = n;
			directory.insert({i,b});
		}
	}
	void insert(int key);
	bool search(int key);
	bool Delete(int key);
	void split(int idx);
	void shrink();
	void display();
	void merge(int idx);
};
void extendibleHash::split(int idx)
{
	bucket *b = directory[idx];
	bucket *newBucket = new bucket();
	buckets.push_back(newBucket);
	vector<int> temp;
	for(set<int>::iterator it = b->nums.begin();it!=b->nums.end();++it)
		temp.push_back(*it);
	b->nums.clear();
	if(b->localDepth==globalDepth)
	{
		directory[idx^(1<<globalDepth)] = newBucket;
		for(int i=0;i<(1<<globalDepth);++i)
			if(i!=idx)
				directory[i^(1<<globalDepth)] = directory[i];
		globalDepth++;
		newBucket->localDepth = ++b->localDepth;
		for(int i=0;i<temp.size();++i)
			{
				int h = (temp[i])&((1<<globalDepth)-1);
				directory[h]->nums.insert(temp[i]);
			}
		return;
	}
	int k = idx&((1<<(b->localDepth))-1);
	int t = (b->localDepth);
	for(int i=0;i<(1<<globalDepth);++i)
	{
		int lst = i&((1<<(b->localDepth))-1);
		if(k==lst)
			if((i>>t)&1)
				directory[i] = newBucket;
	}
	newBucket->localDepth = ++b->localDepth;
	for(int i=0;i<temp.size();++i)
		(directory[(temp[i])&((1<<globalDepth)-1)]->nums).insert(temp[i]);
	return;
}
void extendibleHash::insert(int key)
{
	int x = ((1<<globalDepth)-1)&key;
	bucket *b = directory[x];
	if((b->nums).size()<capacity)
	{
		(b->nums).insert(key);
		return;
	}
	else
	{
		split(x);
		insert(key);
	}
}
bool extendibleHash::search(int key)
{
	int x = ((1<<globalDepth)-1)&key;
	bucket *b = directory[x];
	if((b->nums).find(key)==(b->nums).end())
		return 0;
	return 1;
}
bool extendibleHash::Delete(int key)
{
	int x = ((1<<globalDepth)-1)&key;
	bucket *b= directory[x];
	set<int>::iterator it = (b->nums).find(key);
	if(it==(b->nums).end())
		return 0;
	b->nums.erase(it);
	merge(x);
	shrink();
	return 1;
}
void extendibleHash::display()
{
	cout<<globalDepth<<endl;
	cout<<buckets.size()<<endl;
	for(int i=0;i<buckets.size();++i)
		cout<<(buckets[i]->nums).size()<<" "<<buckets[i]->localDepth<<endl;
	//cout<<endl;
	//for(auto it=directory.begin();it!=directory.end();++it)
		//cout<<it->first<<" "<<(it->second)->nums.size()<<endl;
	for(auto it=directory.begin();it!=directory.end();++it)
	{
		cout<<it->first<<" ->   ";
		for(auto x:((it->second)->nums))
			cout<<x<<" ";
		cout<<endl;
	}
	return;
}
void extendibleHash::shrink()
{
	if(globalDepth<2)
		return;
	while(globalDepth>=2)
	{
	int count = 0;
	for(int i=0;i<buckets.size();++i)
		if(buckets[i]->localDepth<globalDepth)
			++count;
	if(count==buckets.size())
	{
		for(int i = 1<<(globalDepth-1);i<1<<(globalDepth);++i)
			directory.erase(i);
		globalDepth--;
	}
	else
		return;
	}
}
void extendibleHash::merge(int idx)
{
	if(((directory[idx]->nums).size()!=0||(directory[idx]->localDepth<2)))
		return;
	buckets.erase(find(buckets.begin(),buckets.end(),directory[idx]));
	int t = directory[idx]->localDepth-1;
	bucket* b = directory[idx];
	int img = idx^(t<<1);
	bucket *imgBucket = directory[img];
	imgBucket->localDepth--;
	for(int i = 0;i<(1<<globalDepth);++i)
		if(directory[i] == directory[idx])
			directory[i] = imgBucket;
	delete b;			
	return;	
}
int main()
{
	int globalDepth;
	int capacity;
	cin>>globalDepth>>capacity;
	extendibleHash* hashTable = new extendibleHash(globalDepth,capacity);
	int t;
	while(true)
	{
		cin>>t;
		if(t==6)
			break;
		if(t==2)
		{
			int key;
			cin>>key;
			hashTable->insert(key);
		}
		if(t==3)
		{
			int key;
			cin>>key;
			if(hashTable->search(key))
				cout<<"Key Present in the table"<<endl;
			else
				cout<<"Key not present in the table"<<endl;
		}
		if(t==4)
		{
			int key;
			cin>>key;
			if(hashTable->Delete(key))
				cout<<"Key Deleted"<<endl;
			else
				cout<<"Element not found"<<endl;
		}
		if(t==5)
			hashTable->display();
	}
	return 0;
}