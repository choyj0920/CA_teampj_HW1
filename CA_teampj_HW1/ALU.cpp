#include "pch.h"
#include "ALU.h"

ALU::ALU()
{
}


ALU::~ALU()
{
	if (num1 !=NULL)	delete[] num1;
	if (num2 != NULL)	delete[] num2;
	if (result != NULL) delete[] result;
}

//bool ALU::check_range(int a, int b)
//{
//	//입력값이 - (2의n-1) ~ (2의n-1 -1) 인지 범위 확인(예제기준 n -> 32)
//	if (((a >= -(pow(2, BIT_SIZE - 1))) && (a <= (pow(2, BIT_SIZE - 1) - 1))) && (b >= -(pow(2, BIT_SIZE - 1))) && (b <= (pow(2, BIT_SIZE - 1) - 1)))
//		return true;
//	else
//		return false;
//}

void ALU::BinaryComplement(const bool * num, bool * comple, int bitn)
{
	for (int i = 0; i < bitn; i++) {
		comple[i] = !num[i];
	}
	for (int i = 0; i < bitn; i++) {
		comple[i] = !comple[i];
		if (comple[i] == 1)
			break;
	}
}

void ALU::DecimalToBinary(bool * binary, int n, int bitn)
{
	int number = n; // 원래 n값
	bool n_posit = n >= 0; //양수인지
	
	for (int i = 0; i < bitn - 1; i++) {
		if (i == bitn - 2 && number == -(pow(2, bitn - 1))) { //최소인 경우 1000...
			binary[bitn - 1] = 1;
			break;
		}
		binary[i] = n % 2;
		n /= 2;
		if (n == 0)
			break;
	}

	if (n_posit || number == -(pow(2, BIT_SIZE - 1))) { //양수거나,  최소면(10000)
		
	}
	else {//음수면

		BinaryComplement(binary,binary ,bitn);
	}
}

__int64 ALU::BinaryToDecimal(bool * num, int bitn)
{
	__int64 n = 0; //계산 결과
	__int64 two_ = 1;

	if (num[bitn - 1]) { //부호 비트가 1이면
		BinaryComplement(num,num, bitn); //음수여서 보수변환
		two_ = -1; // - 부호 처리 위해
	}

	for (int i = 0; i < bitn; i++) {
		if (num[i])
			n += two_;
		two_ *= 2; //(2)n승 or (-2)n승 
	   //cout << i<< " 누적 결과 : " << n <<" 더해지는 값 : " <<two_ << endl;
	}
	return n;
}

void ALU::PrintBinary(const bool * num, int bitn)
{
	for (int i = bitn - 1; i >= 0; i--) {
		if (num[i]) {
			cout << "1 ";
		}
		else
			cout << "0 ";
	}
}

void ALU::Init(int a, int b)
{
	n1 = a, n2 = b;
	if (num1)	delete[] num1;
	if (num2)	delete[] num2;
	if (result) delete[] result;
	num1 = new bool[BIT_SIZE] {false, };
	num2 = new bool[BIT_SIZE] {false, };
	result = new bool[BIT_SIZE] {false, };
	DecimalToBinary(num1, n1, BIT_SIZE);
	DecimalToBinary(num2, n2, BIT_SIZE);

}

void ALU::Start_Calculate()
{
	PrintAddSub();
	system("pause");	cout << "\n\n" << "곱셈 실행\n";
	Multiply();
	system("pause");	cout << "\n\n" << "나눗셈 실행\n";
	Divide();
}

void  ALU::Add(const bool * num1,const  bool * num2, bool * add_result, int bitn)
{
	Carry[0] = false;
	for (int i = 0; i < bitn; i++) {
		Carry[i + 1] = ((num1[i] ^ num2[i]) &   Carry[i]) | (num1[i] & num2[i]);
		add_result[i] = num1[i] ^ num2[i] ^ Carry[i];
	}

	//flag 연산
	Flag(add_result, bitn);

}

