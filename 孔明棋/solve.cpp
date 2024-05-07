#include"Boardgame_simple.h"
void Boardgame_central::print_statenum()
{

	long long ans = 0;
	map<long long, bool>visited_central;
	queue<OriginState>squ;
	squ.push(OriginState(this->origin_state));
	char array[10][10];
	memcpy(array, this->chessboard, 100);
	//正常
	while (!squ.empty())
	{
		OriginState temps = squ.front();
		squ.pop();

		ans++;
		this->state_to_array_unsecure(array, temps.state);
		visited_central[temps.state] = true;

		//cout << visited_central.size() << endl;
		//this->print_chessboard(array);
		//system("pause");
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

								//cout << "进化内" << endl;
								//this->print_chessboard(array);
								OriginState ts(array);
								visited_central[ts.state] = true;
								squ.push(ts);

								array[i][j] = 0;
								array[x1][y1] = 1;
								array[x2][y2] = 1;
							}
					}


	}
	cout << "共遍历" << ans << "状态" << endl;
	cout << "非重复状态共有" << visited_central.size() << endl;
}
struct ConState_cmp {
	bool operator()(const ConState& x, const ConState& y)
	{
		if (x.step+x.get_chessnum() > y.step+y.get_chessnum())
			return 1;
		return 0;
	}
};
void Boardgame_central::LeastStepsSolve_Astar()
{
	map<long long, int>visited;
	char array[10][10];
	memcpy(array, this->chessboard, sizeof(this->chessboard));



	priority_queue<ConState, vector<ConState>, ConState_cmp>pq;
	pq.push(ConState(this->chessboard));
	while (!pq.empty())
	{
		ConState temps = pq.top();
		pq.pop();
		int chessnum = temps.get_chessnum();//使用State封装的getchessnum效率更高
		if (temps.state == this->destin_state)
		{
			if (temps.step > this->step_min)
				break;
			this->anwser_central.push_back(temps.route);
			this->step_min = temps.step;
			continue;
		}
		//else if (this->isrepeated_state(temps, visited_central))//避免重复进行 一个状态扩展到其他状态 的操作
		//	continue;//理论上这个重复判断应该只有一处，但为了极限减少内存，两处都有
		/*产生新状态*/

		//记录
		visited[temps.state] = temps.step;

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
									if (!this->isrepeated_state(ts, visited))
										pq.push(ts);
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
									if (!this->isrepeated_state(ts, visited))
										pq.push(ts);
									array[i][j] = 1;
									array[x1][y1] = 1;
									array[x2][y2] = 0;
								}
						}
		}



	}
}
void Boardgame_central::OnlyOneLeft_deequal()
{
	//dfs
	map<long long, bool>visited;
	char array[State::board_maxlength][State::board_maxlength];
	memcpy(array, this->chessboard, sizeof(chessboard));//这种sizeof正常使用
	int chessnum = get_chessnum(array);
	dfs_deequal(array, chessnum, "", visited);
}
bool Boardgame_central::dfs_deequal(char array[State::board_maxlength][State::board_maxlength], int chessnum, string route, map<long long, bool>& visited)
{
	//要求棋局是正方形
	if (chessnum == 1)
	{
		this->anwser_simple = route;
		this->chessnum_min = 1;
		return 1;
	}
	else if (chessnum < 1)
	{
		cout << "chessnum error:" << chessnum << endl;
		return 0;
	}
	else if (this->chessnum_min > chessnum) {
		this->anwser_simple = route;
		this->chessnum_min = chessnum;
	}
	//将该状态的所有等效状态加入map

	//对应的八种状态都未在表中出现
	long long tempstate;
	char temparray[10][10];
	memcpy(temparray, array, sizeof(temparray));
	tempstate = Boardgame_simple::array_to_state(temparray);
	visited[tempstate] = 1;
	for (int i = 0; i < 3; i++)
	{
		tempstate = this->turn_unsecure(temparray);
		visited[tempstate] = 1;
	}

	//翻转后再旋转
	tempstate = this->mirror_level(temparray);
	visited[tempstate] = 1;

	for (int i = 0; i < 3; i++)
	{
		tempstate = this->turn_unsecure(temparray);
		visited[tempstate] = 1;
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
								if (!visited.count(Boardgame_simple::array_to_state(array)))
									if (dfs_deequal(array, chessnum - 1, route + Boardgame_simple::num_to_string(x2, y2) + char('0' + k ^ 1), visited))
										return 1;
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
								if (!visited.count(Boardgame_simple::array_to_state(array)))
									if (dfs_deequal(array, chessnum - 1, route + Boardgame_simple::num_to_string(i, j) + char(k + '0'), visited))
										return 1;

								array[i][j] = 1;
								array[x1][y1] = 1;
								array[x2][y2] = 0;
							}
					}
	}
	return 0;
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
										state_map[ts.state] = state_map[temps.state] + this->num_to_string(i, j) + char(k + '0');
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
		this->state_to_array_unsecure(s, forward.front().state);
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
									OriginState temps = OriginState(s, up, down, left, right);
									if (!this->isrepeated_state(temps, state_map))//表中没有记录该状态 且没有等效状态
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
		this->state_to_array_unsecure(s, backward.front().state);
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