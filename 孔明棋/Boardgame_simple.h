#pragma once
#include"solitaire.h"
struct Boardgame_simple {

	//面向所有棋局的dfs解

	/*实例数据成员*/
	long long origin_state;//初始棋局
	char chessboard[State::board_maxlength][State::board_maxlength];

	int up;//边界
	int down;
	int left;
	int right;
	int method_dividing;//搜索空格和搜索棋子策略选择的分界线，默认值取棋格数一半
	string anwser_simple;//一个解
	int chessnum_min;//最后棋子最少剩下几个
	int squarenum;//格子数量

	/*静态数据成员*/
	static constexpr int dy[4] = { 0,0,-1,1 };
	static constexpr int dx[4] = { 1,-1,0,0 };//下上左右 k属于0,1,2,3 取反方向为k^1
	static constexpr char direction[4] = { 'd','u','l','r' };




	/*实例函数成员*/
	Boardgame_simple(string s);//棋局正方形的* o .串, ‘0’为换行标记
	long long array_to_state(char array[State::board_maxlength][State::board_maxlength]);//simple类提供的更高效的转换函数
	void print_chessboard(char[State::board_maxlength][State::board_maxlength]);
	void print_chessboard();
	void print_simplecase();
	void OnlyOneLeft();//dfs搜索棋子仅剩一个的解
	void print_OnlyOneLeft();
	void print_route() { print_route(this->anwser_simple); }
	void print_route(string);
	void print_route_continuous(string);
	bool dfs(char[State::board_maxlength][State::board_maxlength],int,string);
	int get_chessnum(char s[State::board_maxlength][State::board_maxlength]);
	string route_transfer_string() { return route_transfer_string(this->anwser_simple); }
	string route_transfer_string(string ans) {
		for (int i = 0; i <= ans.size() - 3; i += 3)
			ans[i + 2] = direction[ans[i + 2] - '0'];
		return ans;
	}
	void state_to_array_unsecure(char[State::board_maxlength][State::board_maxlength],long long state);//在初始棋局的基础上将压缩码解压
	long long construct_final_state()//保证对中心对称格局生成中心最终压缩码，对普通格局生成一个合法的最终码
	{
		long long ans = 1;
		//long long ans = 1 << (this->squarenum / 2);//应该不会超32
		for (int i = 0; i < this->squarenum / 2; i++)
			ans *= 2;
		return ans;
	}
	long long mirror_level(char array[State::board_maxlength][State::board_maxlength]);//将传入的数组永久性上下镜像 并返回镜像后的状态码
	long long mirror_vertical(char s[State::board_maxlength][State::board_maxlength]);//要注意，被镜像的只有有效部分
	
	static string read_string();
	
	static string num_to_string(int i, int j)
	{
		//从上至下 从左至右 00~99
		int num = i * 10 + j;
		string ans = "";
		int temp = num % 10;
		num /= 10;
		ans += num % 10 + '0';
		ans += temp + '0';
		return ans;
	}
};

struct Boardgame_central :public Boardgame_simple {
	
	//用于求解最少步数的所有解(给定起始和终止状态)
	//要求棋盘中心对称，目标棋局中心对称，即要求棋盘有效部分是一个正方形，主要面向求天才十八步的解
	/*实例数据成员*/
	long long destin_state;
	int step_min;
	vector<string> anwser_central;
	

	/*实例函数成员*/
	Boardgame_central(string s) :Boardgame_simple(s) {
		step_min = this->chessnum_min;
		destin_state = this->construct_final_state();//终局棋盘由中心函数生成仅剩中心的压缩串
	}
	Boardgame_central(string s,int step) :Boardgame_simple(s) {
		step_min = step;
		destin_state = this->construct_final_state();
	}
	Boardgame_central(string s,long long final_state) :Boardgame_simple(s),destin_state(final_state) {
		step_min = this->chessnum_min;
	}
	void print_centralcase();
	void LeastStepsSolve();
	void LeastStepsSolve_less_memory();
	void LeastStepsSolve_more_less_memory();
	void OnlyOneLeft();//使用双向bfs，面向中心对称棋局，求解和父类OnlyOneLeft一样的问题

	void LSS_less_memory_bfs(queue<ConState>& before, queue<ConState>& after, map<long long, int>& visited);
	void print_LeastStepsSolve();
	long long turn_unsecure(char array[State::board_maxlength][State::board_maxlength]);//将传入的数组内left~right up~down中的有效部分永久旋转，并返回压缩码
	bool isrepeated_state(ConState& state, map<long long, int>&);
	bool isrepeated_state(OriginState& st, map<long long, string>& visited);
};
