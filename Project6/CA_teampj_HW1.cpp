#include "pch.h"

int main()
{
	operat oper(-7,-3);
	//oper.start_calculate();
	//oper.multiple();
	//oper.division();

	while (true) {
		cout << "두 정수 입력 : ";
		int a, b;
		cin >> a >> b;
		oper.initial(a, b);
		oper.start_calculate();

		cout << "계속 하시려면 'Y'를 입력하세요";
		char con;
		cin >> con;
		if (!(con == 'y' || con == 'Y')) 
			break;
	}
}

