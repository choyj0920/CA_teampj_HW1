#include "pch.h"
#pragma warning(disable:4996)

int main()
{
	ALU ALU_simul;
	//oper.start_calculate();
	//oper.multiple();
	//oper.division();

	int a, b;

	while (true) {
		cout << "두 정수 입력 : ";
		cin.clear();
		cin >> a >> b;
		if (cin.fail()) {
			cout << "입력한 숫자가 가능한 범위를 벗어났습니다" << endl;
			continue;
		}
		ALU_simul.Init(a, b);
		ALU_simul.Start_Calculate();

		cout << "계속 하시려면 'Y'를 입력하세요";
		char con;
		cin >> con;
		if (!(con == 'y' || con == 'Y')) 
			break;
	}
}

