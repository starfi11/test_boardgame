#include"Boardgame_simple.h"
#include<queue>
bool Boardgame_central::isrepeated_state(ConState& st,map<long long,int>&visited)
{
	//对应的八种状态都未在表中出现，或st的步数比所有出现过的状态的步数都小或等于，则返回0
	int equalmin = 0x3f3f3f3f;
	long long tempstate;
	char array[10][10];
	memcpy(array, this->chessboard, sizeof(chessboard));
	this->state_to_array_unsecure(array,st.state);

	if (visited.count(st.state))
		equalmin = visited[st.state];
	for (int i = 0; i < 3; i++)
	{
		tempstate = this->turn_unsecure(array);
		if (visited.count(tempstate))
			equalmin = min(equalmin, visited[tempstate]);
	}

	//翻转后再旋转
	tempstate = this->mirror_level(array);
	if (visited.count(tempstate))
		equalmin = min(equalmin, visited[tempstate]);

	for (int i = 0; i < 3; i++)
	{
		tempstate = this->turn_unsecure(array);
		if (visited.count(tempstate))
			equalmin = min(equalmin, visited[tempstate]);
	}

	return !(equalmin == 0x3f3f3f3f || st.step <= equalmin);
}
bool Boardgame_central::isrepeated_state(OriginState& st, map<long long, string>& visited)
{
	//对应的八种状态都未在表中出现，或st的步数比所有出现过的状态的步数都小或等于，则返回0
	int equalmin = 0x3f3f3f3f;
	long long tempstate;
	char array[10][10];
	memcpy(array, this->chessboard, sizeof(chessboard));
	this->state_to_array_unsecure(array, st.state);

	if (visited.count(st.state))
		return 1;
	for (int i = 0; i < 3; i++)
	{
		tempstate = this->turn_unsecure(array);
		if (visited.count(tempstate))
			return 1;
	}

	//翻转后再旋转
	tempstate = this->mirror_level(array);
	if (visited.count(tempstate))
		return 1;

	for (int i = 0; i < 3; i++)
	{
		tempstate = this->turn_unsecure(array);
		if (visited.count(tempstate))
			return 1;
	}

	return 0;
}
long long Boardgame_central::turn_unsecure(char array[State::board_maxlength][State::board_maxlength])
//将传入的数组内left~right up~down中的有效部分永久旋转，并返回压缩码
//硬性要求有效部分是一个正方形(奇偶均可）
{
	int originx, originy, newx, newy, temp;
	char originnum;
	int x_boundary, y_boundary;
	int width = this->down - this->up + 1;
	x_boundary = width / 2 - 1 + up;
	y_boundary = width / 2 + left;
	if (width % 2 == 0)
		y_boundary--;

	for(int i=up;i<=x_boundary;i++)
		for (int j = left; j <= y_boundary; j++)
		{
			originx = i;
			originy = j;
			newx = originx;
			newy = originy;
			originnum = array[originx][originy];
			for (int k = 0; k < 3; k++)
			{
				temp = newx;
				newx = width - 1 - newy;
				newy = temp;
				array[originx][originy] = array[newx][newy];
				originx = newx;
				originy = newy;
			}
			array[originx][originy] = originnum;
		}
	return this->array_to_state(array);
}
void Boardgame_central::print_centralcase()
{
	char array[10][10];
	memcpy(array, this->chessboard, sizeof(this->chessboard));
	cout << "搜寻步数最短解:" << endl;
	cout << "原始棋局:" << endl;
	this->print_chessboard();
	this->state_to_array_unsecure(array, this->destin_state);
	cout << "目标棋局:" << endl;
	this->print_chessboard(array);
}
void Boardgame_central::print_LeastStepsSolve()
{
	if (this->step_min >= get_chessnum(this->chessboard))
	{
		cout << "无解" << endl;
		return;
	}

	cout << "最短步数：" << this->step_min << endl;
	cout << "解个数：" << anwser_central.size() << endl;
	for (int i = 0; i < this->anwser_central.size(); i++)
		cout << anwser_central[i] << endl;
	for (int i = 0; i < this->anwser_central.size(); i++)
		cout << this->route_transfer_string(anwser_central[i]) << endl;

	for (int i = 0; i < this->anwser_central.size(); i++)
	{
		cout << "解法" << i + 1 << endl;
		this->print_route_continuous(anwser_central[i]);
	}
}
void Boardgame_central::LSS_less_memory_bfs(queue<ConState>& before, queue<ConState>& after, map<long long, int>& visited)
{
	int chessnum = before.front().get_chessnum();
	char array[10][10];
	memcpy(array, this->chessboard, sizeof(chessboard));
	while (!before.empty())
	{

		ConState tempcons = before.front();
		before.pop();

		if (tempcons.step > this->step_min)
			continue;
		if (tempcons.state == this->destin_state)
		{
			//搜到
			if (tempcons.step < this->step_min)
			{//清空并更新
				this->anwser_central.clear();
				this->step_min = tempcons.step;
				this->anwser_central.push_back(tempcons.route);
			}
			else if (tempcons.step == this->step_min)
			{//加入
				this->anwser_central.push_back(tempcons.route);
			}
			continue;
		}


		this->state_to_array_unsecure(array, tempcons.state);

		int beforenum = -1;
		int tempssize = tempcons.route.size();
		if (tempcons.route.size() < 3)//独特性的解压beforenum,要求route格式必须是01u 02d三个三个组成的
		{
			beforenum = -1;
		}
		else {
			beforenum = (tempcons.route[tempssize - 3] - '0' + this->dx[tempcons.route[tempssize - 1] - '0'] * 2) * 10 +
				(tempcons.route[tempcons.route.size() - 2] - '0' + this->dy[tempcons.route[tempssize - 1] - '0'] * 2);
		}

		if (chessnum > this->method_dividing)
		{//遍历空格
			for (int i = up; i <= down; i++)
				for (int j = left; j <= right; j++)
					if (array[i][j] == 0)
						for (int k = 0; k < 4; k++)
						{
							int x1 = i + dx[k], x2 = i + dx[k] * 2;
							int y1 = j + dy[k], y2 = j + dy[k] * 2;
							if (x1 >= up && x1 <= down &&
								x2 >= up && x2 <= down &&
								y1 >= left && y1 <= right &&
								y2 >= left && y2 <= right)
								if (array[x1][y1] == 1 && array[x2][y2] == 1)
								{
									array[i][j] = 1;
									array[x1][y1] = 0;
									array[x2][y2] = 0;

									int nextstep = tempcons.step;
									if (beforenum != x2 * 10 + y2)
										nextstep++;

									ConState ts(array, tempcons.route + this->num_to_string(x2, y2) + char((k ^ 1) + '0'), nextstep);
									if (!this->isrepeated_state(ts, visited))
									{
										visited[ts.state] = ts.step;
										after.push(ts);
									}
									array[i][j] = 0;
									array[x1][y1] = 1;
									array[x2][y2] = 1;
								}
						}
		}
		else {
			//遍历棋子 forward
			for (int i = up; i <= down; i++)
				for (int j = left; j <= right; j++)
					if (array[i][j] == 1)
						for (int k = 0; k < 4; k++)
						{
							int x1 = i + dx[k], x2 = i + dx[k] * 2;
							int y1 = j + dy[k], y2 = j + dy[k] * 2;
							if (x1 >= up && x1 <= down &&
								x2 >= up && x2 <= down &&
								y1 >= left && y1 <= right &&
								y2 >= left && y2 <= right)
								if (array[x1][y1] == 1 && array[x2][y2] == 0)
								{
									array[i][j] = 0;
									array[x1][y1] = 0;
									array[x2][y2] = 1;

									int nextstep = tempcons.step;
									if (beforenum != i * 10 + j)
										nextstep++;

									ConState ts(array, tempcons.route + this->num_to_string(i, j) + char(k + '0'), nextstep);
									if (!this->isrepeated_state(ts, visited))
									{
										visited[ts.state] = ts.step;
										after.push(ts);
									}
									array[i][j] = 1;
									array[x1][y1] = 1;
									array[x2][y2] = 0;
								}
						}
		}

	}//while

}
void Boardgame_central::LeastStepsSolve_more_less_memory()
{
	map<long long, int>visited_central;//并存储步数
	queue<ConState>squ;
	squ.push(ConState(this->origin_state));
	int deep = squ.front().get_chessnum();//以棋子个数记录层数
	char array[10][10];
	memcpy(array, this->chessboard, sizeof(this->chessboard));
	while (!squ.empty())
	{
		ConState temps = squ.front();
		squ.pop();
		if (temps.step > this->step_min)
			continue;
		if (temps.state == this->destin_state)
		{
			//搜到
			if (temps.step < this->step_min)
			{//清空并更新
				this->anwser_central.clear();
				this->step_min = temps.step;
				this->anwser_central.push_back(temps.route);
			}
			else if (temps.step == this->step_min)
			{//加入
				this->anwser_central.push_back(temps.route);
			}
			continue;
		}
		//else if (this->isrepeated_state(temps, visited_central))//避免重复进行 一个状态扩展到其他状态 的操作
		//	continue;//理论上这个重复判断应该只有一处，但为了极限减少内存，两处都有
		/*产生新状态*/

		//记录
		


		//解压
		
		int beforenum = -1;
		
		this->state_to_array_unsecure(array, temps.state);
		int tempssize = temps.route.size();
		int chessnum = temps.get_chessnum();//使用State封装的getchessnum效率更高

		if (chessnum < deep)
		{
			visited_central.clear();
		}
		deep = chessnum;


		if (temps.route.size() < 3)//独特性的解压beforenum,要求route格式必须是01u 02d三个三个组成的
		{
			beforenum = -1;
		}
		else {
			beforenum = (temps.route[tempssize - 3] - '0' + this->dx[temps.route[tempssize - 1] - '0'] * 2) * 10 +
				(temps.route[temps.route.size() - 2] - '0' + this->dy[temps.route[tempssize - 1] - '0'] * 2);
		}
		
		if (chessnum > this->method_dividing)
		{//遍历空格
			for (int i = up; i <= down; i++)
				for (int j = left; j <= right; j++)
					if (array[i][j] == 0)
						for (int k = 0; k < 4; k++)
						{
							int x1 = i + dx[k], x2 = i + dx[k] * 2;
							int y1 = j + dy[k], y2 = j + dy[k] * 2;
							if (x1 >= up && x1 <= down &&
								x2 >= up && x2 <= down &&
								y1 >= left && y1 <= right &&
								y2 >= left && y2 <= right)
								if (array[x1][y1] == 1 && array[x2][y2] == 1)
								{
									array[i][j] = 1;
									array[x1][y1] = 0;
									array[x2][y2] = 0;

									int nextstep = temps.step;
									if (beforenum != x2 * 10 + y2)
										nextstep++;

									ConState ts(array, temps.route + this->num_to_string(x2, y2) + char((k ^ 1) + '0'), nextstep);
									if (!this->isrepeated_state(ts, visited_central))
									{
										visited_central[ts.state] = ts.step;
										squ.push(ts);
									}
									array[i][j] = 0;
									array[x1][y1] = 1;
									array[x2][y2] = 1;
								}
						}
		}
		else {
			//遍历棋子 forward
			for (int i = up; i <= down; i++)
				for (int j = left; j <= right; j++)
					if (array[i][j] == 1)
						for (int k = 0; k < 4; k++)
						{
							int x1 = i + dx[k], x2 = i + dx[k] * 2;
							int y1 = j + dy[k], y2 = j + dy[k] * 2;
							if (x1 >= up && x1 <= down &&
								x2 >= up && x2 <= down &&
								y1 >= left && y1 <= right &&
								y2 >= left && y2 <= right)
								if (array[x1][y1] == 1 && array[x2][y2] == 0)
								{
									array[i][j] = 0;
									array[x1][y1] = 0;
									array[x2][y2] = 1;

									int nextstep = temps.step;
									if (beforenum != i * 10 + j)
										nextstep++;

									ConState ts(array, temps.route + this->num_to_string(i, j) + char(k + '0'), nextstep);
									if (!this->isrepeated_state(ts, visited_central))
									{
										visited_central[ts.state] = ts.step;
										squ.push(ts);
									}
									array[i][j] = 1;
									array[x1][y1] = 1;
									array[x2][y2] = 0;
								}
						}
		}

	}
}
void Boardgame_central::LeastStepsSolve_less_memory()
{
	//逐层BFS
	//上一步落点的位置由ConState的route后三位给出
	map<long long, int>visited_central;//并存储步数
	queue<ConState>squ1;
	queue<ConState>squ2;
	squ1.push(ConState(this->origin_state));
	while ((!squ1.empty()) || (!squ2.empty()))
	{
		if (!squ1.empty())
		{
			this->LSS_less_memory_bfs(squ1, squ2, visited_central);
			visited_central.clear();
		}
		else {
			this->LSS_less_memory_bfs(squ2, squ1, visited_central);
			visited_central.clear();
		}
	}
}
void Boardgame_central::LeastStepsSolve()
{
	//BFS解法 因为在该场景中搜索树上下均窄，双向bfs会退化为bfs,既然如此则使用记录更方便的bfs
	//上一步落点的位置由ConState的route后三位给出
	map<long long, int>visited_central;//并存储步数
	queue<ConState>squ;
	squ.push(ConState(this->origin_state));
	char array[10][10];
	memcpy(array, this->chessboard, sizeof(this->chessboard));
	while (!squ.empty())
	{
		ConState temps = squ.front();
		squ.pop();
		if (temps.step > this->step_min)
			continue;
		if (temps.state == this->destin_state)
		{
			//搜到
			if (temps.step < this->step_min)
			{//清空并更新
				this->anwser_central.clear();
				this->step_min = temps.step;
				this->anwser_central.push_back(temps.route);
			}
			else if (temps.step == this->step_min)
			{//加入
				this->anwser_central.push_back(temps.route);
			}
			continue;
		}
		//else if (this->isrepeated_state(temps, visited_central))//避免重复进行 一个状态扩展到其他状态 的操作
		//	continue;//理论上这个重复判断应该只有一处，但为了极限减少内存，两处都有
		/*产生新状态*/

		//记录
		visited_central[temps.state] = temps.step;


		//解压
		
		int beforenum = -1;
		
		this->state_to_array_unsecure(array, temps.state);
		int tempssize = temps.route.size();
		if (temps.route.size() < 3)//独特性的解压beforenum,要求route格式必须是01u 02d三个三个组成的
		{
			beforenum = -1;
		}
		else {
			beforenum = (temps.route[tempssize - 3] - '0' + this->dx[temps.route[tempssize - 1] - '0'] * 2) * 10 +
				(temps.route[temps.route.size() - 2] - '0' + this->dy[temps.route[tempssize - 1] - '0'] * 2);
		}
		int chessnum = temps.get_chessnum();//使用State封装的getchessnum效率更高
		if (chessnum > this->method_dividing)
		{//遍历空格
			for (int i = up; i <= down; i++)
				for (int j = left; j <= right; j++)
					if (array[i][j] == 0)
						for (int k = 0; k < 4; k++)
						{
							int x1 = i + dx[k], x2 = i + dx[k] * 2;
							int y1 = j + dy[k], y2 = j + dy[k] * 2;
							if (x1 >= up && x1 <= down &&
								x2 >= up && x2 <= down &&
								y1 >= left && y1 <= right &&
								y2 >= left && y2 <= right)
								if (array[x1][y1] == 1 && array[x2][y2] == 1)
								{
									array[i][j] = 1;
									array[x1][y1] = 0;
									array[x2][y2] = 0;

									int nextstep = temps.step;
									if (beforenum != x2 * 10 + y2)
										nextstep++;

									ConState ts(array, temps.route + this->num_to_string(x2, y2) + char((k ^ 1) + '0'), nextstep);
									if (!this->isrepeated_state(ts, visited_central))
										squ.push(ts);
									array[i][j] = 0;
									array[x1][y1] = 1;
									array[x2][y2] = 1;
								}
						}
		}
		else {
			//遍历棋子 forward
			for (int i = up; i <= down; i++)
				for (int j = left; j <= right; j++)
					if (array[i][j] == 1)
						for (int k = 0; k < 4; k++)
						{
							int x1 = i + dx[k], x2 = i + dx[k] * 2;
							int y1 = j + dy[k], y2 = j + dy[k] * 2;
							if (x1 >= up && x1 <= down &&
								x2 >= up && x2 <= down &&
								y1 >= left && y1 <= right &&
								y2 >= left && y2 <= right)
								if (array[x1][y1] == 1 && array[x2][y2] == 0)
								{
									array[i][j] = 0;
									array[x1][y1] = 0;
									array[x2][y2] = 1;

									int nextstep = temps.step;
									if (beforenum != i * 10 + j)
										nextstep++;

									ConState ts(array, temps.route + this->num_to_string(i, j) + char(k + '0'), nextstep);
									if (!this->isrepeated_state(ts, visited_central))
										squ.push(ts);
									array[i][j] = 1;
									array[x1][y1] = 1;
									array[x2][y2] = 0;
								}
						}
		}

	}
}
void Boardgame_central::OnlyOneLeft()
{
	//bfs求解最小剩余
	map<long long, string>state_map;
	queue<OriginState>squ;
	squ.push(OriginState(this->origin_state));
	char array[10][10];
	memcpy(array, this->chessboard, sizeof(this->chessboard));
	state_map[this->origin_state] = "";
	while (!squ.empty())
	{
		OriginState temps = squ.front();
		squ.pop();

		//解压
		this->state_to_array_unsecure(array, temps.state);
		int chessnum = temps.get_chessnum();//使用State封装的getchessnum效率更高
		if (chessnum == 1)
		{
			this->anwser_simple = state_map[temps.state];
			return;
		}
		else if (chessnum < this->chessnum_min)
		{
			this->anwser_simple = state_map[temps.state];
			this->chessnum_min = chessnum;
		}
		if (chessnum > this->method_dividing)
		{//遍历空格
			for (int i = up; i <= down; i++)
				for (int j = left; j <= right; j++)
					if (array[i][j] == 0)
						for (int k = 0; k < 4; k++)
						{
							int x1 = i + dx[k], x2 = i + dx[k] * 2;
							int y1 = j + dy[k], y2 = j + dy[k] * 2;
							if (x1 >= up && x1 <= down &&
								x2 >= up && x2 <= down &&
								y1 >= left && y1 <= right &&
								y2 >= left && y2 <= right)
								if (array[x1][y1] == 1 && array[x2][y2] == 1)
								{
									array[i][j] = 1;
									array[x1][y1] = 0;
									array[x2][y2] = 0;

									OriginState ts(array);
									if (!this->isrepeated_state(ts, state_map))
									{
										squ.push(ts);
										state_map[ts.state] = state_map[temps.state] + this->num_to_string(x2, y2) + char(k ^ 1 + '0');
									}
									array[i][j] = 0;
									array[x1][y1] = 1;
									array[x2][y2] = 1;
								}
						}
		}
		else {
			//遍历棋子 forward
			for (int i = up; i <= down; i++)
				for (int j = left; j <= right; j++)
					if (array[i][j] == 1)
						for (int k = 0; k < 4; k++)
						{
							int x1 = i + dx[k], x2 = i + dx[k] * 2;
							int y1 = j + dy[k], y2 = j + dy[k] * 2;
							if (x1 >= up && x1 <= down &&
								x2 >= up && x2 <= down &&
								y1 >= left && y1 <= right &&
								y2 >= left && y2 <= right)
								if (array[x1][y1] == 1 && array[x2][y2] == 0)
								{
									array[i][j] = 0;
									array[x1][y1] = 0;
									array[x2][y2] = 1;

									OriginState ts(array);
									if (!this->isrepeated_state(ts, state_map))
									{
										squ.push(ts);
										state_map[ts.state] = state_map[temps.state] + this->num_to_string(i, j) + char(k+'0');
									}
									array[i][j] = 1;
									array[x1][y1] = 1;
									array[x2][y2] = 0;
								}
						}
		}

	}
}
void Boardgame_central::OnlyOneLeft_BycicleBFS()
{
	queue<OriginState>forward;
	queue<OriginState>backward;
	map<long long, string>state_map;
	map<long long, string>back_map;

	forward.push(OriginState(this->origin_state));
	backward.push(OriginState(this->destin_state));
	char s[OriginState::board_maxlength][OriginState::board_maxlength];
	memcpy(s, this->chessboard, sizeof(chessboard));
	while ((!forward.empty()) && (!backward.empty()))
	{


		long long ostate = forward.front().state;
		if (back_map.count(ostate))
		{
			//将反串拼接至正串中
			string tempstring = back_map[ostate];
			state_map[this->destin_state] = state_map[ostate];
			for (int i = tempstring.size() - 3; i >= 0; i -= 3)
			{
				state_map[this->destin_state] += tempstring.substr(i, 3);
			}
			this->anwser_simple = state_map[this->destin_state];
			this->chessnum_min = 1;
			return;
		}
		//if (ostate!=this->origin_state&&state_map.count(ostate)) {
		//	forward.pop();
		//	continue;
		//}
		forward.front().array_to_state_efficient(s, this->up, down, left, right);
		int forwardnum = forward.front().get_chessnum();
		forward.pop();
		//正向

		if (forwardnum > this->method_dividing)
		{//遍历空格
			for (int i = up; i <= down; i++)
				for (int j = left; j <= right; j++)
					if (s[i][j] == 0)
						for (int k = 0; k < 4; k++)
						{
							int x1 = i + dx[k], x2 = i + dx[k] * 2;
							int y1 = j + dy[k], y2 = j + dy[k] * 2;
							if (x1 >= up && x1 <= down &&
								x2 >= up && x2 <= down &&
								y1 >= left && y1 <= right &&
								y2 >= left && y2 <= right)
								if (s[x1][y1] == 1 && s[x2][y2] == 1)
								{
									s[i][j] = 1;
									s[x1][y1] = 0;
									s[x2][y2] = 0;
									OriginState temps = OriginState(s,up,down,left,right);
									if (!this->isrepeated_state(temps,state_map))//表中没有记录该状态 且没有等效状态
									{
										state_map[temps.state] = state_map[ostate] + this->num_to_string(x2, y2) + char(k ^ 1 + '0');
										//cout << "forward" << state_map[temps.state]<<endl;
										forward.push(temps);
									}
									else {
										//表中已记录该状态，可以什么也不做，也可以比较步数，步数小则覆盖
									}

									s[i][j] = 0;
									s[x1][y1] = 1;
									s[x2][y2] = 1;
								}
						}
		}
		else {
			//遍历棋子 forward
			for (int i = up; i <= down; i++)
				for (int j = left; j <= right; j++)
					if (s[i][j] == 1)
						for (int k = 0; k < 4; k++)
						{
							int x1 = i + dx[k], x2 = i + dx[k] * 2;
							int y1 = j + dy[k], y2 = j + dy[k] * 2;
							if (x1 >= 0 && x1 < 7 &&
								x2 >= 0 && x2 < 7 &&
								y1 >= 0 && y1 < 7 &&
								y2 >= 0 && y2 < 7)
								if (s[x1][y1] == 1 && s[x2][y2] == 0)
								{
									s[i][j] = 0;
									s[x1][y1] = 0;
									s[x2][y2] = 1;
									OriginState temps = OriginState(s);
									if (!this->isrepeated_state(temps, state_map))//表中没有记录该状态 且没有等效状态
									{
										state_map[temps.state] = state_map[ostate] + this->num_to_string(i, j) + char(k + '0');
										//cout << "forward" << state_map[temps.state]<<endl;
										forward.push(temps);
									}
									else {
										//表中已记录该状态，可以什么也不做，也可以比较步数，步数小则覆盖
									}

									s[i][j] = 1;
									s[x1][y1] = 1;
									s[x2][y2] = 0;
								}
						}
		}
		ostate = backward.front().state;
		if (state_map.count(ostate))
		{
			string tempstring = back_map[ostate];
			state_map[this->destin_state] = state_map[ostate];
			for (int i = tempstring.size() - 3; i >= 0; i -= 3)
			{
				state_map[this->destin_state] += tempstring.substr(i, 3);
			}
			this->anwser_simple = state_map[this->destin_state];
			this->chessnum_min = 1;
			return;
		}
		/*	if (ostate!=this->destin_state&&backmap.count(ostate)) {
				backward.pop();
				continue;
			}*/
		backward.front().array_to_state_efficient(s, this->up, down, left, right);
		int backwardnum = backward.front().get_chessnum();
		backward.pop();
		if (backwardnum < this->method_dividing)
		{//遍历棋子
			for (int i = up; i <= down; i++)
				for (int j = left; j <= right; j++)
					if (s[i][j] == 1)
						for (int k = 0; k < 4; k++)
						{
							int x1 = i + dx[k], x2 = i + dx[k] * 2;
							int y1 = j + dy[k], y2 = j + dy[k] * 2;
							if (x1 >= 0 && x1 < 7 &&
								x2 >= 0 && x2 < 7 &&
								y1 >= 0 && y1 < 7 &&
								y2 >= 0 && y2 < 7)
								if (s[x1][y1] == 0 && s[x2][y2] == 0)
								{
									s[i][j] = 0;
									s[x1][y1] = 1;
									s[x2][y2] = 1;
									OriginState temps = OriginState(s);
									if (!this->isrepeated_state(temps, back_map))//表中没有记录该状态 且没有等效状态
									{
										back_map[temps.state] = back_map[ostate] + this->num_to_string(x2, y2) + char(k ^ 1 + '0');
										//cout << "forward" << state_map[temps.state]<<endl;
										backward.push(temps);
									}
									else {
										//表中已记录该状态，可以什么也不做，也可以比较步数，步数小则覆盖
									}

									s[i][j] = 1;
									s[x1][y1] = 0;
									s[x2][y2] = 0;
								}
						}
		}
		else {
			//遍历空格 backward
			for (int i = up; i <= down; i++)
				for (int j = left; j <= right; j++)
					if (s[i][j] == 0)
						for (int k = 0; k < 4; k++)
						{
							int x1 = i + dx[k], x2 = i + dx[k] * 2;
							int y1 = j + dy[k], y2 = j + dy[k] * 2;
							if (x1 >= 0 && x1 < 7 &&
								x2 >= 0 && x2 < 7 &&
								y1 >= 0 && y1 < 7 &&
								y2 >= 0 && y2 < 7)
								if (s[x1][y1] == 0 && s[x2][y2] == 1)
								{
									s[i][j] = 1;
									s[x1][y1] = 1;
									s[x2][y2] = 0;
									OriginState temps = OriginState(s);
									if (!this->isrepeated_state(temps, back_map))//表中没有记录该状态 且没有等效状态
									{
										back_map[temps.state] = back_map[ostate] + this->num_to_string(i, j) + char(k + '0');
										//cout << "forward" << state_map[temps.state]<<endl;
										backward.push(temps);
									}
									else {
										//表中已记录该状态，可以什么也不做，也可以比较步数，步数小则覆盖
									}

									s[i][j] = 0;
									s[x1][y1] = 0;
									s[x2][y2] = 1;
								}
						}
		}

	}//while
}