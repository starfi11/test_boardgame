#include"Boardgame_simple.h"

void Boardgame_central::LeastStepsSlove_BiBfsAll()
{
	priority_queue<State,vector<State>,Statecmp>forward;//分别存储目标层的正向与反向节点
	priority_queue<State,vector<State>,Statecmp>backward;//目标层的棋子个数为1+n)/2
	BiBfsAll_sub(forward, backward);
	vector<State>tempward;
	//将forward,与backward组合生成解
	//forward与backward均按state升序排列
	while (!(forward.empty()) && (!backward.empty()))
	{
		if (forward.top().state < backward.top().state)
		{
			//forward中需要出队
			long long temp = forward.top().state;
			while (!forward.empty() && forward.top().state == temp)
				forward.pop();
		}
		else if (forward.top().state > backward.top().state)
		{
			//backward中需要出队
			long long temp = backward.top().state;
			while (!backward.empty() && backward.top().state == temp)
				backward.pop();
		}
		else {
			//相等 组合答案
			//将forward头部与后续所有相等的backward组合答案
			long long temp = backward.top().state;
			while (!backward.empty() && backward.top().state == temp)
			{
				tempward.push_back(backward.top());
				backward.pop();
			}
			
			temp = forward.top().state;
			while (!forward.empty() && forward.top().state == temp)
			{
				//将反串拼接至正串中
				string s = forward.top().route;
				string temps = s;
				forward.pop();
				for (int i = 0; i < tempward.size(); i++)
				{
					temps = s;
					string tempstring = tempward[i].route;
					for (int i = tempstring.size() - 3; i >= 0; i -= 3)
						temps += tempstring.substr(i, 3);
					//计算合并后串s的步数
					int steps = Boardgame_simple::get_routesteps(temps);
					if (steps < this->step_min)
					{
						this->anwser_central.clear();
						this->step_min = steps;
						this->anwser_central.push_back(temps);
					}
					else if (steps == this->step_min)
						this->anwser_central.push_back(temps);
				}
				
			}//forward.front
			tempward.clear();
		}//else
	}//while
}
void Boardgame_central::BiBfsAll_sub(priority_queue<State,vector<State>,Statecmp>&forward,priority_queue<State, vector<State>, Statecmp>&backward)
{
	//'d','u','l','r'
	//扩展极性
	int polarity_forward_chess[4] = { 0,2,1,3 };//下左扩展优先  下左上右
	int polarity_forward_space[4] = { 1,3,0,2 };
	//逆向bfs的极性将决定能否搜索到部分解
	int polarity_backward_chess[4] = { 3,1,2,0 };
	int polarity_backward_space[4] = { 2,0,3,1 };
	//得出结论，逆向的不能带极性，正向可以极性
	// 两种方案：1.双向，逆向不带极性，能避免正向搜到非目标终局，但不带极性导致状态数非常多
	//			2.单向，正向带极性，但会搜索到所有终局（包括五种剩1个的和各种剩余终局）
	//字典序
	// A44=24
	//      step  solution
	//0123	 7		6
	//0132	无解
	//0213   7		6
	//0231   7      3
	//0312  无解
	//0321	无解
	//1023	 7		6
	//1032	 10     111
	//1203	 7		6
	//1230	 7		6
	//1302	 10		111
	//1320	 10		111
	//2013   8		11
	//2031	 8		8
	//2103	 6		5
	//2130	 6		5
	//2301	 8		8
	//2310	 6		5
	//3012	 8		74
	//3021	 8		74
	//3102	 无解
	//3120	 无解
	//3201	 8		74
	//3210  无解
	//去除逆向极性解为6 5

	//for (int i = 0; i < 4; i++)
	//{
	//	polarity_forward_chess[i] = i;
	//	polarity_forward_space[i] = i;
	//	polarity_backward_chess[i] = i;
	//	polarity_backward_space[i] = i;
	//}
	queue<State>upward;
	upward.push(State(this->origin_state));
	int destinlayer = (upward.front().get_chessnum() + State(this->destin_state).get_chessnum()) / 2;
	char array[10][10];
	map<long long, bool> equalmap;
	memcpy(array, this->chessboard, sizeof(this->chessboard));
	
	while (!upward.empty())
	{
		State temps = upward.front();
		int chessnum = temps.get_chessnum();//使用State封装的getchessnum效率更高
		if (chessnum <= destinlayer)
			break;
		upward.pop();
		equalmap.clear();//扩展极性重置
		//由temps解码扩展，要注意扩展的极性
		this->state_to_array_unsecure(array, temps.state);
		
		if (chessnum > this->method_dividing)
		{//遍历空格
			for (int k = 0; k < 4; k++)
				for (int i = up; i <= down; i++)
					for (int j = left; j <= right; j++)
						if (array[i][j] == 0)
						{
							int x1 = i + dx[polarity_forward_space[k]], x2 = i + dx[polarity_forward_space[k]] * 2;
							int y1 = j + dy[polarity_forward_space[k]], y2 = j + dy[polarity_forward_space[k]] * 2;
							if (x1 >= up && x1 <= down &&
								x2 >= up && x2 <= down &&
								y1 >= left && y1 <= right &&
								y2 >= left && y2 <= right)
								if (array[x1][y1] == 1 && array[x2][y2] == 1)
								{
									array[i][j] = 1;
									array[x1][y1] = 0;
									array[x2][y2] = 0;

									State nexts(array, temps.route + this->num_to_string(x2, y2) + char((polarity_forward_space[k] ^ 1) + '0'));
									//此层if是每次扩展时的极性保证，猜测保证每次扩展的极性，便能保证路径串在整体上的极性
									//想想怎么根据步数过滤
									if (!this->isrepeated_state(nexts, equalmap))//若equalmap中没有nexts的所有等效状态
									{
										equalmap[nexts.state] = 1;
										upward.push(nexts);
									}

									array[i][j] = 0;
									array[x1][y1] = 1;
									array[x2][y2] = 1;
								}
						}
		}
		else {
			//遍历棋子 forward
			for (int k = 0; k < 4; k++)
				for (int i = up; i <= down; i++)
					for (int j = left; j <= right; j++)
						if (array[i][j] == 1)

						{
							int x1 = i + dx[polarity_forward_chess[k]], x2 = i + dx[polarity_forward_chess[k]] * 2;
							int y1 = j + dy[polarity_forward_chess[k]], y2 = j + dy[polarity_forward_chess[k]] * 2;
							if (x1 >= up && x1 <= down &&
								x2 >= up && x2 <= down &&
								y1 >= left && y1 <= right &&
								y2 >= left && y2 <= right)
								if (array[x1][y1] == 1 && array[x2][y2] == 0)
								{
									array[i][j] = 0;
									array[x1][y1] = 0;
									array[x2][y2] = 1;

									State nexts(array, temps.route + this->num_to_string(i, j) + char(polarity_forward_chess[k] + '0'));
									if (!this->isrepeated_state(nexts, equalmap))//若equalmap中没有nexts的所有等效状态
									{
										equalmap[nexts.state] = 1;
										upward.push(nexts);
									}


									array[i][j] = 1;
									array[x1][y1] = 1;
									array[x2][y2] = 0;
								}
						}
		}

	}//正向扩展至目标层
	while (!upward.empty())
	{
		forward.push(upward.front());
		upward.pop();
	}

	upward.push(State(this->destin_state));
	while (!upward.empty())
	{
		State temps = upward.front();
		int chessnum = temps.get_chessnum();//使用State封装的getchessnum效率更高
		if (chessnum >= destinlayer)
			break;
		upward.pop();
		//equalmap.clear();
		//由temps解码扩展，要注意扩展的极性
		this->state_to_array_unsecure(array, temps.state);

		if (chessnum > this->method_dividing)
		{//遍历空格
			for (int k = 0; k < 4; k++)
				for (int i = up; i <= down; i++)
					for (int j = left; j <= right; j++)
						if (array[i][j] == 0)

						{
							int x1 = i + dx[polarity_backward_space[k]], x2 = i + dx[polarity_backward_space[k]] * 2;
							int y1 = j + dy[polarity_backward_space[k]], y2 = j + dy[polarity_backward_space[k]] * 2;
							if (x1 >= up && x1 <= down &&
								x2 >= up && x2 <= down &&
								y1 >= left && y1 <= right &&
								y2 >= left && y2 <= right)
								if (array[x1][y1] == 0 && array[x2][y2] == 1)
								{
									array[i][j] = 1;
									array[x1][y1] = 1;
									array[x2][y2] = 0;

									State nexts(array, temps.route + this->num_to_string(i, j) + char(polarity_backward_space[k] + '0'));
									//if (!this->isrepeated_state(nexts, equalmap))//若equalmap中没有nexts的所有等效状态
									//{
									//	equalmap[nexts.state] = 1;
									//	upward.push(nexts);
									//}
									/*else
										upward.push(nexts);*/

									upward.push(nexts);
									array[i][j] = 0;
									array[x1][y1] = 0;
									array[x2][y2] = 1;
								}
						}
		}
		else {
			//遍历棋子 forward
			for (int k = 0; k < 4; k++)
				for (int i = up; i <= down; i++)
					for (int j = left; j <= right; j++)
						if (array[i][j] == 1)

						{
							int x1 = i + dx[polarity_backward_chess[k]], x2 = i + dx[polarity_backward_chess[k]] * 2;
							int y1 = j + dy[polarity_backward_chess[k]], y2 = j + dy[polarity_backward_chess[k]] * 2;
							if (x1 >= up && x1 <= down &&
								x2 >= up && x2 <= down &&
								y1 >= left && y1 <= right &&
								y2 >= left && y2 <= right)
								if (array[x1][y1] == 0 && array[x2][y2] == 0)
								{
									array[i][j] = 0;
									array[x1][y1] = 1;
									array[x2][y2] = 1;

									State nexts(array, temps.route + this->num_to_string(x2, y2) + char(polarity_backward_chess[k] ^ 1 + '0'));
									//if (!this->isrepeated_state(nexts, equalmap))//若equalmap中没有nexts的所有等效状态
									//{
									//	equalmap[nexts.state] = 1;
									//	upward.push(nexts);
									//}
									/*else
										upward.push(nexts);*/
									upward.push(nexts);
									array[i][j] = 1;
									array[x1][y1] = 0;
									array[x2][y2] = 0;
								}
						}
		}
		
	}//反向
	while (!upward.empty())
	{
		backward.push(upward.front());
		/*cout << "反向生成:" << endl;
		cout << upward.front().state << endl;
		this->print_chessboard(upward.front().state);*/
		upward.pop();
	}
}