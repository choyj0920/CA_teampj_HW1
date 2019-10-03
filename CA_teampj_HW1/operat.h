#pragma once

class operat
{
	//num[0]�� ���� ���ڸ� ...num[3]num[2]num[1]num[0]
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

	//num�� �ּҰ��� ��Ʈ ������ �Է¹޾� ������ ����� �����ͷ� ����
	static bool* complement(bool *num, int bitn);
	//��� ������ bit���� �Է��ϸ� bool�� 2���� �迭�� �ּҰ����� ����
	static bool* tobinary(int n, int bitn);
	//num�� �ּҰ��� ��Ʈ ������ �Է� �޾� �� num�� �������� ��ȯ
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
