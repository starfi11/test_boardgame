#include"Boardgame_simple.h"
#include<queue>
bool Boardgame_central::isrepeated_state(OriginState& st, map<long long, bool>& visited)
{
	//对应的八种状态都未在表中出现
	long long tempstate;
	char array[10][10];
	memcpy(array, this->chessboard, sizeof(chessboard));
	this->state_to_array_unsecure(array, st.state);
	/*cout << "初始" << endl;
	this->print_chessboard(array);*/

	//cout << "new" << endl;
	if (visited.count(st.state))
		return 1;
	for (int i = 0; i < 3; i++)
	{
		tempstate = this->turn_unsecure(array);
		/*cout << "旋转" << endl;
		this->print_chessboard(tempstate);*/
		if (visited.count(tempstate))
			return 1;
	}

	//翻转后再旋转
	tempstate = this->mirror_level(array);
	//cout << "旋转" << endl;
	//this->print_chessboard(tempstate);
	if (visited.count(tempstate))
		return 1;

	for (int i = 0; i < 3; i++)
	{
		tempstate = this->turn_unsecure(array);
		//cout << "旋转" << endl;
		//this->print_chessboard(tempstate);
		if (visited.count(tempstate))
			return 1;
	}

	return 0;
}
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

	return !(equalmin == 0x3f3f3f3f || st.step <= equalmin );//是否要加一
}
bool Boardgame_central::isrepeated_state(OriginState& st, map<long long, string>& visited)
{
	//对应的八种状态都未在表中出现
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
	x_boundary = (width / 2) - 1 + up;
	y_boundary = (width / 2) + left;
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
				temp = newx - up + left;
				newx = width - newy + left + up - 1;
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

	system("pause");
	for (int i = 0; i < this->anwser_central.size(); i++)
	{
		cout << "解法" << i + 1 << endl;
		this->print_route_continuous(anwser_central[i]);
	}
}
