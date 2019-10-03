#include "pch.h"

operat::operat(int a, int b) :n1(a), n2(b) //숫자 2진수 변환
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

bool * operat::complement(bool *num,int bitn) //보수 변환
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

bool * operat::tobinary(int n ,int bitn) //2진수 변환
{
	bool n_posit = n >= 0; //양수인지
	bool *binary = new bool[bitn] {false, };
	for (int i = 0; i < BIT_SIZE - 1; i++) {
		binary[i] = n % 2;
		n /= 2;
		if (n == 0) break;
	}
	if (!n_posit) //음수면
		return complement(binary,bitn); //보수 변환
	else
		return binary;
}

int operat::frombinary(bool * num, int bitn) //10진수 변환
{
   int n = 0; //계산 결과
   int two_ = 1;
   
   if (num[bitn - 1]) { //부호 비트가 1이면
	  num = complement(num, bitn); //음수여서 보수변환
      two_ = -1; // - 부호 처리 위해
   }
   
   for (int i = 0; i < BIT_SIZE; i++) {
	   if (num[i])
		   n += two_; 
	   two_ *= 2; //(2)n승 or (-2)n승 
   }
   return n;
}

void operat::print(bool * num, int bitn) //2진수 프린트
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
	system("pause");	cout << "\n\n" << "곱셈 실행\n";
	multiple();
	system("pause");	cout << "\n\n" << "나눗셈 실행\n";
	division();
}

bool * operat::add(bool * num1, bool * num2, int bitn)
{
	//병렬 가산기 bitn개
	bool *add_result = new bool[bitn];
	for (int i = 0; i < bitn; i++) {
		add_result[i] = num1[i] ^ num2[i] ^ Carry[i];
		Carry[i + 1] = ((num1[i] ^ num2[i]) &   Carry[i]) | (num1[i] & num2[i]);
	}
	//flag 연산
	flag_c = Carry[bitn];

	flag_s = add_result[bitn - 1];

	flag_z = true;
	for (int i = 0; i < bitn; i++) {
		if (add_result[i] == true) { //하나라도 1이면 z가 0으로 세팅
			flag_z = false;
			break;
		}
	}//z는 모든 결과값이 0이어야 1로 세팅

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
	cout << "덧셈 실행 \n";
	result = add(num1, num2, BIT_SIZE);
	cout << n1 << " + (" << n2 << ") : " << endl;
	cout << "   num1 : "; print(num1,BIT_SIZE); cout << endl;
	cout << "+  num2 : "; print(num2,BIT_SIZE); cout << endl;
	cout << "\n----------------------------------------------------------------------------------------\n";
	if(flag_v == 1)
		cout << "result : " << "오버플로우" << endl;
	else {
		cout << " result : "; print(result, BIT_SIZE);  cout << " = 10진수 : " << frombinary(result, BIT_SIZE) << endl;
	}
	cout << "V : " << flag_v << ", Z : " << flag_z << ", S : " << flag_s << ", C : " << flag_c << endl<<endl;

	 
	cout << "뺄셈 실행 \n";
	delete result;
	result = subtract(num1, num2, BIT_SIZE);
	cout << n1 << " - (" << n2 << ") : " << endl;
	cout << "   num1 : "; print(num1, BIT_SIZE); cout << endl;
	cout << "-  num2 : "; print(num2, BIT_SIZE); cout << endl;
	cout << "\n----------------------------------------------------------------------------------------\n";
	if (flag_v == 1)
		cout << "result : " << "오버플로우" << endl;
	else {
		cout << " result : "; print(result, BIT_SIZE);  cout << " = 10진수 : " << frombinary(result, BIT_SIZE) << endl;
	}

	cout << "V : " << flag_v << ", Z : " << flag_z << ", S : " << flag_s << ", C : " << flag_c << endl;

}

void operat::multiple()
{
	//num1 피승수M num2 승수Q , X_result A+Q;
	cout << n1 << " X " << n2 << endl;
	bool *X_result = new bool[(BIT_SIZE * 3)]{ false, };
	bool Q0 = false;
	bool *Q = X_result;
	bool *A = X_result + BIT_SIZE; //A의 위치

	for (int i = 0; i < BIT_SIZE; i++) {
		Q[i] = num2[i];
	}

	//가이드라인출력
	cout << setw(2 * BIT_SIZE) << "A   " << "//" << setw(2 * BIT_SIZE) << "Q" << "//" << "Q0/" << setw(2 * BIT_SIZE) << "M   " << "연산 상태 " << endl;
	print(A, BIT_SIZE); cout << "//"; print(Q, BIT_SIZE); cout << "//"; cout << int(Q0); cout << "//"; print(num1,BIT_SIZE);
	cout << "\n------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
	
	for (int i = 0; i < BIT_SIZE; i++) {
		if (Q[0] & (!Q0)) { //10인 경우
			memcpy(A, subtract(A, num1, BIT_SIZE), BIT_SIZE);
			print(A, BIT_SIZE); cout << "//"; print(Q, BIT_SIZE); cout << "//"; cout << int(Q0); //cout << "//"; print(num1, BIT_SIZE); 
			cout << " || A <- A - M \n";
		}
		else if ((!Q[0])&Q0) { //01인 경우
			memcpy(A, add(A, num1, BIT_SIZE), BIT_SIZE);
			print(A, BIT_SIZE); cout << "//"; print(Q, BIT_SIZE); cout << "//"; cout << int(Q0); //cout << "//"; print(num1, BIT_SIZE);
			cout << " || A <- A + M \n";
		}

		Q0 = Q[0];

		//산술적 우측 shift
		X_result[BIT_SIZE * 2] = X_result[BIT_SIZE * 2 - 1]; //최상위비트복사(산술적)
		X_result++; A++; Q++;

		/*for (int j = 0; j < (BIT_SIZE * 2) - 1; j++)
			X_result[i] = X_result[i + 1];*/
		
		print(A, BIT_SIZE); cout << "//"; print(Q, BIT_SIZE); cout << "//"; cout << int(Q0); //cout << "//"; print(num1, BIT_SIZE);

		if (i < BIT_SIZE - 1)
			cout << " >> right shift \n";
		else if (i == BIT_SIZE - 1)
			cout << " || 연산 완료" << endl;

		cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";

	}
	cout << "결과 : "; print(X_result, BIT_SIZE * 2); cout << endl; cout << "십진수 : " << frombinary(X_result, BIT_SIZE * 2) << endl;
}

