
#include <stack> //���ͽ�Ʈ��� ã�� �׷��� ��� Ž��
#include <string> 
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

//Ŭ����

class edge
{
public:
	int id; //�� ������ id
	double dist; //���ÿ� ���� ������ �Ÿ�
	int t_num; //��ӵ��� ȯ�� Ƚ��
	edge* next;//������
	edge() : next(NULL) {}//null�� �ʱ�ȭ
};

class nodeInfo
{
	string node_name;//���� �̸�
	string highway_name;//���ð� ���� ��ӵ��� �̸�

public:
	void setString(string s1, string s2) {
		this->node_name = s1;
		this->highway_name = s2;
	}
	string getNName() {
		return this->node_name;
	}
	string getHNmae() {
		return this->highway_name;
	}
};

//��������

nodeInfo* info;//�� ���(����)�� ������ ��� 
edge* vertex; //�� ������ vertex�� ����� edge��
bool* visited; //�� vertex�� ���ͽ�Ʈ�� �˰��򿡼� ���Ǿ��°�?
int start_idx, dest_idx;//���� ������ id, ���� ������ id
stack<int> s, r;
int way_num[32];
string way_name[32];

#define MAX_NUM 515//�������� �� ����, �ڱ� �ڽ� �ߺ� ����
#define MAX_WAY 31//������ �� ����

//�Լ�
int loadFile(const char* name);
void inputInfo(bool menu);//���� �Է�
int find_min_edge();//���ͽ�Ʈ�� ����. ���� ���� ã��
void do_Dijkstra(int s_idx, int d_idx);//���ͽ�Ʈ��
bool find_route(stack<int> &s, int s_idx, int d_idx);//���ã��
void print_Result();//��� ���
void printMenu();//�޴� ���
int showWay(bool menu);//�� ���
void setWayName();//�ʱ⿡ �� ���� ����
void way_menu(bool menu);

int main() {

	int sel;//�޴� ���ð�
	bool loop = true; // �Ҹ޴� ����
	char goon;//�Ҹ޴� ��Ʈ �ߴ��� ���� y/n

	loadFile("��������.txt");
	setWayName();

	while (1) {

		printMenu();
		cin >> sel;
		loop = true;

		while (loop) {
			system("cls");
			switch (sel)
			{
			case 1:
				cout << "===============================================1. ���κ� ���� ����======================================" << endl;
				showWay(false);

				break;
			case 2:

				way_menu(false);

				break;
			case 3:
				way_menu(true);

				break;

			case 4:
				cout << "���α׷��� �����մϴ�" << endl;
				return 0;
			default:
				cout << "�������� �ʴ� �޴��Դϴ�. �ٽ� �Է����ּ���." << endl;
				loop = false;
				break;
			}

			cout << endl << "����Ͻðڽ��ϱ�?(y/n)" << endl;
			cin >> goon;

			if (goon == 'n') {
				loop = false;
			}
			system("cls");
		}
	}

	return 0;
}

void way_menu(bool menu) {

	double min;

	loadFile("��������.txt");

	//cout << endl;

	inputInfo(menu);

	do_Dijkstra(start_idx, dest_idx);


	min = vertex[dest_idx].dist;
	for (int i = 0; i < MAX_NUM; i++)
	{
		if (info[dest_idx].getNName() == info[i].getNName())
		{
			if (vertex[i].dist < min)
			{
				min = vertex[i].dist;
				dest_idx = i;
			}
		}
	}

	string str;
	int s_idx =start_idx , d_idx = dest_idx;

	if (menu) {
		str = info[start_idx].getNName();
		for (int i = 0; i < MAX_NUM; i++)
		{
			if (info[i].getNName() == str)
			{
				s_idx = i;
				break;
			}
		}

		str = info[dest_idx].getNName();
		for (int i = 0; i < MAX_NUM; i++)
		{
			if (info[i].getNName() == str)
			{
				d_idx = i;
				break;
			}
		}
	}


	find_route(s, s_idx, d_idx);
	print_Result();


}

void printMenu() {

	cout << "========================================================================================================" << endl;
	cout << "1 : ���κ� ���� ����   2 : �ִ� �Ÿ� ã��(���� �Է�)   3 : �ִ� �Ÿ� ã��(���κ� ����)    4 : �׸��ϱ�" << endl;
	cout << "========================================================================================================" << endl;
	cout << "�޴��� �Է����ּ��� : ";

}

