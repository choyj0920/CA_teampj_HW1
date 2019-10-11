#pragma once
class ALU
{
	int n1, n2;
	bool *num1;
	bool *num2;
	bool *result;
	bool Carry[BIT_SIZE + 1] = { false, };
	bool flag_c;
	bool flag_s;
	bool flag_z;
	bool flag_v;

public:
	ALU();
	~ALU();

	//num의 주소값과 비트 개수를 입력받아 보수를 계산해 comple의 값을 변경
	static void BinaryComplement(const bool *num,bool *comple,int bitn);
	//어떠한 정수와 bit수를 입력하면 binary에 그 이진수를 저장
	static void DecimalToBinary(bool* binary,int n, int bitn);
	//num의 주소값과 비트 개수를 입력 받아 그 num을 십진수로 변환
	static __int64 BinaryToDecimal(bool *num, int bitn);
	//이진수 출력
	static void PrintBinary(const bool *num, int bitn);
	////입력값 범위가 32비트에 들어갈 수 있는지 판단
	//static bool check_range(int a, int b);

	//입력값을 통해 클래스 내부 변수 초기화
	void Init(int a, int b);
	//계산 시작
	void Start_Calculate();
	//병렬가산기 연산
	void Add(const bool *num1,const bool *num2,bool* add_result, int bitn);
	//플래그 연산
	void Flag(bool *add_result, int bitn);
	//병렬감산기 연산
	void Subtract(const bool *num1,const bool *num2,bool* sub_result, int bitn);
	//병렬감가산기 결과 출력
	void PrintAddSub();
	//Booth 알고리즘을 이용한 곱셈 연산
	void Multiply();
	//나누기 연산을 통한 나눗셈 연산
	void Divide();

};

