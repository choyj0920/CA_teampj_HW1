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
//	//�Է°��� - (2��n-1) ~ (2��n-1 -1) ���� ���� Ȯ��(�������� n -> 32)
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
	int number = n; // ���� n��
	bool n_posit = n >= 0; //�������
	
	for (int i = 0; i < bitn - 1; i++) {
		if (i == bitn - 2 && number == -(pow(2, bitn - 1))) { //�ּ��� ��� 1000...
			binary[bitn - 1] = 1;
			break;
		}
		binary[i] = n % 2;
		n /= 2;
		if (n == 0)
			break;
	}

	if (n_posit || number == -(pow(2, BIT_SIZE - 1))) { //����ų�,  �ּҸ�(10000)
		
	}
	else {//������

		BinaryComplement(binary,binary ,bitn);
	}
}

__int64 ALU::BinaryToDecimal(bool * num, int bitn)
{
	__int64 n = 0; //��� ���
	__int64 two_ = 1;

	if (num[bitn - 1]) { //��ȣ ��Ʈ�� 1�̸�
		BinaryComplement(num,num, bitn); //�������� ������ȯ
		two_ = -1; // - ��ȣ ó�� ����
	}

	for (int i = 0; i < bitn; i++) {
		if (num[i])
			n += two_;
		two_ *= 2; //(2)n�� or (-2)n�� 
	   //cout << i<< " ���� ��� : " << n <<" �������� �� : " <<two_ << endl;
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
	system("pause");	cout << "\n\n" << "���� ����\n";
	Multiply();
	system("pause");	cout << "\n\n" << "������ ����\n";
	Divide();
}

void  ALU::Add(const bool * num1,const  bool * num2, bool * add_result, int bitn)
{
	Carry[0] = false;
	for (int i = 0; i < bitn; i++) {
		Carry[i + 1] = ((num1[i] ^ num2[i]) &   Carry[i]) | (num1[i] & num2[i]);
		add_result[i] = num1[i] ^ num2[i] ^ Carry[i];
	}

	//flag ����
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
		if (add_result[i] == true) { //�ϳ��� 1�̸� z�� 0���� ����
			flag_z = false;
			break;
		}
	}//z�� ��� ������� 0�̾�� 1�� ����

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
	cout << "���� ���� \n";
	Add(num1, num2,result, BIT_SIZE);
	cout << n1 << " + (" << n2 << ") : " << endl;
	cout << "   num1 : "; PrintBinary(num1, BIT_SIZE); cout << endl;
	cout << "+  num2 : "; PrintBinary(num2, BIT_SIZE); cout << endl;
	cout << "\n----------------------------------------------------------------------------------------\n";
	if (flag_v == 1)
		cout << "result : " << "�����÷ο�/����÷ο�" << endl;
	else {
		cout << " result : "; PrintBinary(result, BIT_SIZE);  cout << " = 10���� : " << BinaryToDecimal(result, BIT_SIZE) << endl;
	}
	cout << "V : " << flag_v << ", Z : " << flag_z << ", S : " << flag_s << ", C : " << flag_c << endl << endl;
	flag_v = flag_z = flag_s = flag_c = 0; //flag �ʱ�ȭ


	cout << "���� ���� \n";
	Subtract(num1, num2, result, BIT_SIZE);
	cout << n1 << " - (" << n2 << ") : " << endl;
	cout << "   num1 : "; PrintBinary(num1, BIT_SIZE); cout << endl;
	cout << "-  num2 : "; PrintBinary(num2, BIT_SIZE); cout << endl;
	cout << "\n----------------------------------------------------------------------------------------\n";
	if (flag_v == 1)
		cout << "result : " << "�����÷ο�/����÷ο�" << endl;
	else {
		cout << " result : "; PrintBinary(result, BIT_SIZE);  cout << " = 10���� : " << BinaryToDecimal(result, BIT_SIZE) << endl;
	}

	cout << "V : " << flag_v << ", Z : " << flag_z << ", S : " << flag_s << ", C : " << flag_c << endl;
	flag_v = flag_z = flag_s = flag_c = 0; //flag �ʱ�ȭ
}

