#define _CRT_SECURE_NO_WARNINGS
#include"solitaire.h"
#include"Bidirectional_BFS.h"
int main()
{

	long long state1 = State::string_to_state("111111111111111101111111111111111");
	//cout << state1 << endl;
	State s1(state1);
	State s2(State::state_final);
//	char s[7][7];
	//s1.transfer_to_array(s);
	//s1.print();
	//State s3(s);
	//s3.print();
	//s1.print();
	//s2.print();
	//return 0;
	long long state3 = State::string_to_state("100101000100000000100000000000000");
	long long state4 = State::string_to_state("000001001110011110000110000100011");
	State s3(state3);
	/*s3.print();
	char s[7][7];
	s3.transfer_to_array(s);*/
	/*for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (s[i][j] == -1)
				cout << "2 ";
			else if (s[i][j] == 0)
				cout << "0 ";
			else if (s[i][j] == 1)
				cout << "1 ";
		}
		cout << endl;
	}*/
	//return 0;


	//long long temp;
	//while (scanf("%lld", &temp) > 0)
	//{
	//	State s(temp);
	//	s.print();
	//}
	//return 0;
	Bidirectional_BFS case3(state4, State::state_final);
	cout << case3.solve() << endl;
	cout << case3.state_map[State::state_final];
	case3.route_print();
	return 0;
	Bidirectional_BFS case1(state1, State::state_final);
	cout<<case1.solve()<<endl;
	cout << case1.state_map[case1.destin_state]<<endl;
	
	Bidirectional_BFS case2(state3, State::state_final);
	cout << case2.solve()<<endl;
	cout << case2.state_map[case2.destin_state];
	return 0;
}