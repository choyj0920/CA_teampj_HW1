#include "pch.h"

operat::operat(int a, int b) :n1(a), n2(b) //���� 2���� ��ȯ
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

bool * operat::complement(bool *num,int bitn) //���� ��ȯ
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

bool * operat::tobinary(int n ,int bitn) //2���� ��ȯ
{
	bool n_posit = n >= 0; //�������
	bool *binary = new bool[bitn] {false, };
	for (int i = 0; i < BIT_SIZE - 1; i++) {
		binary[i] = n % 2;
		n /= 2;
		if (n == 0) break;
	}
	if (!n_posit) //������
		return complement(binary,bitn); //���� ��ȯ
	else
		return binary;
}

int operat::frombinary(bool * num, int bitn) //10���� ��ȯ
{
   int n = 0; //��� ���
   int two_ = 1;
   
   if (num[bitn - 1]) { //��ȣ ��Ʈ�� 1�̸�
	  num = complement(num, bitn); //�������� ������ȯ
      two_ = -1; // - ��ȣ ó�� ����
   }
   
   for (int i = 0; i < BIT_SIZE; i++) {
	   if (num[i])
		   n += two_; 
	   two_ *= 2; //(2)n�� or (-2)n�� 
   }
   return n;
}

void operat::print(bool * num, int bitn) //2���� ����Ʈ
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
	system("pause");	cout << "\n\n" << "���� ����\n";
	multiple();
	system("pause");	cout << "\n\n" << "������ ����\n";
	division();
}

bool * operat::add(bool * num1, bool * num2, int bitn)
{
	//���� ����� bitn��
	bool *add_result = new bool[bitn];
	for (int i = 0; i < bitn; i++) {
		add_result[i] = num1[i] ^ num2[i] ^ Carry[i];
		Carry[i + 1] = ((num1[i] ^ num2[i]) &   Carry[i]) | (num1[i] & num2[i]);
	}
	//flag ����
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

	return add_result;
}

bool * operat::subtract(bool * num1, bool * num2, int bitn)
{
	bool *sub_num = complement(num2, bitn);
	return add(num1, sub_num, bitn);

}

void operat::addsub_printf()
{
	cout << "���� ���� \n";
	result = add(num1, num2, BIT_SIZE);
	cout << n1 << " + (" << n2 << ") : " << endl;
	cout << "   num1 : "; print(num1,BIT_SIZE); cout << endl;
	cout << "+  num2 : "; print(num2,BIT_SIZE); cout << endl;
	cout << "\n----------------------------------------------------------------------------------------\n";
	if(flag_v == 1)
		cout << "result : " << "�����÷ο�" << endl;
	else {
		cout << " result : "; print(result, BIT_SIZE);  cout << " = 10���� : " << frombinary(result, BIT_SIZE) << endl;
	}
	cout << "V : " << flag_v << ", Z : " << flag_z << ", S : " << flag_s << ", C : " << flag_c << endl<<endl;

	 
	cout << "���� ���� \n";
	delete result;
	result = subtract(num1, num2, BIT_SIZE);
	cout << n1 << " - (" << n2 << ") : " << endl;
	cout << "   num1 : "; print(num1, BIT_SIZE); cout << endl;
	cout << "-  num2 : "; print(num2, BIT_SIZE); cout << endl;
	cout << "\n----------------------------------------------------------------------------------------\n";
	if (flag_v == 1)
		cout << "result : " << "�����÷ο�" << endl;
	else {
		cout << " result : "; print(result, BIT_SIZE);  cout << " = 10���� : " << frombinary(result, BIT_SIZE) << endl;
	}

	cout << "V : " << flag_v << ", Z : " << flag_z << ", S : " << flag_s << ", C : " << flag_c << endl;

}