void ALU::Multiply()
{
	//num1 �ǽ¼�M num2 �¼�Q , X_result A+Q;
	cout << n1 << " X " << n2 << endl;
	bool regist[(BIT_SIZE * 2)]={ false, };
	bool *X_result = regist;
	bool Q0 = false;
	bool *Q = X_result;
	bool *A = X_result + BIT_SIZE; //A�� ��ġ
	bool cu_flag_v = false; //�����÷ο� ���� üũ ���� ����

	for (int i = 0; i < BIT_SIZE; i++) {
		Q[i] = num2[i];
	}

	//���̵�������
	cout << setw(2 * BIT_SIZE) << "A   " << "//" << setw(2 * BIT_SIZE) << "Q" << "//" << "Q0/" << setw(2 * BIT_SIZE) << "M   " << "���� ���� " << endl;
	PrintBinary(A, BIT_SIZE); cout << "//"; PrintBinary(Q, BIT_SIZE); cout << "//"; cout << int(Q0); cout << "//"; PrintBinary(num1, BIT_SIZE);
	cout << "\n------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";

	for (int i = 0; i < BIT_SIZE; i++) {
		if (Q[0] & (!Q0)) { //10�� ���
			Subtract(A, num1, A, BIT_SIZE);
			PrintBinary(A, BIT_SIZE); cout << "//"; PrintBinary(Q, BIT_SIZE); cout << "//"; cout << int(Q0); //cout << "//"; print(num1, BIT_SIZE); 
			cout << " || A <- A - M	";
			cout << setw(8) << i + 1 << ") V : " << flag_v << ", Z : " << flag_z << ", S : " << flag_s << ", C : " << flag_c << endl;

		}
		else if ((!Q[0])&Q0) { //01�� ���
			Add(A, num1, A, BIT_SIZE);
			PrintBinary(A, BIT_SIZE); cout << "//"; PrintBinary(Q, BIT_SIZE); cout << "//"; cout << int(Q0); //cout << "//"; print(num1, BIT_SIZE);
			cout << " || A <- A + M	";
			cout << setw(8) << i + 1 << ") V : " << flag_v << ", Z : " << flag_z << ", S : " << flag_s << ", C : " << flag_c << endl;

		}

		Q0 = Q[0];

		//����� ���� shift
		Shift_right(X_result, BIT_SIZE * 2);

		PrintBinary(A, BIT_SIZE); cout << "//"; PrintBinary(Q, BIT_SIZE); cout << "//"; cout << int(Q0); //cout << "//"; print(num1, BIT_SIZE);

		if (i < BIT_SIZE - 1)
			cout << " >>  right shift ";
		else if (i == BIT_SIZE - 1)
			cout << " || ���� �Ϸ�";

		cout << setw(10) << i + 1 << ") V : " << flag_v << ", Z : " << flag_z << ", S : " << flag_s << ", C : " << flag_c << endl;
		cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";


		cu_flag_v |= flag_v; //�����÷ο� üũ

		if (i == BIT_SIZE - 1)
			break;
		else
			flag_v = flag_z = flag_s = flag_c = 0; //flag �ʱ�ȭ

	}

	if (cu_flag_v == 1) {
		cout << "��� : �����÷ο�/����÷ο�� ���� �������" << endl;
	}
	else {
		cout << "��� : "; PrintBinary(X_result, BIT_SIZE * 2); cout << endl; cout << "������ : " << BinaryToDecimal(X_result, BIT_SIZE * 2) << endl;

	}
	
	
}

