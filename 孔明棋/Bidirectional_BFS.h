#pragma once
#include"solitaire.h"
#include<map>
#include<string>
#include<queue>
static const int numarray[7][7] = {
		0, 0, 1, 2, 3, 0, 0,
		0, 0, 4, 5, 6, 0, 0,
		7, 8, 9, 10,11,12,13,
		14,15,16,17,18,19,20,
		21,22,23,24,25,26,27,
		0, 0, 28,29,30,0, 0,
		0, 0, 31,32,33,0, 0
};
static const int renumarray_x[34] = {
	-1,
	0,0,0,
	1,1,1,
	2,2,2,2,2,2,2,
	3,3,3,3,3,3,3,
	4,4,4,4,4,4,4,
	5,5,5,
	6,6,6
};
static const int renumarray_y[34] = {
	-1,2,3,4,
	2,3,4,
	0,1,2,3,4,5,6,
	0,1,2,3,4,5,6,
	0,1,2,3,4,5,6,
	2,3,4,
	2,3,4
};
struct Bidirectional_BFS{
	long long origin_state;
	long long destin_state;
	static const int dividing = 13;//小于该数时遍历棋子，否则遍历空格
	static constexpr int dy[4] = { 0,0,-1,1 };//上下左右
	static constexpr int dx[4] = { 1,-1,0,0 };//上下左右
	static constexpr char direction[4] = { 'd','u','l','r'};
	
	map<long long, string>state_map;//01~33,string格式为01u02d21l22r以此表示路径
	map<long long, string>backmap;
	//一次操作可由一个原位置和一个方向表示
	//而到达某个状态的操作字符数一定相同,视连跳为一步，map中步数少的将覆盖步数多的

	//可建一个map<long long,long long>将可记录到达某状态的方法数
	Bidirectional_BFS(long long origin, long long destin):origin_state(origin),destin_state(destin)
	{
		state_map[origin] = "";
		backmap[destin] = "";
	}
	bool solve();//双向搜索
	string two_string(int num) {
		string s = "";
		int temp = num % 10;
		num /= 10;
		s += num % 10+'0';
		s += temp + '0';
		return s;
	}
	void route_print();
};