#include <cstdio>
#include <iostream>
#include <cstring>
#include <cmath>
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <algorithm>
#include <queue>
#include <map>
#include <fstream>
using namespace std;
#define infinite 0x7fffffff

char inidata[5000000];
int datasize = 0;

struct st_node
{
	char data;
	int frequence;
	int num;
	string code;
	int parent;
	int left;
	int right;
	st_node() :data('\0'), frequence(0), parent(0), left(0), right(0), num(0) {}
}node[5000];

class hftree
{
public:
	struct cmp
	{
		bool operator()(int a, int b)
		{
			return node[a].frequence > node[b].frequence;
		}
	};
	int charactor[500];
	int nodesize = 0;
	int root = 0;

	void insert(char ch)
	{
		if (!charactor[ch])
		{
			charactor[ch] = ++nodesize;
			node[nodesize].data = ch;
			node[nodesize].num = nodesize;
			node[charactor[ch]].frequence++;
			return;
		}
		node[charactor[ch]].frequence++;
	}
	void getcode()
	{
		for (int i = 1; i <= nodesize; i++)
		{
			int now = i;
			int parents = node[now].parent;
			while (parents)
			{
				if (node[parents].left == now)
					node[i].code = '0' + node[i].code;
				else node[i].code = '1' + node[i].code;
				now = parents;
				parents = node[now].parent;
			}
		}
	}
	void build()
	{
		priority_queue<int, vector<int>, cmp> chipspq;
		for (int i = 1; i <= nodesize; i++)
		{
			chipspq.push(i);
		}
		int pos = nodesize;
		while (chipspq.size() > 1)
		{
			int tmp1 = chipspq.top();
			chipspq.pop();
			int tmp2 = chipspq.top();
			chipspq.pop();
			++pos;
			node[pos].frequence = node[tmp1].frequence + node[tmp2].frequence;
			node[pos].left = tmp1;
			node[pos].right = tmp2;
			node[tmp1].parent = pos;
			node[tmp2].parent = pos;
			chipspq.push(pos);
		}
		root = chipspq.top();
		chipspq.pop();
		getcode();
	}
	void show()
	{
		for (int i = 1; i <= nodesize; i++)
		{
			printf("%c  f=%d,  code is: ", node[i].data, node[i].frequence);
			cout << node[i].code << endl;
		}
	}
	char findchar(string & str, int & cut)
	{
		cut = 0;
		int now = root;
		while (now > nodesize)
		{
			if (str[cut++] == '0')
			{
				now = node[now].left;
			}
			else
			{
				now = node[now].right;
			}
		}
		return node[now].data;
	}
}chips;


map<char, string> outlib;
int maxlen = 0;

void LIB_INITIALIZE()
{
	for (int i = 1; i <= chips.nodesize; i++)
	{
		outlib[node[i].data] = node[i].code;
		if (node[i].code.length() > maxlen)
			maxlen = node[i].code.length();
	}
}

void output()
{
	string code;
	for (int i = 1; i <= datasize; i++)
	{
		code += outlib[inidata[i]];
		while (code.length() <= 8 && i <= datasize)
		{
			i++;
			code += outlib[inidata[i]];
		}
		unsigned char k = 0;
		for (int j = 0; j <= 7; j++)
		{
			k = k * 2 + code[j] - '0';
		}
		//printf("[%d]\n", k);
		printf("%c", k);
		code = code.substr(8);
	}
}

string bistring(unsigned char ch)
{
	string tmp;
	while (ch)
	{
		tmp = (char)(ch % 2 + '0') + tmp;
		ch /= 2;
	}
	while (tmp.length() != 8)
	{
		tmp = '0' + tmp;
	}
	return tmp;
}

void decode()
{
	string bis;
	int cut = 0;
	while (1)
	{
		while (bis.length() <= maxlen)
		{
			unsigned char tmp = '\0';
			if (scanf("%c", &tmp) == EOF)
				return;
			bis += bistring(tmp);
		}
		printf("%c", chips.findchar(bis, cut));
		bis = bis.substr(cut);
	}
}
int main()
{
	freopen("Uncle_Toms_Cabin.txt", "r", stdin);
	while (scanf("%c", &inidata[++datasize]) != EOF)
	{
		if (inidata[datasize] < 0)
		{
			datasize--;
			continue;
		}
		chips.insert(inidata[datasize]);
	}
	datasize--;
	chips.build();
	LIB_INITIALIZE();
	//freopen("lib.txt", "w", stdout);
	//chips.show();
	//freopen("Uncle_Toms_Cabin.out", "w", stdout);
	//freopen("outbit.txt", "w", stdout);

	//output();


	fclose(stdin);
	freopen("Uncle_Toms_Cabindecode.out", "w", stdout);
	freopen("Uncle_Toms_Cabin.out", "rb", stdin);
	decode();
	return 0;
}