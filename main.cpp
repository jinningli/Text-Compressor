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
#include <vector>
using namespace std;
#define infinite 0x7fffffff
#define wordsize 35
#define wordlength 4
int frequenceplus = 14;
//22
string worddic[] =
{
	"",
	"the", "of", "to", "and", "that",
	"in", "was", "is", "it", "you",
	"had", "he", "we", "she", "on",
	"as", "not", "her", "be", "have",
	"for", "But", "The", "with", "are",
	"from", "all", "It", "or", "can",
	"at", "were", "what", "his", "this"
};

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
	string word;
	st_node() :data('\0'), frequence(0), parent(0), left(0), right(0), num(0), word("") {}
}node[5000];

bool nodecmp(st_node a, st_node b)
{
	return a.frequence > b.frequence;
}

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
		sort(node + 1, node + nodesize + 1, nodecmp);
		while( frequenceplus >= nodesize - 1)  frequenceplus -= 10;
		for (int i = nodesize + 1; i <= nodesize + wordsize; i++)
		{
			node[i].word = worddic[i - nodesize];
			node[i].frequence = node[i - nodesize + frequenceplus].frequence;
		}
		nodesize += wordsize;
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
	char findchar(string & str, int & cut, string & strout)
	{
		cut = 0;
		int now = root;
		while (now > nodesize)
		{
			if (cut == str.length())
				exit(0);
			if (str[cut++] == '0')
			{
				now = node[now].left;
			}
			else
			{
				now = node[now].right;
			}
		}
		if (now >= nodesize - wordsize + 1 && now <= nodesize)
			strout = node[now].word;
		return node[now].data;
	}
}chips;


map<char, string> outlib;
map<string, string> wordlib;
int maxlen = 0;

void LIB_INITIALIZE()
{
	for (int i = 1; i <= chips.nodesize - wordsize; i++)
	{
		outlib[node[i].data] = node[i].code;
		if (node[i].code.length() > maxlen)
			maxlen = node[i].code.length();
	}
	for (int i = chips.nodesize - wordsize + 1; i <= chips.nodesize; i++)
	{
		wordlib[node[i].word] = node[i].code;
		if (node[i].code.length() > maxlen)
			maxlen = node[i].code.length();
	}
}

string codeplus(int &i)
{
//	if (i >= datasize - wordlength) return outlib[inidata[i]];
	string capa;
	for (int j = 0; j < wordlength; j++)
	{
		if ( i + j > datasize) break;
		capa += inidata[i + j];
		if (wordlib.count(capa))
		{
			i += j;
			return wordlib[capa];
		}
	}
	return outlib[inidata[i]];
}


void output()
{
	printf("%d %d %d ", chips.root, chips.nodesize, maxlen);
	for (int i = 1; i <= chips.nodesize - wordsize; i++)
	{
		printf("%c", node[i].data);
	}
	for (int i = chips.nodesize + 1; i <= chips.root; i++)
	{
		printf("%d %d ", node[i].left, node[i].right);
	}
	string code;
	for (int i = 1; i <= datasize; i++)
	{
		code += codeplus(i);
		while (code.length() <= 8 && i < datasize)
		{
			i++;
			code += codeplus(i);
		}
		unsigned char k = 0;
		if (i == datasize)
		{
			while (code.length() > 8)
			{
				k = 0;
				for (int j = 0; j <= 7; j++)
				{
					k = k * 2 + code[j] - '0';
				}
				printf("%c", k);
				code = code.substr(8);
			}
			if (!code.length())
				return;
			k = 0;
			for (int j = 0; j <= code.length() - 1; j++)
			{
				k = k * 2 + code[j] - '0';
			}
			printf("%c", k);
			printf("%c", code.length());
			return;
		}
		k = 0;
		for (int j = 0; j <= 7; j++)
		{
			k = k * 2 + code[j] - '0';
		}
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
	scanf("%d %d %d", &chips.root, &chips.nodesize, &maxlen);
	getchar();
	for (int i = 1; i <= chips.nodesize - wordsize; i++)
	{
		scanf("%c",&node[i].data);
	}
	for (int i = chips.nodesize + 1; i <= chips.root; i++)
	{
		scanf("%d %d ", &node[i].left, &node[i].right);
	}
	for (int i = chips.nodesize - wordsize + 1; i <= chips.nodesize; i++)
	{
		node[i].word = worddic[i - chips.nodesize + wordsize];
		node[i].frequence = node[i - chips.nodesize + wordsize + frequenceplus].frequence;
	}
	string bis;
	string found;
	int cut = 0;
	bool ending = false;
	while (1)
	{
		while (bis.length() <= maxlen + 16)
		{
			unsigned char tmp = '\0';
			if (scanf("%c", &tmp) == EOF)
			{
				ending = true;
				break;
			}
			bis += bistring(tmp);
		}
		if (ending)
		{
			int cnt = 0;
			for (int k = bis.length() - 8; k <= bis.length() - 1; k++)
			{
				cnt = cnt * 2 + bis[k] - '0';
			}
			for (int k =  bis.length() - 8 - cnt; k <= bis.length() - 9; k++)
			{
				bis[k - 8 + cnt] = bis[k];
			}
			bis = bis.substr(0, bis.length() - 16 + cnt);
			while (bis.length())
			{
				found = "";
				unsigned char find = chips.findchar(bis, cut, found);
				if (found != "")
				{
					cout << found;
				}
				else
				{
					printf("%c", find);
				}
				bis	= bis.substr(cut);
			}
		}
		found = "";
		unsigned char find = chips.findchar(bis, cut, found);
		if (found != "")
		{
			cout << found;
		}
		else
		{
			printf("%c", find);
		}
		bis = bis.substr(cut);
	}
}


void compress(char infile[], char outfile[])
{
	freopen(infile, "r", stdin);
	freopen(outfile, "wb", stdout);
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
	//chips.show();
	output();
}


void decompress(char infile[], char outfile[])
{
	freopen(infile, "rb", stdin);
	freopen(outfile, "w", stdout);
	decode();
}

int main(int argc, char* argv[])
{
	//freopen("dic.txt", "w", stdout);
	/*
	if (argc != 4)
	{
		printf("---------------------------------------\nCompress: -c [filename1] [filename2]\n\nDecompress: -d [filename1] [filename2]\n---------------------------------------");
		return 0;
	}
	if (argv[1][1] == 'c'|| argv[1][1] == 'C')
	{
		compress(argv[2], argv[3]);
	}
	else if (argv[1][1] == 'd' || argv[1][1] == 'D')
	{
		decompress(argv[2], argv[3]);
	}
	else
	{
		printf("---------------------------------------\nCompress: -c [filename1] [filename2]\n\nDecompress: -d [filename1] [filename2]\n---------------------------------------");
	}*/
	compress("Uncle_Toms_Cabin.txt", "[Compressed]Uncle_Toms_Cabin.out");
	decompress("[Compressed]Uncle_Toms_Cabin.out", "[Decoded]Uncle_Toms_Cabin.txt");
	return	0;
}
