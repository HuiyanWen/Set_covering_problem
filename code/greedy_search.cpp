#include "stdafx.h"
#include "utils.h"
#include "init.h"
#include "generate_node.h"

int cover_num(vector<int> &U, vector<int> &S, int num, int begin_num) {
	int cover = 0;
	for (int i = 0; i < num; i++) {
		for (int j = 0; j < U.size(); j++) {
			if (U[j] == -1) continue;
			else if (S[begin_num + i] == U[j]) {
				cover++;
				continue;
			}
		}
	}
	return cover;
}

int sum_num(vector<int> &S_num, int num) {
	int sum = 0;
	for (int i = 0; i < num; i++) {
		sum += S_num[i];
	}
	return sum;
}

bool check_right(vector<int> C, vector<int> X, vector<int> S_num, vector<int> S) {
	
	int *flag = (int *)malloc(X.size() * sizeof(int));
	for (int i = 0; i < X.size(); i++) flag[i] = 0;
	for (int i = 0; i < C.size(); i++) {		
		for (int j = 0; j < S_num[C[i]]; j++) {
			for (int k = 0; k < X.size(); k++) {
				if (X[k] == S[sum_num(S_num, C[i]) + j])
					flag[k] = 1;
					//S[sum_num(S_num, max_cover) + i]
			}
		}
		
	}
	for (int i = 0; i < X.size(); i++) {
		if (flag[i] == 0) {
			free(flag);
			return false;
		}
	}
	free(flag);
	return true;
}

void greedy_search(char *init_path) {
	const char *data_path;
	int size, whether_show_process;
	map<string, string>fname;
	CParseIniFile config;
	bool flag = config.ReadConfig(init_path, fname, "1");
	if (flag) {
		size = stoi(fname["size"]);
		whether_show_process = stoi(fname["whether_show_process"]);
		data_path = fname["data_path"].c_str();
	}
	else {
		cout << "Loading ini 1 error!" << endl;
		return;
	}
	vector<int> U;
	vector<int> X;
	vector<int> S;
	vector<int> S_num;
	cout << "Loading data." << endl;
	Load_tf(data_path, S, S_num, size, U);
	cout << "Loading finished." << endl;
	/*cout << sum_num(S_num, size-1) << endl;
	cout << S_num[size - 1] << endl;
	cout << S.size() << endl;
	cout << U.size() << endl;
	exit(-1);*/
	int max_f = 0;
	for (int i = 0; i < S_num.size(); i++) {
		if (S_num[i] > max_f) max_f = S_num[i];
	}
	cout << "f:" << max_f << endl;


	clock_t begin, end;
	begin = clock();

	for (int i = 0; i < U.size(); i++) X.push_back(U[i]);
	vector<int> C;
	vector<int> S_flag;//集合下标标记
	for (int i = 0; i < S_num.size(); i++) {
		S_flag.push_back(i);
	}
	int max_cover_num = 0;
	int max_cover = -1;
	int cover_temp;
	int print_temp = 0;

	while (real_size(U)>0) {
		print_temp++;
		//chose S
		max_cover_num = 0;
		max_cover = -1;

		for (int i = 0; i < S_flag.size(); i++) {
			//找S中cover U的最大的集合
			if (S_flag[i] != -1) {
				cover_temp = cover_num(U, S, S_num[i], sum_num(S_num, i));
				if(cover_temp>max_cover_num) {									
					max_cover_num = cover_temp;
					max_cover = i;
				}			
			}
		}
		S_flag[max_cover] = -1;
		C.push_back(max_cover);
		for (int i = 0; i < S_num[max_cover]; i++) {
			for (int j = 0; j < U.size(); j++) {
				if (U[j] == -1) continue;
				else if (S[sum_num(S_num, max_cover) + i] == U[j]) {
					U[j] = -1;
				}
			}
		}
		//cout << real_size(U) << endl;
		//exit(-1);
		if (whether_show_process) {
			cout << "第" << print_temp << "次新增元素:";
			for (int i = 0; i < S_num[max_cover]; i++) {
				cout << S[sum_num(S_num, max_cover) + i] << " ";
			}
			cout << endl;
			
		}		
		cout << "第" << print_temp << "次剩余元素:";
		cout << real_size(U) << endl;
	}
	end = clock();
	cout << "总所需子集数:";
	cout << C.size() << endl;
	cout << "C:";
	for (int i = 0; i < C.size(); i++) cout << C[i] << " ";
	cout << endl;
	cout << "是否覆盖了原集合X：" << check_right(C, X, S_num, S) << endl;
	cout << "Total time:" << end - begin << endl;
	//for(int i = 0; i<20 ;i++) cout << S_num[i] << endl;
}