void ALU::Shift_left(bool* num, int bitn, bool Q0)
{
	Carry[0] = Q0;
	for (int i = 0; i < bitn; i++) 
		Carry[i + 1] = num[i];
	
	for (int i = 0; i < bitn; i++) 
		num[i] = Carry[i];
	
	
	Flag(num, bitn);
	
}

void ALU::Shift_right(bool* num, int bitn)
{	
	Carry[bitn - 1] = 0;
	for (int i = 0; i < bitn-1; i++)
		Carry[i] = num[i + 1];

	for (int i = 0; i < bitn -1; i++) 
		num[i] = Carry[i];
	
	Flag(num, bitn);
}

void ALU::Flag(bool *add_result ,int bitn) {
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
}

void ALU::Subtract(const bool * num1,const bool * num2, bool * sub_result, int bitn)
{
	bool* num2_comple = new bool[bitn];
	BinaryComplement(num2, num2_comple, bitn);
	Add(num1, num2_comple,sub_result ,bitn);
	delete[]  num2_comple;
}

void ALU::PrintAddSub()
{
	cout << "덧셈 실행 \n";
	Add(num1, num2,result, BIT_SIZE);
	cout << n1 << " + (" << n2 << ") : " << endl;
	cout << "   num1 : "; PrintBinary(num1, BIT_SIZE); cout << endl;
	cout << "+  num2 : "; PrintBinary(num2, BIT_SIZE); cout << endl;
	cout << "\n----------------------------------------------------------------------------------------\n";
	if (flag_v == 1)
		cout << "result : " << "오버플로우/언더플로우" << endl;
	else {
		cout << " result : "; PrintBinary(result, BIT_SIZE);  cout << " = 10진수 : " << BinaryToDecimal(result, BIT_SIZE) << endl;
	}
	cout << "V : " << flag_v << ", Z : " << flag_z << ", S : " << flag_s << ", C : " << flag_c << endl << endl;
	flag_v = flag_z = flag_s = flag_c = 0; //flag 초기화


	cout << "뺄셈 실행 \n";
	Subtract(num1, num2, result, BIT_SIZE);
	cout << n1 << " - (" << n2 << ") : " << endl;
	cout << "   num1 : "; PrintBinary(num1, BIT_SIZE); cout << endl;
	cout << "-  num2 : "; PrintBinary(num2, BIT_SIZE); cout << endl;
	cout << "\n----------------------------------------------------------------------------------------\n";
	if (flag_v == 1)
		cout << "result : " << "오버플로우/언더플로우" << endl;
	else {
		cout << " result : "; PrintBinary(result, BIT_SIZE);  cout << " = 10진수 : " << BinaryToDecimal(result, BIT_SIZE) << endl;
	}

	cout << "V : " << flag_v << ", Z : " << flag_z << ", S : " << flag_s << ", C : " << flag_c << endl;
	flag_v = flag_z = flag_s = flag_c = 0; //flag 초기화
}

