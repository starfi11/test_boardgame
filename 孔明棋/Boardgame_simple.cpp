#include"Boardgame_simple.h"
void Boardgame_simple::print_route_continuous(string s)
{
	char array[State::board_maxlength][State::board_maxlength];
	memcpy(array, this->chessboard, sizeof(chessboard));
	int x, y, direct, ans = 0;
	int constep = 0;
	int beforenum = -1;
	for (int i = 0; i <= s.size() - 3; i += 3)
	{
		cout << "step" <<constep<<"."<< ans << endl;
		this->print_chessboard(array);
		x = s[i] - '0';
		y = s[i + 1] - '0';
		direct = s[i + 2] - '0';
		array[x][y] = 0;
		array[x + Boardgame_simple::dx[direct]][y + dy[direct]] = 0;
		array[x + dx[direct] * 2][y + dy[direct] * 2] = 1;
		ans++;
		if (x * 10 + y != beforenum)
			constep++;
		beforenum = (x + dx[direct] * 2) * 10 + y + dy[direct] * 2;
	}
	cout << "step" << constep << "." << ans << endl;
	this->print_chessboard(array);
}
long long Boardgame_simple::mirror_level(char array[State::board_maxlength][State::board_maxlength])
{
	//上下翻转
	int index1 = this->up, index2 = this->down;
	while (index2 > index1)
	{
		for (int i = left; i <= right; i++)
			swap(array[index1][i], array[index2][i]);
		index1++;
		index2--;
	}
	return this->array_to_state(array);
}
long long Boardgame_simple::array_to_state(char array[State::board_maxlength][State::board_maxlength])
{
	long long ans = 0;
	for(int i=this->up;i<=this->down;i++)
		for(int j=left;j<=right;j++)
			if (array[i][j] != -1)
			{
				ans *= 2;
				ans += array[i][j];
			}
	return ans;
}
void Boardgame_simple::state_to_array_unsecure(char array[State::board_maxlength][State::board_maxlength], long long state)
//在初始棋局的基础上将压缩码解压
{
	//仅当array是一个具体棋盘时解压正确，没有错误保障
	for(int i=this->down;i>=this->up;i--)
		for (int j = this->right; j >= this->left; j--)
			if (array[i][j] != -1)
			{
				array[i][j] = state % 2;
				state /= 2;
			}
}
void Boardgame_simple::print_OnlyOneLeft()
{
	cout << "最少剩余:" << this->chessnum_min << endl;
	cout << "路径压缩串:" << this->anwser_simple << endl;
	cout << "路径压缩串:" << this->route_transfer_string() << endl;
	cout << "路径输出:" << endl;
	this->print_route();
}
void Boardgame_simple::print_route(string s)
{
	char array[State::board_maxlength][State::board_maxlength];
	memcpy(array, this->chessboard, sizeof(chessboard));
	int x, y, direct, ans = 0;
	for (int i = 0; i <= s.size() - 3; i+=3)
	{
		cout << "step" << ans << endl;
		this->print_chessboard(array);
		x = s[i] - '0';
		y = s[i + 1] - '0';
		direct = s[i + 2]-'0';
		array[x][y] = 0;
		array[x + Boardgame_simple::dx[direct]][y + dy[direct]] = 0;
		array[x + dx[direct] * 2][y + dy[direct] * 2] = 1;
		ans++;
	}
	cout << "step" << ans << endl;
	this->print_chessboard(array);
}
int Boardgame_simple::get_chessnum(char array[State::board_maxlength][State::board_maxlength])
{
	int ans = 0;
	for (int i = up; i <= down; i++)
		for (int j = left; j <= right; j++)
			if (array[i][j] == 1)
				ans++;
	return ans;
}
void Boardgame_simple::OnlyOneLeft()
{
	//dfs
	char array[State::board_maxlength][State::board_maxlength];
	memcpy(array, this->chessboard, sizeof(chessboard));
	int chessnum = get_chessnum(array);
	dfs(array,chessnum,"");
}
bool Boardgame_simple::dfs(char array[State::board_maxlength][State::board_maxlength],int chessnum,string route)
{
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
	if (chessnum > this->method_dividing)
	{//遍历空格
		for (int i = up; i <= down; i++)
			for (int j = left; j <= right; j++)
				if (array[i][j] == 0)
					for (int k = 0; k < 4; k++)
					{
						int x1 = i + dx[k], x2 = i + dx[k] * 2;
						int y1 = j + dy[k], y2 = j + dy[k] * 2;
						if (x1 >= up && x1 <=down &&
							x2 >= up && x2 <=down &&
							y1 >= left && y1 <= right &&
							y2 >= left && y2 <= right)
							if (array[x1][y1] == 1 && array[x2][y2] == 1)
							{
								array[i][j] = 1;
								array[x1][y1] = 0;
								array[x2][y2] = 0;
								
								if (dfs(array, chessnum - 1,route+Boardgame_simple::num_to_string(x2,y2)+char('0' + k ^ 1  )))
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

								if (dfs(array, chessnum - 1, route+Boardgame_simple::num_to_string(i, j) + char(k+'0')))
									return 1;

								array[i][j] = 1;
								array[x1][y1] = 1;
								array[x2][y2] = 0;
							}
					}
	}
	return 0;
}
void Boardgame_simple::print_simplecase()
{
	printf("origin_state = %lld\n", origin_state);
	printf("up    = %d\n", up);
	printf("down  = %d\n", down);
	printf("left  = %d\n", left);
	printf("right = %d\n", right);
	this->print_chessboard();
}
void Boardgame_simple::print_chessboard(char array[State::board_maxlength][State::board_maxlength])
{
	cout << "ChessBoard:" << endl;
	for (int i = up; i <= down; i++)
	{
		for (int j = left; j <= right; j++)
			if (array[i][j] == -1)
				cout << ". ";
			else if (array[i][j] == 1)
				cout << "* ";
			else if (array[i][j] == 0)
				cout << "o ";
			else printf("%d", array[i][j]);
		cout << endl;
	}
}
void Boardgame_simple::print_chessboard()
{
	this->print_chessboard(this->chessboard);
}
string Boardgame_simple::read_string()
{

	cout << "输入棋盘格局,可有空格,以EOF结尾" << endl;
	string s;
	string ans;
	getline(cin, ans);
	while (getline(cin,s))
	{
		ans += '0';
		ans += s;
	}
	return ans;
}
Boardgame_simple::Boardgame_simple(string s)//健壮性较弱的构造函数
{	
	//cout << s << endl;
	char temp = -1;
	int index = 0;
	int row = 0;
	bool sig = 0;
	int ans = 0;
	int chessans = 0;
	up = 0;
	down = State::board_maxlength - 1;
	left = 0;
	right = State::board_maxlength - 1;
	memset(this->chessboard, -1, sizeof(this->chessboard));
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] == '.')
			temp = -1;
		else if (s[i] == '*')
		{
			ans++;
			chessans++;
			temp = 1;
		}
		else if (s[i] == 'o')
		{
			ans++;
			temp = 0;
		}
		else if (s[i] == '0')
		{
			index = 0;
			row++;
			continue;
		}
		else continue;
		this->chessboard[row][index] = temp;
		index++;
	}
	this->squarenum = ans;
	this->method_dividing = ans / 2;
	this->chessnum_min = chessans;
	for (int i = 0; i < State::board_maxlength; i++)
	{
		for (int j = 0; j < State::board_maxlength; j++)
			if (chessboard[i][j] != -1)
			{
				up = i;
				sig = 1;
				break;
			}
		if (sig)
			break;
	}
	sig = 0;
	for (int i = State::board_maxlength - 1; i >= 0; i--)
	{
		for (int j = 0; j < State::board_maxlength; j++)
			if (chessboard[i][j] != -1)
			{
				down = i;
				sig = 1;
				break;
			}
		if (sig)
			break;
	}
	sig = 0;
	for (int i = 0; i < State::board_maxlength; i++)
	{
		for (int j = 0; j < State::board_maxlength; j++)
			if (chessboard[j][i] != -1)
			{
				left = i;
				sig = 1;
				break;
			}
		if (sig)
			break;
	}
	sig = 0;
	for (int i = State::board_maxlength - 1; i >= 0; i--)
	{
		for (int j = 0; j < State::board_maxlength; j++)
			if (chessboard[j][i] != -1)
			{
				right = i;
				sig = 1;
				break;
			}
		if (sig)
			break;
	}
	this->origin_state = State::array_to_state_unsecure(this->chessboard);
}