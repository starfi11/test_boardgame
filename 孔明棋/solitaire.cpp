#include"solitaire.h"
long long OriginState::array_to_state_efficient(char array[10][10], int up, int down, int left, int right)
{
	long long ans = 0;
	for(int i=up;i<=down;i++)
		for(int j=left;j<=right;j++)
			if (array[i][j] != -1)
			{
				ans *= 2;
				ans += array[i][j];
			}
	return ans;
}
int OriginState::get_chessnum()//计算棋局中棋子的个数
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
long long OriginState::array_to_state_unsecure(char array[board_maxlength][board_maxlength])
{
	long long temp = 0;
	for (int i = 0; i < board_maxlength; i++)
		for (int j = 0; j < board_maxlength; j++)
		{
			if (array[i][j] == -1)
				continue;
			temp *= 2;
			temp += array[i][j];
		}
	return temp;
}
long long OriginState::string_to_state(string s)
{
	long long ans = 0;
	for (int i = 0; i < s.size(); i++)
	{
		ans *= 2;
		ans += s[i] - '0';
	}
	return ans;
}
string OriginState::array_to_string(char array[board_maxlength][board_maxlength])
{
	string ans;
	for (int i = 0; i < board_maxlength; i++)
		for (int j = 0; j < board_maxlength; j++)
			if (array[i][j] != -1)
				ans += array[i][j] + '0';
	return ans;
}
void OriginState::array_print(char s[board_maxlength][board_maxlength])
{
	for (int i = 0; i < board_maxlength; i++)
	{
		for (int j = 0; j < board_maxlength; j++)
		{
			if (s[i][j] == -1)
				cout << ". ";
			else if (s[i][j] == 0)
				cout << "o ";
			else if (s[i][j] == 1)
				cout << "* ";
		}
		cout << endl;
	}
}