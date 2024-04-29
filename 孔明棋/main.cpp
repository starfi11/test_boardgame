#include"solitaire.h"
#include"Boardgame_simple.h"

int main()
{
	/*string s=Boardgame_simple::read_string();
	cout << s;*/

	/*string s = "..***...0..***...0*******.0***o***.0*******.0..***...0..***...0........";
	Boardgame_simple case2(s);
	case2.print_simplecase();*/




	//char array[10][10];
	//Boardgame_central centralcase2(Boardgame_simple::read_string());
	//centralcase2.print_centralcase();
	////centralcase2.LeastStepsSolve();
	////centralcase2.print_LeastStepsSolve();
	//memcpy(array, centralcase2.chessboard, sizeof(centralcase2.chessboard));
	//centralcase2.turn_unsecure(array);
	//centralcase2.print_chessboard(array);

	//centralcase2.turn_unsecure(array);
	//centralcase2.print_chessboard(array);

	//centralcase2.turn_unsecure(array);
	//centralcase2.print_chessboard(array);

	//centralcase2.turn_unsecure(array);
	//centralcase2.print_chessboard(array);

	//return 0;

	int op = 1;
	while (op)
	{
		cout << "1.simple棋局创建" << endl << "2.central棋局创建" << endl << "3.central棋局，内存优化求解" << endl;
		cout << "4.central more less memory" << endl;
		cout << "5.central_BFS棋局创建，搜索最少剩余的一个解" << endl;
		cout << "6.BycicleBFS" << endl;
		scanf("%d", &op);
		switch (op) {

		case 0:
			break;
		case 1: {
			Boardgame_simple simplecase1(Boardgame_simple::read_string());
			simplecase1.print_simplecase();
			simplecase1.OnlyOneLeft();
			simplecase1.print_OnlyOneLeft();
			break;
		}
		case 2: {
			Boardgame_central centralcase2(Boardgame_simple::read_string(), 18);
			centralcase2.print_centralcase();
			centralcase2.LeastStepsSolve();
			centralcase2.print_LeastStepsSolve();
			break;
		}
		case 3: {
			Boardgame_central centralcase3(Boardgame_simple::read_string());
			centralcase3.print_centralcase();
			centralcase3.LeastStepsSolve_less_memory();
			centralcase3.print_LeastStepsSolve();
			break;
		}
		case 4: {
			Boardgame_central centralcase4(Boardgame_simple::read_string());
			centralcase4.print_centralcase();
			centralcase4.LeastStepsSolve_more_less_memory();
			centralcase4.print_LeastStepsSolve();
			break;
		}
		case 5: {
			Boardgame_central centralcase5(Boardgame_simple::read_string());
			centralcase5.print_simplecase();
			centralcase5.OnlyOneLeft();
			centralcase5.print_OnlyOneLeft();
			break;
		}
		case 6: {
			Boardgame_central centralcase6(Boardgame_simple::read_string());
			centralcase6.print_simplecase();
			centralcase6.OnlyOneLeft_BycicleBFS();
			centralcase6.print_OnlyOneLeft();
		}
		}//switch
	}//while
	return 0;
}