#pragma once
#include<iostream>
using namespace std;
struct State {//棋局状态
	long long state;
	static const long long state_final = 1 << 16;
	State():state(state_final){}//默认状态为天才解终止状态
	State(long long state):state(state){}
	State(char s[7][7]);
	void transfer_to_array(char s[7][7]);
	static void array_init(char s[7][7]);//工具函数
	static void array_print(char s[7][7]);
	static long long string_to_state(string s);
	int true_count()//计算棋局中棋子的个数
	{
		long long temp = state;
		int ans = 0;
		while (temp)
		{
			ans++;
			temp = temp & (temp - 1);
		}
		return ans;
	}

	void print();
};
