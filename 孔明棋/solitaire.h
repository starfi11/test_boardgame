#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<map>
#include<vector>
#include<string>
#include<queue>
#include<algorithm>
using namespace std;
struct OriginState {

	/*棋局状态ing 单个State没有意义，只有知道棋盘才能解压*/
	/*******  	实例数据成员	*******/
	long long state;											//二进制01压缩，只压缩棋局内的格子；孔明棋33格则state的二进制位范围为0~32，共33个


	/*******  	静态数据成员	*******/
	static const int board_maxlength = 10;						//10*10的最大棋盘


	/*******  	实例函数成员	*******/
	OriginState():state(0) {}
	OriginState(long long state):state(state){}
	OriginState(char array[10][10]){ this->state = OriginState::array_to_state_unsecure(array); }
	OriginState(char array[10][10],int up,int down,int left,int right){}
	int get_chessnum() const;//获得本状态的棋子数


	/*******  	静态函数成员（工具函数）	*******/

	static void array_print(char array[board_maxlength][board_maxlength]);
	static long long string_to_state(string);
	static string array_to_string(char[board_maxlength][board_maxlength]);
	static long long array_to_state_unsecure(char[board_maxlength][board_maxlength]);
	static long long array_to_state_efficient(char array[10][10], int up, int down, int left, int right);
};
struct State :public OriginState{

	/*棋局状态ing 单个State没有意义，只有知道棋盘才能解压*/
	/*******  	实例数据成员	*******/
	string route;

	/*******  	实例函数成员	*******/
	State() :OriginState(), route("") {}
	State(long long state) :OriginState(state), route("") {}
	State(long long state, string s) :OriginState(state), route(s) {}
	State(char array[10][10]) :OriginState(array),route("") {}
	State(char array[10][10], string s) :OriginState(array),route(s) {}

};
struct ConState :public State {
	int step;//到达当前状态的步数(连跳视为一步)
	ConState() :State(),step(0) {}
	ConState(long long state):State(state),step(0){}
	ConState(char array[State::board_maxlength][State::board_maxlength], int step) :State(array), step(step) {}
	ConState(char array[State::board_maxlength][State::board_maxlength]) :State(array), step(0) {}
	ConState(char array[State::board_maxlength][State::board_maxlength],string s,int step):State(array,s),step(step){}
};