void setWayName() {

	string str = "";
	int cnt = 0;

	for (int i = 0; i < MAX_NUM; i++) {
		if (str != info[i].getHNmae()) {
			str = info[i].getHNmae();
			way_name[cnt] = str;
			way_num[cnt] = i;
			cnt++;
		}
	}

}
int showWay(bool menu) {


	int sel;
	int end = MAX_NUM-1;
	bool loop = true;
	int i;

	for (i = 0; i < MAX_WAY; i++) {

			cout <<"\t" <<i + 1 << " : " << way_name[i] << "\t";
			if ((i % 2 == 1)) {
				cout << endl;
			}

	}

	cout << endl << "========================================================================================================" << endl;
	cout << "������� ������ ��ȣ�� �Է����ּ���. : ";
	cin >> sel;

	sel--;
	if (sel < (MAX_WAY-1)) {
		end = way_num[sel + 1];
	}

	system("cls");
	cout << endl << "######" << way_name[sel] << "######" << endl;
	for (i = way_num[sel]; i < end; i++) {
		cout << "\t"<<i - way_num[sel] + 1 <<" : " <<info[i].getNName()<<"\t" ;
		if ((i-way_num[sel]) % 4 == 3 ){
			cout <<  endl;
		}
	}

	if (menu) {
		cout << endl << "========================================================================================================" << endl;
		cout << "������ ��ȣ�� �Է����ּ���. : ";
		cin >> i;
		i = way_num[sel] + i - 1;
		return i;
	}

	return -1;
}

int loadFile(const char* name) {

	//num = MAX_NUM;

	ifstream file;

	edge* temp;
	edge* p;
	int n = MAX_NUM;
	string except_str = "ȯ�±���";

	int n1, n2;
	double dist;
	string s1, s2, s3;

	////////////////////////�������� �ʱ�ȭ
	vertex = new edge[n];
	info = new nodeInfo[n];
	visited = new bool[n];

	file.open(name, ios::in);


	while (file >> n1)
	{

		file >> n2;
		file >> dist;
		file >> s1;
		file >> s2;
		file >> s3;

		temp = new edge();//n1�� n2���� ������� �߰�
		temp->id = n2;
		temp->dist = dist;
		if (s1 == s2)//������
		{
			temp->t_num = 1;
		}
		else//���� ����
		{
			temp->t_num = 0;
		}


		p = &vertex[n1];//����1�� ��� 
		while (p->next != NULL)
		{
			p = p->next;
		}
		p->next = temp;


		temp = new edge();//n2�� n1���� ������� �߰�
		temp->id = n1;
		temp->dist = dist;
		if (s1 == s2)
		{
			temp->t_num = 1;
		}
		else
		{
			temp->t_num = 0;
		}


		p = &vertex[n2];
		while (p->next != NULL)
		{
			p = p->next;
		}
		p->next = temp;




		if (s3.compare(except_str) != 0) {
			info[n1].setString(s1, s3);
			info[n2].setString(s2, s3);
		}

		//info[n1]->node_name = s1;
		//info[n2] = s2;


	}

	file.close();

	return 0;
}

void inputInfo(bool menu) {

	string str;

	start_idx = -999;
	dest_idx = -999;


	while (start_idx == dest_idx)
	{

		if (menu == false) {
			cout << "======================================2. �ִ� �Ÿ� ã�� (���� �Է�)======================================" << endl;
			while (start_idx < 0)
			{



				cout << "������� �Է����ּ��� : ";
				cin >> str;


				for (int i = 0; i < MAX_NUM; i++)
				{
					if (info[i].getNName() == str)
					{
						start_idx = i;
						break;
					}
				}

				//������
				if (start_idx < 0)
				{
					cout << "�������� �ʴ� �����Դϴ�. �ٽ� �Է����ּ���." << endl;

				}
			}

			while (dest_idx < 0)
			{

				cout << "�������� �Է����ּ��� : ";
				cin >> str;
				cout << endl;



				for (int i = 0; i < MAX_NUM; i++)
				{
					if (info[i].getNName() == str)
					{
						dest_idx = i;
						break;
					}
				}


				if (dest_idx < 0)
				{
					cout << "�������� �ʴ� �����Դϴ�. �ٽ� �Է����ּ���." << endl;

				}

			}
		}
		else {
		
			cout << "######������� �Է����ּ���######" << endl;
			cout << "=====================================3. �ִ� �Ÿ� ã�� (���ο��� ����)==================================" << endl;
			start_idx = showWay(true);
			system("cls");
			cout << "######�������� �Է����ּ���######" << endl;
			cout << "=====================================3. �ִ� �Ÿ� ã�� (���ο��� ����)==================================" << endl;
			dest_idx = showWay(true);
		
		}

		
		if (start_idx == dest_idx)
		{
			cout << "������� �������� �����ϴ�. �ٽ� �Է����ּ���" << endl;
			system("cls");
			start_idx = -999;
			dest_idx = -999;
		}

	}



	cout << "������ ������ " << info[start_idx].getHNmae() << "�� " << info[start_idx].getNName() << " ���� " << info[dest_idx].getHNmae() << "�� " << info[dest_idx].getNName() << "�Դϴ�." << endl;
	cout << "========================================================================================================" << endl;


}


