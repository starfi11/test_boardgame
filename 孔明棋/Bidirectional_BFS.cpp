#include"Bidirectional_BFS.h"

bool Bidirectional_BFS::solve()
{
	queue<State>forward;
	queue<State>backward;
	forward.push(State(this->origin_state));
	backward.push(State(this->destin_state));
	char s[7][7];
	while ((!forward.empty()) && (!backward.empty()))
	{


		long long ostate = forward.front().state;
		if (backmap.count(ostate))
		{
			//将反串拼接至正串中
			string tempstring = backmap[ostate];
			state_map[this->destin_state] = state_map[ostate];
			for (int i = tempstring.size() - 3; i >= 0; i -= 3)
			{
				state_map[this->destin_state] += tempstring.substr(i, 3);
			}
			return 1;
		}
		//if (ostate!=this->origin_state&&state_map.count(ostate)) {
		//	forward.pop();
		//	continue;
		//}
		forward.front().transfer_to_array(s);
		int forwardnum = forward.front().true_count();
		forward.pop();
		//正向

		if (1)
		{//遍历空格
			for (int i = 0; i < 7; i++)
				for (int j = 0; j < 7; j++)
					if (s[i][j] == 0)
						for (int k = 0; k < 4; k++)
						{
							int x1 = i + dx[k], x2 = i + dx[k] * 2;
							int y1 = j + dy[k], y2 = j + dy[k] * 2;
							if (x1 >= 0 && x1 < 7 &&
								x2 >= 0 && x2 < 7 &&
								y1 >= 0 && y1 < 7 &&
								y2 >= 0 && y2 < 7)
								if (s[x1][y1] == 1 && s[x2][y2] == 1)
								{
									s[i][j] = 1;
									s[x1][y1] = 0;
									s[x2][y2] = 0;
									State temps = State(s);
									if (!this->state_map.count(temps.state))//表中没有记录该状态
									{
										this->state_map[temps.state] = state_map[ostate] + two_string(numarray[x2][y2]) + direction[k^1];
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
		ostate = backward.front().state;
		if (state_map.count(ostate))
		{
			string tempstring = backmap[ostate];
			state_map[this->destin_state] = state_map[ostate];
			for (int i = tempstring.size() - 3; i >= 0; i -= 3)
			{
				state_map[this->destin_state] += tempstring.substr(i, 3);
			}
			return 1;
		}
	/*	if (ostate!=this->destin_state&&backmap.count(ostate)) {
			backward.pop();
			continue;
		}*/
		backward.front().transfer_to_array(s);
	
		int backwardnum = backward.front().true_count();
		backward.pop();
		if (1)
		{//遍历棋子
			for (int i = 0; i < 7; i++)
				for (int j = 0; j < 7; j++)
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
									State temps = State(s);
									if (!this->backmap.count(temps.state))//表中没有记录该状态
									{
										this->backmap[temps.state] = backmap[ostate] + two_string(numarray[x2][y2]) + direction[k ^ 1];
										//cout << "backward" << backmap[temps.state]<<endl;
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

	}//while
	return 0;
}
void Bidirectional_BFS::route_print()
{
	if (!this->state_map.count(this->destin_state))
	{
		cout << "false" << endl;
		return;
	}
	char s[7][7];
	State temps(this->origin_state);
	temps.transfer_to_array(s);
	int deep = 0;
	int temp, x, y, x1, x2, y1, y2;
	char ch;
	string route = this->state_map[this->destin_state];
	for (int i = 0; i <= this->state_map[this->destin_state].size() - 3; i += 3)
	{
		printf("第%d步:\n", deep);
		State::array_print(s);
		deep++;
		ch = route[i + 2];
		temp = route[i] - '0';
		temp *= 10;
		temp += route[i + 1] - '0';
		x = renumarray_x[temp];
		y = renumarray_y[temp];
		if (ch == 'u') {
			x1 = x - 1;
			x2 = x - 2;
			y1 = y;
			y2 = y;
		}
		else if (ch == 'd') {
			x1 = x + 1;
			x2 = x + 2;
			y1 = y;
			y2 = y;
		}
		else if (ch == 'l') {
			x1 = x;
			x2 = x;
			y1 = y-1;
			y2 = y-2;
		}
		else if (ch == 'r') {
			x1 = x;
			x2 = x;
			y1 = y + 1;
			y2 = y + 2;
		}
		s[x][y] = 0;
		s[x1][y1] = 0;
		s[x2][y2] = 1;
	}
	printf("第%d步:\n", deep);
	State::array_print(s);
}