void operat::division()
{
	//num1 피젯수Q, num2 젯수M , X_result A+Q;
	cout << n1 << " / " << n2 << endl;
	bool *X_result = new bool[(BIT_SIZE * 3)]{ false, };
	bool Q0 = 0; //연산 전후의 A부호가 같으면 1로 / 다르면 0으로 + 연산 결과 복귀 
	bool *Q = X_result ;
	bool *A = X_result + BIT_SIZE; //A의 위치
	bool ASign = false; 
	
	//피젯수Q 대입
	for (int i = 0; i < BIT_SIZE; i++) { 
		Q[i] = num1[i];
	}
	
	//Q가 음수면 A는 1111...
	if (Q[BIT_SIZE - 1]) { 
		for (int i = 0; i < BIT_SIZE; i++)
			A[i] = true;
	}

	//가이드라인출력
	cout << setw(2 * BIT_SIZE) << "A   " << "//" << setw(2 * BIT_SIZE) << "Q //" <<  setw(2 * BIT_SIZE) << "M   " << "연산 상태 " << endl; 
	print(A, BIT_SIZE); cout << "//"; print(Q, BIT_SIZE); cout << "//";  print(num2, BIT_SIZE);
	cout << "\n------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
	
	for (int i = 0; i < BIT_SIZE; i++) {

		// 좌측shift
		X_result--; A--; Q--;
			Q[0] = 0; //임시로 Q[0]에 0대입
		print(A, BIT_SIZE); cout << "//"; print(Q, BIT_SIZE); cout << " << left shift \n";

		//cout << "A부호 : " << A[BIT_SIZE] << "/ M부호 : " << num1[BIT_SIZE-1] << endl << endl;

		ASign = A[BIT_SIZE-1]; //A 이전 부호값 저장

		//A,M의 부호 비교
		if (!(A[BIT_SIZE-1] ^ num2[BIT_SIZE-1])) { //부호가 같은 경우 A-M
			memcpy(A, subtract(A, num2, BIT_SIZE), BIT_SIZE);
			
			print(A, BIT_SIZE); cout << "//"; print(Q, BIT_SIZE);
			cout << " || A <- A - M \n";

		}
		else if (A[BIT_SIZE-1] ^ num2[BIT_SIZE-1]) { //부호가 다른 경우 A+M
			memcpy(A, add(A, num2, BIT_SIZE), BIT_SIZE);

			print(A, BIT_SIZE); cout << "//"; print(Q, BIT_SIZE);
			cout << " || A <- A + M \n";
		}

		//연산 전후의 A부호 비교
		if ((ASign^A[BIT_SIZE-1])) { //부호가 다르면 A복원
			if (!(ASign^num2[BIT_SIZE - 1])) { //A+M
				memcpy(A, add(A, num2, BIT_SIZE), BIT_SIZE);
			}
			else { //A-M
				memcpy(A, subtract(A, num2, BIT_SIZE), BIT_SIZE);
			}

			print(A, BIT_SIZE); cout << "//"; print(Q, BIT_SIZE);
			cout << " || Q0 <- 0 , A 복원\n";			
		}

		else { //부호가 같으면
			Q[0] = 1;
			print(A, BIT_SIZE); cout << "//"; print(Q, BIT_SIZE);
			cout << " || Q0 <- 1\n";
		}
		cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
	}
		//Q와 M의 부호가 다르면 보수 취하기
		bool *share;
		if (num1[BIT_SIZE - 1] ^ num2[BIT_SIZE - 1]) 
			share = complement(Q, BIT_SIZE);
		else
			share = Q;

		cout << "나머지 : "; print(A, BIT_SIZE); cout << "십진수 : " << frombinary(A, BIT_SIZE) << endl;
		cout << setw(9) << " 몫 : "; print(share, BIT_SIZE); cout << "십진수 : " << frombinary(share, BIT_SIZE) << endl;

}
