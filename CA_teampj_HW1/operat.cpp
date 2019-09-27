#include "pch.h"
operat::operat(int a, int b) :n1(a), n2(b)
{
	num1 = tobinary(a, BIT_SIZE);
	num2 = tobinary(b, BIT_SIZE);
}

void operat::initial(int a, int b)
{
	n1 = a;
	n2 = b;
	num1 = tobinary(a, BIT_SIZE);
	num2 = tobinary(b, BIT_SIZE);
}

operat::~operat()
{
}

bool * operat::complement(bool *num,int bitn)
{
	bool *comple = new bool[bitn];
	for (int i = 0; i < bitn; i++) {
		comple[i] = !num[i];
	}
	for (int i = 0; i < bitn; i++) {
		comple[i] = !comple[i];
		if (comple[i] == 1)
			break;
	}

	return comple;
}

bool * operat::tobinary(int n ,int bitn)
{
	bool n_posit = n >= 0;
	bool *binary = new bool[bitn] {false, };
	for (int i = 0; i < BIT_SIZE - 1; i++) {
		binary[i] = n % 2;
		n /= 2;
		if (n == 0) break;
	}
	if (!n_posit)
		return complement(binary,bitn);
	else
		return binary;
}

int operat::frombinary(bool * num, int bitn)
{
	 int n = 0;
   int two_ = 1;
   
   if (num[bitn - 1]) {
	  num = complement(num, bitn);
      two_ = -1;
   }
   
   for (int i = 0; i < BIT_SIZE; i++) {
	   if (num[i])
		   n += two_;
	   two_ *= 2;
   }
   return n;
}

void operat::print(bool * num, int bitn)
{
	for (int i = bitn - 1; i >= 0; i--) {
		if (num[i]) {
			cout << "1 ";
		}
		else
			cout << "0 ";
	}
}


void operat::start_calculate()
{
	addsub_printf();
	system("pause");
	cout << "\n\n" << "∞ˆº¿ Ω««‡\n";
	multiple();
}

bool * operat::add(bool * num1, bool * num2, int bitn)
{
	//∫¥∑ƒ ∞°ªÍ±‚ bitn∞≥
	bool *add_result = new bool[bitn];
	for (int i = 0; i < bitn; i++) {
		add_result[i] = num1[i] ^ num2[i] ^ Carry[i];
		Carry[i + 1] = ((num1[i] ^ num2[i]) &   Carry[i]) | (num1[i] & num2[i]);
	}
	//flag ø¨ªÍ
	flag_c = Carry[bitn];
	flag_s = add_result[bitn - 1];

	flag_z = true;
	for (int i = 0; i < bitn; i++) {
		if (add_result[i] == true) {
			flag_z = false;
			break;
		}
	}
	flag_v = Carry[bitn] ^ Carry[bitn - 1];
	return add_result;
}

bool * operat::subtract(bool * num1, bool * num2, int bitn)
{
	bool *sub_num = complement(num2, bitn);
	return add(num1, sub_num, bitn);

}

void operat::addsub_printf()
{
	cout << "µ°º¿ Ω««‡ \n";
	result = add(num1, num2, BIT_SIZE);
	cout << n1 << " + (" << n2 << ") : " << endl;
	cout << "   num1 : "; print(num1,BIT_SIZE); cout << endl;
	cout << "+  num2 : "; print(num2,BIT_SIZE); cout << endl;
	cout << "============================================\n";
	cout << " result : "; print(result,BIT_SIZE);  cout << " = 10¡¯ºˆ : " << frombinary(result,BIT_SIZE) << endl;

	cout << "V : " << flag_v << ", Z : " << flag_z << ", S : " << flag_s << ", C : " << flag_c << endl;
	cout << "ª¨º¿ Ω««‡ \n";
	delete result;
	result = subtract(num1, num2, BIT_SIZE);
	cout << n1 << " + (" << -n2 << ") : " << endl;
	cout << "   num1 : "; print(num1, BIT_SIZE); cout << endl;
	cout << "-  num2 : "; print(num2, BIT_SIZE); cout << endl;
	cout << "============================================\n";
	cout << " result : "; print(result, BIT_SIZE);  cout << " = 10¡¯ºˆ : " << frombinary(result, BIT_SIZE) << endl;

	cout << "V : " << flag_v << ", Z : " << flag_z << ", S : " << flag_s << ", C : " << flag_c << endl;

}

void operat::multiple()
{
	//num1 ««Ω¬ºˆM num2 Ω¬ºˆQ , X_result A+Q;
	cout << n1 << " X " << n2 << endl;
	bool *X_result = new bool[(BIT_SIZE * 3)]{ false, };
	bool Q0 = false;
	bool *Q = X_result;
	bool *A = X_result + BIT_SIZE; //A¿« ¿ßƒ°
	for (int i = 0; i < BIT_SIZE; i++) {
		Q[i] = num2[i];
	}
	cout << setw(2 * BIT_SIZE) << "A   " << "//" << setw(2 * BIT_SIZE) << "Q" << "//" << "Q0/" << setw(2 * BIT_SIZE) << "M   " << "ø¨ªÍ ªÛ≈¬ " << endl;
	print(A, BIT_SIZE); cout << "//"; print(Q, BIT_SIZE); cout << "//"; cout << int(Q0); cout << "//"; print(num1,BIT_SIZE);
	cout << "\n==================================================================================================================================================================================================\n";
	for (int i = 0; i < BIT_SIZE; i++) {
		if (Q[0] & (!Q0)) {
			memcpy(A, subtract(A, num1, BIT_SIZE), BIT_SIZE);
			print(A, BIT_SIZE); cout << "//"; print(Q, BIT_SIZE); cout << "//"; cout << int(Q0); cout << "//"; print(num1, BIT_SIZE);
			cout << "|| A <- A - M \n";
		}
		else if ((!Q[0])&Q0) {
			memcpy(A, add(A, num1, BIT_SIZE), BIT_SIZE);
			print(A, BIT_SIZE); cout << "//"; print(Q, BIT_SIZE); cout << "//"; cout << int(Q0); cout << "//"; print(num1, BIT_SIZE);
			cout << "|| A <- A + M \n";
		}

		Q0 = Q[0];
		X_result[BIT_SIZE * 2] = X_result[BIT_SIZE * 2 - 1];
		X_result++; A++; Q++;
		/*for (int j = 0; j < (BIT_SIZE * 2) - 1; j++)
			X_result[i] = X_result[i + 1];*/
		
		print(A, BIT_SIZE); cout << "//"; print(Q, BIT_SIZE); cout << "//"; cout << int(Q0); cout << "//"; print(num1, BIT_SIZE);

		cout << "|| >> ¿Ãµø\n";
		cout << "==================================================================================================================================================================================================\n";
	}
	print(A, BIT_SIZE); cout << "//"; print(Q, BIT_SIZE); cout << "//"; cout << int(Q0); cout << "//"; print(num1, BIT_SIZE);
	cout << "Ω ¡¯ºˆ : " << frombinary(X_result, BIT_SIZE * 2) << endl;
}

void operat::division()
{
	
}