void ALU::Multiply()
{
	//num1 피승수M num2 승수Q , X_result A+Q;
	cout << n1 << " X " << n2 << endl;
	bool regist[(BIT_SIZE * 2)]={ false, };
	bool *X_result = regist;
	bool Q0 = false;
	bool *Q = X_result;
	bool *A = X_result + BIT_SIZE; //A의 위치
	bool cu_flag_v = false; //오버플로우 여부 체크 위한 변수

	for (int i = 0; i < BIT_SIZE; i++) {
		Q[i] = num2[i];
	}

	//가이드라인출력
	cout << setw(2 * BIT_SIZE) << "A   " << "//" << setw(2 * BIT_SIZE) << "Q" << "//" << "Q0/" << setw(2 * BIT_SIZE) << "M   " << "연산 상태 " << endl;
	PrintBinary(A, BIT_SIZE); cout << "//"; PrintBinary(Q, BIT_SIZE); cout << "//"; cout << int(Q0); cout << "//"; PrintBinary(num1, BIT_SIZE);
	cout << "\n------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";

	for (int i = 0; i < BIT_SIZE; i++) {
		if (Q[0] & (!Q0)) { //10인 경우
			Subtract(A, num1, A, BIT_SIZE);
			PrintBinary(A, BIT_SIZE); cout << "//"; PrintBinary(Q, BIT_SIZE); cout << "//"; cout << int(Q0); //cout << "//"; print(num1, BIT_SIZE); 
			cout << " || A <- A - M	";
			cout << setw(8) << i + 1 << ") V : " << flag_v << ", Z : " << flag_z << ", S : " << flag_s << ", C : " << flag_c << endl;

		}
		else if ((!Q[0])&Q0) { //01인 경우
			Add(A, num1, A, BIT_SIZE);
			PrintBinary(A, BIT_SIZE); cout << "//"; PrintBinary(Q, BIT_SIZE); cout << "//"; cout << int(Q0); //cout << "//"; print(num1, BIT_SIZE);
			cout << " || A <- A + M	";
			cout << setw(8) << i + 1 << ") V : " << flag_v << ", Z : " << flag_z << ", S : " << flag_s << ", C : " << flag_c << endl;

		}

		Q0 = Q[0];

		//산술적 우측 shift
		Shift_right(X_result, BIT_SIZE * 2);

		PrintBinary(A, BIT_SIZE); cout << "//"; PrintBinary(Q, BIT_SIZE); cout << "//"; cout << int(Q0); //cout << "//"; print(num1, BIT_SIZE);

		if (i < BIT_SIZE - 1)
			cout << " >>  right shift ";
		else if (i == BIT_SIZE - 1)
			cout << " || 연산 완료";

		cout << setw(10) << i + 1 << ") V : " << flag_v << ", Z : " << flag_z << ", S : " << flag_s << ", C : " << flag_c << endl;
		cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";


		cu_flag_v |= flag_v; //오버플로우 체크

		if (i == BIT_SIZE - 1)
			break;
		else
			flag_v = flag_z = flag_s = flag_c = 0; //flag 초기화

	}

	if (cu_flag_v == 1) {
		cout << "결과 : 오버플로우/언더플로우로 인한 연산오류" << endl;
	}
	else {
		cout << "결과 : "; PrintBinary(X_result, BIT_SIZE * 2); cout << endl; cout << "십진수 : " << BinaryToDecimal(X_result, BIT_SIZE * 2) << endl;

	}
	
	
}