void ALU::Divide()
{
	//num1 ������Q, num2 ����M , X_result A+Q;
	cout << n1 << " / " << n2 << endl;

	if (n2 == 0) { //ex) 5/0 = �Ҵ�
		cout << "������ 0�� ��� ���� �� �����ϴ�." << endl;
		return;
	}
	
	bool X_result[(BIT_SIZE * 2)] = { false, };
	bool Q0 = 0; //���� ������ A��ȣ�� ������ 1�� / �ٸ��� 0���� + ���� ��� ���� 
	bool* Q = X_result;
	bool *A = X_result + BIT_SIZE; //A�� ��ġ
	bool ASign = false;
	bool cu_flag_v = false;

	//������Q ����
	for (int i = 0; i < BIT_SIZE; i++) {
		Q[i] = num1[i];
	}

	//Q�� ������ A�� 1111...
	if (Q[BIT_SIZE - 1]) {
		for (int i = 0; i < BIT_SIZE; i++)
			A[i] = true;
	}

	//���̵�������
	cout << setw(2 * BIT_SIZE) << "A   " << "//" << setw(2 * BIT_SIZE) << "Q //" << setw(2 * BIT_SIZE) << "M   " << "���� ���� " << endl;
	PrintBinary(A, BIT_SIZE); cout << "//"; PrintBinary(Q, BIT_SIZE); cout << "//";  PrintBinary(num2, BIT_SIZE);
	cout << "\n------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";

	for (int i = 0; i < BIT_SIZE; i++) {

		// ����shift
		Shift_left(X_result, BIT_SIZE * 2,false);

		Q[0] = 0; //�ӽ÷� Q[0]�� 0����
		PrintBinary(A, BIT_SIZE); cout << "//"; PrintBinary(Q, BIT_SIZE); cout << " << left shift "; 
		cout << setw(9) << i + 1 << ") V : " << flag_v << ", Z : " << flag_z << ", S : " << flag_s << ", C : " << flag_c << endl;

		ASign = A[BIT_SIZE - 1]; //A ���� ��ȣ�� ����

		//A,M�� ��ȣ ��
		if (!(A[BIT_SIZE - 1] ^ num2[BIT_SIZE - 1])) { //��ȣ�� ���� ��� A-M
			Subtract(A, num2, result,BIT_SIZE);
			PrintBinary(result, BIT_SIZE); cout << "//"; PrintBinary(Q, BIT_SIZE);
			cout << " || A <- A - M";
		}
		else if (A[BIT_SIZE - 1] ^ num2[BIT_SIZE - 1]) { //��ȣ�� �ٸ� ��� A+M
			Add(A, num2, result,BIT_SIZE);
			PrintBinary(result, BIT_SIZE); cout << "//"; PrintBinary(Q, BIT_SIZE);
			cout << " || A <- A + M";
		}
		memcpy(A, result, BIT_SIZE);

		cout << setw(10) << i + 1 << ") V : " << flag_v << ", Z : " << flag_z << ", S : " << flag_s << ", C : " << flag_c << endl;
		flag_v = flag_z = flag_s = flag_c = 0; //flag �ʱ�ȭ


		//���� ������ A��ȣ ��

		//A�� 000..0 �϶� 'Q0=0,A�� ����'���� �Ұ��� 'Q0=1'�� �Ұ���
		//Q�� �� BIT_N/2 �ڸ����� ��� 0�϶� - Q0=1
		//						  ��� 0�� �ƴҶ� - Q0=0, A�� ����
		bool QZero = false;
		for (int i = BIT_SIZE - 1; i >= BIT_SIZE / 2; i--) {
			QZero |= Q[i];
		}

		//A�� ���� ���� ��ȣ�� ������
		if (!(ASign^result[BIT_SIZE - 1]) || (BinaryToDecimal(result, BIT_SIZE) == 0 && QZero == 0)) { 

			//flag���� ���� A + 0 ����
			bool zero[BIT_SIZE] = { false, };
			Add(A, zero, result, BIT_SIZE);

			Q[0] = 1;
			PrintBinary(A, BIT_SIZE); cout << "//"; PrintBinary(Q, BIT_SIZE);
			if (i < BIT_SIZE - 1)
				cout << " || Q0 <- 1"<< setw(4) << "";
			else if (i == BIT_SIZE - 1)
				cout << " || ���� �Ϸ�"<<setw(4) << "";
		} 
		//A�� ���� ���� ��ȣ�� �ٸ��� A����
		else {
			
			if (!(ASign^num2[BIT_SIZE - 1])) { //A�� M�� ��ȣ�� �޶� A-M�� �߾����� A+M���� ����
				Add(A, num2, result, BIT_SIZE);
				memcpy(A, result, BIT_SIZE);
			}
			else { //��ȣ�� ���Ƽ� A+M�� �߾����� A-M���� ����
				Subtract(A, num2, result, BIT_SIZE);
				memcpy(A, result, BIT_SIZE);
			}

			PrintBinary(A, BIT_SIZE); cout << "//"; PrintBinary(Q, BIT_SIZE);
			
			
			if (i < BIT_SIZE - 1)
				cout << " || Q0 <- 0 , A ����";
			else if (i == BIT_SIZE - 1)
				cout << " || ���� �Ϸ�" << setw(4) << "";
		}
		cout << setw(4) << i + 1 << ") V : " << flag_v << ", Z : " << flag_z << ", S : " << flag_s << ", C : " << flag_c << endl;

		cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";

		cu_flag_v |= flag_v; //�����÷ο� üũ

		if (i == BIT_SIZE - 1)
			break;
		else
			flag_v = flag_z = flag_s = flag_c = 0; //flag �ʱ�ȭ
	}
	//Q�� M�� ��ȣ�� �ٸ��� ���� ���ϱ�
	bool share[BIT_SIZE] = { false, };
	for (int i = 0; i < BIT_SIZE; i++) {
		share[i] = Q[i];
	}
	if (num1[BIT_SIZE - 1] ^ num2[BIT_SIZE - 1])
		BinaryComplement(Q, share, BIT_SIZE);

	if (cu_flag_v == 1)
		cout << "�����÷ο�/����÷ο�� ���� �������" << endl;
	else {
		cout << "������ : "; PrintBinary(A, BIT_SIZE); cout << "������ : " << BinaryToDecimal(A, BIT_SIZE) << endl;
		cout << setw(9) << " �� : "; PrintBinary(share, BIT_SIZE); cout << "������ : " << BinaryToDecimal(share, BIT_SIZE) << endl;
	}

	
}
