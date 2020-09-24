#include "stdafx.h"
#include "init.h"
#include "generate_node.h"
#include "greedy_search.h"
#include "stdio.h"
#include "glpk.h"
#include "linear_sol.h"

int main()
{
	map<string, string>fname;
	CParseIniFile config;
	char* config_path = "config.ini";
	bool flag = config.ReadConfig(config_path, fname, "method");
	if (!flag) return -1;
	switch (stoi(fname["method"])) {
	case 0:
		generate_node(config_path);
		break;
	case 1:
		greedy_search(config_path);
		break;		
	case 2:
		linear_sol(config_path);
		break;
	}

	cout << "Please click enter to exit." << endl;
	char ch;
	while (1) {
		if ((ch = getchar()) == '\n')
			break;
	}
    return 0;
}
