#pragma once

class operat
{
	//num[0]은 가장 끝자리 ...num[3]num[2]num[1]num[0]
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
	operat(int a, int b);
	~operat();

	//num의 주소값과 비트 개수를 입력받아 보수를 계산해 포인터로 리턴
	static bool* complement(bool *num, int bitn);
	//어떠한 정수와 bit수를 입력하면 bool형 2진수 배열의 주소값으로 리턴
	static bool* tobinary(int n, int bitn);
	//num의 주소값과 비트 개수를 입력 받아 그 num을 십진수로 변환
	static int frombinary(bool *num, int bitn);
	static void print(bool *num,int);

	void initial(int a, int b);
	void start_calculate();
	bool* add(bool *num, bool *num2, int bitn);
	bool* subtract(bool *num1, bool *num2, int bitn);
	void addsub_printf();
	void multiple();
	void division();
};