int find_min_edge()
{
	int min_t_num = 9999, pos = -1;
	double min = 99999.0;

	for (int i = 0; i < MAX_NUM; i++)
	{
		if (!visited[i])
		{
			if (vertex[i].dist < min)
			{
				min = vertex[i].dist;
				min_t_num = vertex[i].t_num;
				pos = i;
			}
			else if (vertex[i].dist == min)
			{
				if (vertex[i].t_num < min_t_num)
				{
					min = vertex[i].dist;
					min_t_num = vertex[i].t_num;
					pos = i;
				}
			}
		}
	}


	return pos; // �ε��� ��ȯ
}


void do_Dijkstra(int s_idx, int d_idx) // Dijskstra Algorithm 
{
	int minedge;


	// �׷����� visited�迭 �ʱ�ȭ
	for (int i = 0; i< MAX_NUM; i++)
	{
		visited[i] = false;
		vertex[i].id = -1;
		vertex[i].dist = 99999.0;
		vertex[i].t_num = 9999;
	}
	//cout << "1" << endl;


	for (edge* p = vertex[s_idx].next; p != NULL; p = p->next)
	{

		if (info[s_idx].getNName() == info[p->id].getNName())

		{
			vertex[p->id].dist = 0;
			vertex[p->id].t_num = 0;
		}
		else // �Ϲ����� ���
		{
			vertex[p->id].dist = p->dist;
			vertex[p->id].t_num = p->t_num;
		}
	}
	//cout << "1" << endl;


	//for (int i = 0; i < num; i++)
	//{
	//	if (info[s_idx].getNName() == info[i].getNName())
	//	{
	//		for (edge* p = vertex[i].next; p != NULL; p = p->next)
	//		{
	//			if (info[i].getNName() == info[p->id].getNName())
	//			{
	//				p->dist = 0;
	//				p->t_num = 0;
	//			}
	//		}
	//	}
	//}
	//cout << "2" << endl;


	//visited[s_idx] = s_idx;
	visited[s_idx] = true;

	vertex[s_idx].dist = 0.0;
	vertex[s_idx].t_num = 0;

	//cout << "3" << endl;

	for (int i = 0; i < MAX_NUM - 2; i++)
	{
		// �ּ��� ���ؽ� ����
		minedge = find_min_edge();




		visited[minedge] = true;
		//cout << "5\n";


		for (edge* p = vertex[minedge].next; p != NULL; p = p->next)
		{



			if (vertex[minedge].dist + p->dist < vertex[p->id].dist)
			{
				vertex[p->id].dist = vertex[minedge].dist + p->dist;
				vertex[p->id].t_num = vertex[minedge].t_num + p->t_num;
			}
			else if (vertex[minedge].dist + p->dist == vertex[p->id].dist)
			{
				if (vertex[minedge].t_num + p->t_num < vertex[p->id].t_num)
				{
					vertex[p->id].dist = vertex[minedge].dist + p->dist;
					vertex[p->id].t_num = vertex[minedge].t_num + p->t_num;
				}
			}


		}
		if (minedge == d_idx) {
			return;
		}

		//cout << "6\n";
		//prev = minpos;

	}

}

bool find_route(stack<int> &s, int s_idx, int d_idx) {

	s.push(s_idx);

	if (s_idx == d_idx)
		return true;

	for (edge* p = vertex[s_idx].next; p != NULL; p = p->next)
	{

		if (vertex[s_idx].dist + p->dist == vertex[p->id].dist && vertex[s_idx].t_num + p->t_num == vertex[p->id].t_num)
		{
			if (!find_route(s, p->id, d_idx))
			{
				s.pop();
			}
			else
			{
				return true;
			}
		}
	}
	return false;
}

void print_Result()
{


	int i, pre;

	while (!s.empty())
	{
		r.push(s.top());
		s.pop();
	}
	// ��� ���
	for (i = 0, pre = -1; !r.empty(); i++)
	{
		if (pre != -1 && info[pre].getNName() == info[r.top()].getNName())
		{
			//if (info[r.top()].getNName() != info[start_idx].getNName()) {
			if ((info[r.top()].getHNmae() != info[pre].getHNmae()) && (info[r.top()].getNName() != info[dest_idx].getNName())) {
				cout << "(" << info[r.top()].getHNmae() << "�� ���� ȯ��)";
			
			}

			i--;
			pre = r.top();
			r.pop();
		}
		else
		{
			if (i != 0)
				cout << " => ";
			else
				cout << " (" << info[start_idx].getHNmae() << ")";

			cout << info[r.top()].getNName();
			pre = r.top();
			r.pop();

		}
	}
	cout << endl << endl << "�ִ� �Ÿ� Ž�� ��� " << i - 1 << "���� ���� ������ " << vertex[dest_idx].t_num << "�� ȯ���մϴ�." << endl
		<< "�� �Ÿ��� " << vertex[dest_idx].dist << "km �Դϴ�. " << endl;

	cout << endl;


}
