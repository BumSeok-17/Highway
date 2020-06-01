
#include <stack> //다익스트라로 찾은 그래프 경로 탐색
#include <string> 
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

//클래스

class edge
{
public:
	int id; //각 도시의 id
	double dist; //도시와 도시 사이의 거리
	int t_num; //고속도로 환승 횟수
	edge* next;//다음거
	edge() : next(NULL) {}//null로 초기화
};

class nodeInfo
{
	string node_name;//도시 이름
	string highway_name;//도시가 속한 고속도로 이름

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

//전역변수

nodeInfo* info;//각 노드(도시)들 정보가 담김 
edge* vertex; //각 꼭짓점 vertex에 연결된 edge들
bool* visited; //이 vertex가 다익스트라 알고리즘에서 사용되었는가?
int start_idx, dest_idx;//시작 도시의 id, 도착 도시의 id
stack<int> s, r;
int way_num[32];
string way_name[32];

#define MAX_NUM 515//엣지들의 총 개수, 자기 자신 중복 포함
#define MAX_WAY 31//도로의 총 개수

//함수
int loadFile(const char* name);
void inputInfo(bool menu);//도시 입력
int find_min_edge();//다익스트라에 쓰임. 작은 엣지 찾기
void do_Dijkstra(int s_idx, int d_idx);//다익스트라
bool find_route(stack<int> &s, int s_idx, int d_idx);//경로찾기
void print_Result();//결과 출력
void printMenu();//메뉴 출력
int showWay(bool menu);//길 출력
void setWayName();//초기에 한 번만 수행
void way_menu(bool menu);

int main() {

	int sel;//메뉴 선택값
	bool loop = true; // 소메뉴 루프
	char goon;//소메뉴 루트 중단을 물음 y/n

	loadFile("구간정보.txt");
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
				cout << "===============================================1. 도로별 도시 보기======================================" << endl;
				showWay(false);

				break;
			case 2:

				way_menu(false);

				break;
			case 3:
				way_menu(true);

				break;

			case 4:
				cout << "프로그램을 종료합니다" << endl;
				return 0;
			default:
				cout << "존재하지 않는 메뉴입니다. 다시 입력해주세요." << endl;
				loop = false;
				break;
			}

			cout << endl << "계속하시겠습니까?(y/n)" << endl;
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

	loadFile("구간정보.txt");

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
	cout << "1 : 도로별 도시 보기   2 : 최단 거리 찾기(직접 입력)   3 : 최단 거리 찾기(도로별 선택)    4 : 그만하기" << endl;
	cout << "========================================================================================================" << endl;
	cout << "메뉴를 입력해주세요 : ";

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
	cout << "보고싶은 도로의 번호를 입력해주세요. : ";
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
		cout << "도시의 번호를 입력해주세요. : ";
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
	string except_str = "환승구간";

	int n1, n2;
	double dist;
	string s1, s2, s3;

	////////////////////////전역변수 초기화
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

		temp = new edge();//n1에 n2와의 연결관계 추가
		temp->id = n2;
		temp->dist = dist;
		if (s1 == s2)//교차로
		{
			temp->t_num = 1;
		}
		else//같은 도로
		{
			temp->t_num = 0;
		}


		p = &vertex[n1];//도시1의 헤드 
		while (p->next != NULL)
		{
			p = p->next;
		}
		p->next = temp;


		temp = new edge();//n2에 n1과의 연결관계 추가
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
			cout << "======================================2. 최단 거리 찾기 (직접 입력)======================================" << endl;
			while (start_idx < 0)
			{



				cout << "출발지를 입력해주세요 : ";
				cin >> str;


				for (int i = 0; i < MAX_NUM; i++)
				{
					if (info[i].getNName() == str)
					{
						start_idx = i;
						break;
					}
				}

				//없으면
				if (start_idx < 0)
				{
					cout << "존재하지 않는 도시입니다. 다시 입력해주세요." << endl;

				}
			}

			while (dest_idx < 0)
			{

				cout << "도착지를 입력해주세요 : ";
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
					cout << "존재하지 않는 도시입니다. 다시 입력해주세요." << endl;

				}

			}
		}
		else {
		
			cout << "######출발지를 입력해주세요######" << endl;
			cout << "=====================================3. 최단 거리 찾기 (도로에서 선택)==================================" << endl;
			start_idx = showWay(true);
			system("cls");
			cout << "######도착지를 입력해주세요######" << endl;
			cout << "=====================================3. 최단 거리 찾기 (도로에서 선택)==================================" << endl;
			dest_idx = showWay(true);
		
		}

		
		if (start_idx == dest_idx)
		{
			cout << "출발지과 도착지가 같습니다. 다시 입력해주세요" << endl;
			system("cls");
			start_idx = -999;
			dest_idx = -999;
		}

	}



	cout << "선택한 구간은 " << info[start_idx].getHNmae() << "의 " << info[start_idx].getNName() << " 에서 " << info[dest_idx].getHNmae() << "의 " << info[dest_idx].getNName() << "입니다." << endl;
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


	return pos; // 인덱스 반환
}


void do_Dijkstra(int s_idx, int d_idx) // Dijskstra Algorithm 
{
	int minedge;


	// 그래프와 visited배열 초기화
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
		else // 일반적인 경우
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
		// 최소인 버텍스 선택
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
	// 경로 출력
	for (i = 0, pre = -1; !r.empty(); i++)
	{
		if (pre != -1 && info[pre].getNName() == info[r.top()].getNName())
		{
			//if (info[r.top()].getNName() != info[start_idx].getNName()) {
			if ((info[r.top()].getHNmae() != info[pre].getHNmae()) && (info[r.top()].getNName() != info[dest_idx].getNName())) {
				cout << "(" << info[r.top()].getHNmae() << "로 도로 환승)";
			
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
	cout << endl << endl << "최단 거리 탐색 결과 " << i - 1 << "개의 도시 지나며 " << vertex[dest_idx].t_num << "번 환승합니다." << endl
		<< "총 거리는 " << vertex[dest_idx].dist << "km 입니다. " << endl;

	cout << endl;


}
