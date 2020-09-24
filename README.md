# 求解集合覆盖问题

## 子集生成算法
![](https://github.com/HuiyanWen/Set_covering_problem/blob/master/pic/QQ20200924-201816.png)

## 贪心近似求解
	算法Greedy_cover(X, F)
	输入 : 有限集X, X的子集合族F， X=∪S∈F S，|X|=|F|
	输出 : C⊆F，满足X=∪S∈F S且C是满足X=∪S∈F S的最小集族，即|C|最小
	1:  U←X
	2:  C←θ
	3:   While   U≠θ   Do
	4:      贪心选择能覆盖最多U元素的子集S
	5:      U←U-S
	6:      C←C∪{S}
	7:   Return C
在数据集元素个数等于500时，我们采用贪心求解方法，得到了图2所示结果。可以看到，每一步贪心搜索所新增的元素都有打印。最终需要的子集个数为69个，“C:”处输出的是S中的第多少个子集。最后我们进行了正确性检验，覆盖了原集合X，算法正确。

![](https://github.com/HuiyanWen/Set_covering_problem/blob/master/pic/%E8%B4%AA%E5%BF%83%E8%BF%91%E4%BC%BC.png)

## 线性规划舍入法求解
	线性规划舍入算法
	算法Appox_Cover_VC(X, F)
	输入 : 有限集X, X的子集合族F， X=∪S∈F S，|X|=|F|
	输出 : C⊆F，满足X=∪S∈F S且C是满足X=∪S∈F S的最小集族，即|C|最小
	1:  根据X和F的关系，构建线性规划表达式
	2:  调用glpk库求解LP松弛问题的最优解x
	3:   For   S∈F   Do
	4:      If xs≥1/f   then   C←C∪{S}
	5:   Return C

![](https://github.com/HuiyanWen/Set_covering_problem/blob/master/pic/%E8%88%8D%E5%85%A5%E6%B3%95.png)

## 精确解
在上面线性规划问题的基础上，将其改造为整数规划问题，限定xs的值为0或1，可得到精确解。

![](https://github.com/HuiyanWen/Set_covering_problem/blob/master/pic/%E7%B2%BE%E7%A1%AE%E8%A7%A3.png)

## 三种方法对比
![](https://github.com/HuiyanWen/Set_covering_problem/blob/master/pic/%E8%BF%91%E4%BC%BC%E6%AF%94%E5%AF%B9%E6%AF%94.png)