void ALU::Divide()
{
	//num1 피젯수Q, num2 젯수M , X_result A+Q;
	cout << n1 << " / " << n2 << endl;

	if (n2 == 0) { //ex) 5/0 = 불능
		cout << "젯수가 0인 경우 나눌 수 없습니다." << endl;
		return;
	}
	
	bool X_result[(BIT_SIZE * 2)] = { false, };
	bool Q0 = 0; //연산 전후의 A부호가 같으면 1로 / 다르면 0으로 + 연산 결과 복귀 
	bool* Q = X_result;
	bool *A = X_result + BIT_SIZE; //A의 위치
	bool ASign = false;
	bool cu_flag_v = false;

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
	cout << setw(2 * BIT_SIZE) << "A   " << "//" << setw(2 * BIT_SIZE) << "Q //" << setw(2 * BIT_SIZE) << "M   " << "연산 상태 " << endl;
	PrintBinary(A, BIT_SIZE); cout << "//"; PrintBinary(Q, BIT_SIZE); cout << "//";  PrintBinary(num2, BIT_SIZE);
	cout << "\n------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";

	for (int i = 0; i < BIT_SIZE; i++) {

		// 좌측shift
		Shift_left(X_result, BIT_SIZE * 2,false);

		Q[0] = 0; //임시로 Q[0]에 0대입
		PrintBinary(A, BIT_SIZE); cout << "//"; PrintBinary(Q, BIT_SIZE); cout << " << left shift "; 
		cout << setw(9) << i + 1 << ") V : " << flag_v << ", Z : " << flag_z << ", S : " << flag_s << ", C : " << flag_c << endl;

		ASign = A[BIT_SIZE - 1]; //A 이전 부호값 저장

		//A,M의 부호 비교
		if (!(A[BIT_SIZE - 1] ^ num2[BIT_SIZE - 1])) { //부호가 같은 경우 A-M
			Subtract(A, num2, result,BIT_SIZE);
			PrintBinary(result, BIT_SIZE); cout << "//"; PrintBinary(Q, BIT_SIZE);
			cout << " || A <- A - M";
		}
		else if (A[BIT_SIZE - 1] ^ num2[BIT_SIZE - 1]) { //부호가 다른 경우 A+M
			Add(A, num2, result,BIT_SIZE);
			PrintBinary(result, BIT_SIZE); cout << "//"; PrintBinary(Q, BIT_SIZE);
			cout << " || A <- A + M";
		}
		memcpy(A, result, BIT_SIZE);

		cout << setw(10) << i + 1 << ") V : " << flag_v << ", Z : " << flag_z << ", S : " << flag_s << ", C : " << flag_c << endl;
		flag_v = flag_z = flag_s = flag_c = 0; //flag 초기화


		//연산 전후의 A부호 비교

		//A가 000..0 일때 'Q0=0,A값 복원'으로 할건지 'Q0=1'로 할건지
		//Q의 앞 BIT_N/2 자리수가 모두 0일때 - Q0=1
		//						  모두 0은 아닐때 - Q0=0, A값 복원
		bool QZero = false;
		for (int i = BIT_SIZE - 1; i >= BIT_SIZE / 2; i--) {
			QZero |= Q[i];
		}

		//A의 연산 전후 부호가 같으면
		if (!(ASign^result[BIT_SIZE - 1]) || (BinaryToDecimal(result, BIT_SIZE) == 0 && QZero == 0)) { 

			//flag연산 위해 A + 0 연산
			bool zero[BIT_SIZE] = { false, };
			Add(A, zero, result, BIT_SIZE);

			Q[0] = 1;
			PrintBinary(A, BIT_SIZE); cout << "//"; PrintBinary(Q, BIT_SIZE);
			if (i < BIT_SIZE - 1)
				cout << " || Q0 <- 1"<< setw(4) << "";
			else if (i == BIT_SIZE - 1)
				cout << " || 연산 완료"<<setw(4) << "";
		} 
		//A의 연산 전후 부호가 다르면 A복원
		else {
			
			if (!(ASign^num2[BIT_SIZE - 1])) { //A와 M의 부호가 달라서 A-M을 했었으면 A+M으로 복원
				Add(A, num2, result, BIT_SIZE);
				memcpy(A, result, BIT_SIZE);
			}
			else { //부호가 같아서 A+M을 했었으면 A-M으로 복원
				Subtract(A, num2, result, BIT_SIZE);
				memcpy(A, result, BIT_SIZE);
			}

			PrintBinary(A, BIT_SIZE); cout << "//"; PrintBinary(Q, BIT_SIZE);
			
			
			if (i < BIT_SIZE - 1)
				cout << " || Q0 <- 0 , A 복원";
			else if (i == BIT_SIZE - 1)
				cout << " || 연산 완료" << setw(4) << "";
		}
		cout << setw(4) << i + 1 << ") V : " << flag_v << ", Z : " << flag_z << ", S : " << flag_s << ", C : " << flag_c << endl;

		cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";

		cu_flag_v |= flag_v; //오버플로우 체크

		if (i == BIT_SIZE - 1)
			break;
		else
			flag_v = flag_z = flag_s = flag_c = 0; //flag 초기화
	}
	//Q와 M의 부호가 다르면 보수 취하기
	bool share[BIT_SIZE] = { false, };
	for (int i = 0; i < BIT_SIZE; i++) {
		share[i] = Q[i];
	}
	if (num1[BIT_SIZE - 1] ^ num2[BIT_SIZE - 1])
		BinaryComplement(Q, share, BIT_SIZE);

	if (cu_flag_v == 1)
		cout << "오버플로우/언더플로우로 인한 연산오류" << endl;
	else {
		cout << "나머지 : "; PrintBinary(A, BIT_SIZE); cout << "십진수 : " << BinaryToDecimal(A, BIT_SIZE) << endl;
		cout << setw(9) << " 몫 : "; PrintBinary(share, BIT_SIZE); cout << "십진수 : " << BinaryToDecimal(share, BIT_SIZE) << endl;
	}

	
}
