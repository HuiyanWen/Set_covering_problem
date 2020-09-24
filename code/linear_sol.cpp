#include "stdafx.h"
#include <glpk.h>
#include "linear_sol.h"
#include "init.h"
#include "utils.h"
#include "greedy_search.h"

void linear_sol(char *init_path) {
	const char *data_path;
	int size, whether_precise;
	map<string, string>fname;
	CParseIniFile config;
	bool flag = config.ReadConfig(init_path, fname, "2");
	if (flag) {
		size = stoi(fname["size"]);
		whether_precise = stoi(fname["whether_precise"]);
		data_path = fname["data_path"].c_str();
	}
	else {
		cout << "Loading ini 2 error!" << endl;
		return;
	}
	vector<int> X;
	vector<int> S;
	vector<int> S_num;
	cout << "Loading data." << endl;
	Load_tf(data_path, S, S_num, size, X);
	cout << "Loading finished." << endl;


	double *es = (double *)malloc((size * size + 1) * sizeof(double));
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			es[i*size + j + 1] = 0;
	int max_num = 0;
	int max_temp;
	for (int i = 0; i < size; i++) {		
		for (int j = 0; j < size; j++) {
			for (int k = 0; k < S_num[j]; k++) {
				if (X[i] == S[sum_num(S_num, j) + k]) {
					es[i*size + j + 1] = 1;
				}					
			}
		}
	}

	for (int i = 0; i < size; i++) {
		max_temp = 0;
		for (int j = 0; j < size; j++) {
			max_temp += es[i*size + j + 1];
		}
		if (max_temp > max_num) max_num = max_temp;
	}
	cout << "f:"<< max_num << endl;
	/*for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			cout << es[i*size + j+1] << " ";
		}
		cout << endl;
	}
	exit(-1);*/
	clock_t begin, end;
	begin = clock();
	glp_prob *lp;
	lp = glp_create_prob();
	glp_set_prob_name(lp, "sample");
	glp_set_obj_dir(lp, GLP_MIN);//maximization

	glp_add_rows(lp, size);
	for (int i = 1; i <= size; i++) {			
		glp_set_row_bnds(lp, i, GLP_LO, 1.0, 0.0);
	}

	glp_add_cols(lp, size);
	for (int i = 1; i <= size; i++) {	
		if(whether_precise)
			glp_set_col_kind(lp, i, GLP_IV);
		glp_set_col_bnds(lp, i, GLP_UP, 0.0, 1.0);
		glp_set_col_bnds(lp, i, GLP_LO, 0.0, 0.0);		
		glp_set_obj_coef(lp, i, 1.0);
		
	}
	
	int *ia = (int *)malloc((size * size + 1) * sizeof(int));
	int *ja = (int *)malloc((size * size + 1) * sizeof(int));
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			ia[i*size + j + 1] = i + 1;
			ja[i*size + j + 1] = j + 1;
		}		
	}

	glp_load_matrix(lp, size*size, ia, ja, es); //将约束系数矩阵导入

	glp_iocp param;
	glp_init_iocp(&param);
	param.presolve = GLP_ON;    
	param.msg_lev = GLP_MSG_ON;
	glp_intopt(lp, &param);

	//glp_simplex(lp, NULL); //simplex method使用单纯形法求解
	//glp_interior(lp, NULL);
	vector<int> C;
	cout << glp_mip_obj_val(lp) << endl;
	for (int i = 1; i <= size; i++) {
		if (glp_mip_col_val(lp, i) != 0) {
			//cout << i << ":" << glp_mip_col_val(lp, i) << endl;
			if (glp_mip_col_val(lp, i) >= (1.0 / max_num))
				C.push_back(i-1);
		}			
	}	
	end = clock();
	glp_delete_prob(lp);

	cout << "总所需子集数:";
	cout << C.size() << endl;
	cout << "C:";
	for (int i = 0; i < C.size(); i++) cout << C[i] << " ";
	cout << endl;
	cout << "是否覆盖了原集合X：" << check_right(C, X, S_num, S) << endl;
	cout << "Total time:" << end - begin << endl;
}

