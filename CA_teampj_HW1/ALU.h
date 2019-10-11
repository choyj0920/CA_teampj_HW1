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

	//num�� �ּҰ��� ��Ʈ ������ �Է¹޾� ������ ����� comple�� ���� ����
	static void BinaryComplement(const bool *num,bool *comple,int bitn);
	//��� ������ bit���� �Է��ϸ� binary�� �� �������� ����
	static void DecimalToBinary(bool* binary,int n, int bitn);
	//num�� �ּҰ��� ��Ʈ ������ �Է� �޾� �� num�� �������� ��ȯ
	static __int64 BinaryToDecimal(bool *num, int bitn);
	//������ ���
	static void PrintBinary(const bool *num, int bitn);
	////�Է°� ������ 32��Ʈ�� �� �� �ִ��� �Ǵ�
	//static bool check_range(int a, int b);

	//�Է°��� ���� Ŭ���� ���� ���� �ʱ�ȭ
	void Init(int a, int b);
	//��� ����
	void Start_Calculate();
	//���İ���� ����
	void Add(const bool *num1,const bool *num2,bool* add_result, int bitn);
	//�÷��� ����
	void Flag(bool *add_result, int bitn);
	//���İ���� ����
	void Subtract(const bool *num1,const bool *num2,bool* sub_result, int bitn);
	//���İ������ ��� ���
	void PrintAddSub();
	//Booth �˰����� �̿��� ���� ����
	void Multiply();
	//������ ������ ���� ������ ����
	void Divide();

};