void operat::multiple()
{
	//num1 �ǽ¼�M num2 �¼�Q , X_result A+Q;
	cout << n1 << " X " << n2 << endl;
	bool *X_result = new bool[(BIT_SIZE * 3)]{ false, };
	bool Q0 = false;
	bool *Q = X_result;
	bool *A = X_result + BIT_SIZE; //A�� ��ġ

	for (int i = 0; i < BIT_SIZE; i++) {
		Q[i] = num2[i];
	}

	//���̵�������
	cout << setw(2 * BIT_SIZE) << "A   " << "//" << setw(2 * BIT_SIZE) << "Q" << "//" << "Q0/" << setw(2 * BIT_SIZE) << "M   " << "���� ���� " << endl;
	print(A, BIT_SIZE); cout << "//"; print(Q, BIT_SIZE); cout << "//"; cout << int(Q0); cout << "//"; print(num1,BIT_SIZE);
	cout << "\n------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
	
	for (int i = 0; i < BIT_SIZE; i++) {
		if (Q[0] & (!Q0)) { //10�� ���
			memcpy(A, subtract(A, num1, BIT_SIZE), BIT_SIZE);
			print(A, BIT_SIZE); cout << "//"; print(Q, BIT_SIZE); cout << "//"; cout << int(Q0); //cout << "//"; print(num1, BIT_SIZE); 
			cout << " || A <- A - M \n";
		}
		else if ((!Q[0])&Q0) { //01�� ���
			memcpy(A, add(A, num1, BIT_SIZE), BIT_SIZE);
			print(A, BIT_SIZE); cout << "//"; print(Q, BIT_SIZE); cout << "//"; cout << int(Q0); //cout << "//"; print(num1, BIT_SIZE);
			cout << " || A <- A + M \n";
		}

		Q0 = Q[0];

		//����� ���� shift
		X_result[BIT_SIZE * 2] = X_result[BIT_SIZE * 2 - 1]; //�ֻ�����Ʈ����(�����)
		X_result++; A++; Q++;

		/*for (int j = 0; j < (BIT_SIZE * 2) - 1; j++)
			X_result[i] = X_result[i + 1];*/
		
		print(A, BIT_SIZE); cout << "//"; print(Q, BIT_SIZE); cout << "//"; cout << int(Q0); //cout << "//"; print(num1, BIT_SIZE);

		if (i < BIT_SIZE - 1)
			cout << " >> right shift \n";
		else if (i == BIT_SIZE - 1)
			cout << " || ���� �Ϸ�" << endl;

		cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";

	}
	cout << "��� : "; print(X_result, BIT_SIZE * 2); cout << endl; cout << "������ : " << frombinary(X_result, BIT_SIZE * 2) << endl;
}

void operat::division()
{
	//num1 ������Q, num2 ����M , X_result A+Q;
	cout << n1 << " / " << n2 << endl;
	bool *X_result = new bool[(BIT_SIZE * 3)]{ false, };
	bool Q0 = 0; //���� ������ A��ȣ�� ������ 1�� / �ٸ��� 0���� + ���� ��� ���� 
	bool *Q = X_result ;
	bool *A = X_result + BIT_SIZE; //A�� ��ġ
	bool ASign = false; 
	
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
	cout << setw(2 * BIT_SIZE) << "A   " << "//" << setw(2 * BIT_SIZE) << "Q //" <<  setw(2 * BIT_SIZE) << "M   " << "���� ���� " << endl; 
	print(A, BIT_SIZE); cout << "//"; print(Q, BIT_SIZE); cout << "//";  print(num2, BIT_SIZE);
	cout << "\n------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
	
	for (int i = 0; i < BIT_SIZE; i++) {

		// ����shift
		X_result--; A--; Q--;
			Q[0] = 0; //�ӽ÷� Q[0]�� 0����
		print(A, BIT_SIZE); cout << "//"; print(Q, BIT_SIZE); cout << " << left shift \n";

		//cout << "A��ȣ : " << A[BIT_SIZE] << "/ M��ȣ : " << num1[BIT_SIZE-1] << endl << endl;

		ASign = A[BIT_SIZE-1]; //A ���� ��ȣ�� ����

		//A,M�� ��ȣ ��
		if (!(A[BIT_SIZE-1] ^ num2[BIT_SIZE-1])) { //��ȣ�� ���� ��� A-M
			memcpy(A, subtract(A, num2, BIT_SIZE), BIT_SIZE);
			
			print(A, BIT_SIZE); cout << "//"; print(Q, BIT_SIZE);
			cout << " || A <- A - M \n";

		}
		else if (A[BIT_SIZE-1] ^ num2[BIT_SIZE-1]) { //��ȣ�� �ٸ� ��� A+M
			memcpy(A, add(A, num2, BIT_SIZE), BIT_SIZE);

			print(A, BIT_SIZE); cout << "//"; print(Q, BIT_SIZE);
			cout << " || A <- A + M \n";
		}

		//���� ������ A��ȣ ��
		if ((ASign^A[BIT_SIZE-1])) { //��ȣ�� �ٸ��� A����
			if (!(ASign^num2[BIT_SIZE - 1])) { //A+M
				memcpy(A, add(A, num2, BIT_SIZE), BIT_SIZE);
			}
			else { //A-M
				memcpy(A, subtract(A, num2, BIT_SIZE), BIT_SIZE);
			}

			print(A, BIT_SIZE); cout << "//"; print(Q, BIT_SIZE);
			cout << " || Q0 <- 0 , A ����\n";			
		}

		else { //��ȣ�� ������
			Q[0] = 1;
			print(A, BIT_SIZE); cout << "//"; print(Q, BIT_SIZE);
			cout << " || Q0 <- 1\n";
		}
		cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
	}
		//Q�� M�� ��ȣ�� �ٸ��� ���� ���ϱ�
		bool *share;
		if (num1[BIT_SIZE - 1] ^ num2[BIT_SIZE - 1]) 
			share = complement(Q, BIT_SIZE);
		else
			share = Q;

		cout << "������ : "; print(A, BIT_SIZE); cout << "������ : " << frombinary(A, BIT_SIZE) << endl;
		cout << setw(9) << " �� : "; print(share, BIT_SIZE); cout << "������ : " << frombinary(share, BIT_SIZE) << endl;

}
