#include "stdafx.h"
#include "init.h"
#include "utils.h"

using namespace std;

bool check_repeate(vector<int> &data, int temp) {
	for (int i = 0; i < data.size(); i++) {
		if (data[i] == temp) return false;
	}
	return true;
}

int real_size(vector<int> &data) {
	int sum = 0;
	for (int i = 0; i < data.size(); i++) {
		if (data[i] != -1) sum++;
	}
	return sum;
}

void generate_node(char *init_path) {
	//const char *img_path;
	const char *data_path;
	int size, max_node;
	map<string, string>fname;
	CParseIniFile config;
	bool flag = config.ReadConfig(init_path, fname, "0");
	if (flag) {
		size = stoi(fname["size"]);
		max_node = stoi(fname["max_node"]);
		data_path = fname["data_path"].c_str();
	}
	else {
		cout << "Loading ini 0 error!" << endl;
		return;
	}
	/*int *dot = (int *)malloc((dotnum * 2) * sizeof(int));
	if (dotnum < 3) {
		cout << "Please generate more than 3 dots." << endl;
		exit(-1);
	}
	Load_file(dot_path, dot);*/

	vector<int> X;
	int gnt_node;	

	srand((unsigned)time(0));
	for (int i = 0; i < size; i++) {
		flag = false;
		do {
			gnt_node = myrandom(size * 10);
			if (check_repeate(X, gnt_node)) flag = true;
		} while (!flag);
		X.push_back(gnt_node);
	}
	
	vector<int> X_cpy;
	for (int i = 0; i < X.size(); i++) {
		X_cpy.push_back(X[i]);
	}
	//S0
	int s_num = 0;
	vector<int> temp_select;
	vector<int> S;
	vector<int> S_bing;

	for (int i = 0; i < max_node; i++) {
		flag = false;
		do {
			gnt_node = myrandom(X.size());
			if (check_repeate(temp_select, gnt_node)) flag = true;
		} while (!flag);
		temp_select.push_back(gnt_node);
	}
	for (int i = 0; i < temp_select.size(); i++) {
		S.push_back(X[temp_select[i]]);
		S_bing.push_back(X[temp_select[i]]);
		X[temp_select[i]] = -1;
	}
	s_num++;
	//cout << "yes" << endl;
	/*for (int i = 0; i < X.size(); i++) {
		cout << X[i] << " ";
	}
	cout << endl;*/

	
	/*cout << "****" << endl;
	for (int i = 0; i < S.size(); i++) {
		cout << S[i] << " ";
	}
	cout << endl;

	cout << "****" << endl;
	for (int i = 0; i < X.size(); i++) {
		if(X[i]!=-1)
			cout << X[i] << " ";
	}
	cout << endl;
	cout << real_size(X) << endl;;
	cout << S.size() << endl;*/
	FILE *fp;
	errno_t err;
	if ((err = fopen_s(&fp, data_path, "ab+")) != 0)
		printf("Open dst file failed.\n");

	cout << "Writing tf..." << endl;
	int s_size = S.size();
	
	fwrite(&size, sizeof(int), 1, fp);
	for (int i = 0; i <size; ++i)
	{
		fwrite(&X_cpy[i], sizeof(int), 1, fp);
	}
	fwrite(&s_size, sizeof(int), 1, fp);
	for (int i = 0; i < S.size(); ++i)
	{
		fwrite(&S[i], sizeof(int), 1, fp);
	}
	
	int rand_n;
	int rand_x;
	
	while (real_size(X) >= max_node) {
		S.erase(S.begin(), S.end());
		temp_select.erase(temp_select.begin(), temp_select.end());
		rand_n = myrandom(max_node) + 1;//1~20
		rand_x = myrandom(rand_n) + 1;//1~n
		//X-Sbing x
		for (int i = 0; i < rand_x; i++) {
			flag = false;
			do {
				gnt_node = myrandom(X.size());
				if (check_repeate(temp_select, gnt_node)&&X[gnt_node]!=-1) flag = true;
			} while (!flag);
			temp_select.push_back(gnt_node);
		}
		for (int i = 0; i < temp_select.size(); i++) {
			S.push_back(X[temp_select[i]]);
			S_bing.push_back(X[temp_select[i]]);
			X[temp_select[i]] = -1;
		}
		//Sbing n-x
		temp_select.erase(temp_select.begin(), temp_select.end());
		for (int i = 0; i < rand_n - rand_x; i++) {
			flag = false;
			do {
				gnt_node = myrandom(S_bing.size());
				if (check_repeate(temp_select, gnt_node)) flag = true;
			} while (!flag);
			temp_select.push_back(gnt_node);
		}
		for (int i = 0; i < temp_select.size(); i++) {
			S.push_back(S_bing[temp_select[i]]);
		}
		s_num++;
		s_size = S.size();
		fwrite(&s_size, sizeof(int), 1, fp);
		for (int i = 0; i < S.size(); ++i)
		{
			fwrite(&S[i], sizeof(int), 1, fp);
		}
		//cout << "yes" << endl;
	}
	
	S.erase(S.begin(), S.end());
	rand_n = real_size(X);

	fwrite(&rand_n, sizeof(int), 1, fp);

	for (int i = 0; i < X.size(); i++) {
		if (X[i] != -1) S.push_back(X[i]);
	}

	for (int i = 0; i < S.size(); ++i)
	{
		fwrite(&S[i], sizeof(int), 1, fp);
	}
	s_num++;
	//cout << s_num << endl;
	//cout << real_size(X_cpy) << endl;
	//1~10 X_cpy
	for (int i = 0; i <= size - s_num; i++) {
		temp_select.erase(temp_select.begin(), temp_select.end());
		S.erase(S.begin(), S.end());
		rand_n = myrandom(10) + 1;
		for (int i = 0; i < rand_n; i++) {
			flag = false;
			do {
				gnt_node = myrandom(X_cpy.size());
				if (check_repeate(temp_select, gnt_node)) flag = true;
			} while (!flag);
			temp_select.push_back(gnt_node);
		}
		for (int i = 0; i < temp_select.size(); i++) {
			S.push_back(X_cpy[temp_select[i]]);
		}
		s_size = S.size();
		fwrite(&s_size, sizeof(int), 1, fp);
		for (int i = 0; i < S.size(); ++i)
		{
			fwrite(&S[i], sizeof(int), 1, fp);
		}
		//cout << S.size() << endl;
		//cout << i << endl;
	}
	fclose(fp);
	cout << "Finished." << endl;
	//cout << s_num << endl;
	//cout << real_size(X) << endl;
	//cout << S_bing.size() << endl;



}