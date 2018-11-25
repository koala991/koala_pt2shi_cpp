#include <iostream>
#include <constant.h>
#include <fun0.h>
using namespace std;

void test0() {
	// test on switch arr 
	double arr[3] = { 0, 1, 2 };
	int i = 0;
	array_queue_on(arr, 6);
	while (i < 3)
	{
		cout << arr[i++] << endl;
	}
}

void test1() {
	// test on constant set
	extern int INTE_BLOCK;
	cout << INTE_BLOCK << endl;
}
https://github.com/koala991/koala_pt2shi_cpp.git

int main()
{
	test1();
	return 0;
}
