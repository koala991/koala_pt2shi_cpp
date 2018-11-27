#include <windows.h>
using namespace std;

struct arr_p {
	int cols;
	int rows;
	int *task_id;
	char *arr;
};

HANDLE hmutex;

void arr_test_a(arr_p *ppp)
{
	while (true)
	{
		WaitForSingleObject(hmutex, INFINITE);
		if (*ppp->task_id >= ppp->cols*ppp->rows) break;
		//*(ppp->arr + *(ppp->task_id)) = *(ppp->task_id) + 1;
		*(ppp->task_id) = *(ppp->task_id) + 1;
		*(ppp->arr + *(ppp->task_id) - 1) = 'a';
		Sleep(10);
		ReleaseMutex(hmutex);
	}
}
void arr_test_b(arr_p *ppp)
{
	while (true)
	{
		WaitForSingleObject(hmutex, INFINITE);
		if (*ppp->task_id >= ppp->cols*ppp->rows) break;
		//*(ppp->arr + *(ppp->task_id)) = *(ppp->task_id) + 1;
		*(ppp->task_id) = *(ppp->task_id) + 1;
		*(ppp->arr + *(ppp->task_id) - 1) = 'b';
		Sleep(10);
		ReleaseMutex(hmutex);
	}
}
void arr_test_c(arr_p *ppp)
{
	while (true)
	{
		WaitForSingleObject(hmutex, INFINITE);
		if (*ppp->task_id >= ppp->cols*ppp->rows) break;
		//*(ppp->arr + *(ppp->task_id)) = *(ppp->task_id) + 1;
		*(ppp->task_id) = *(ppp->task_id) + 1;
		*(ppp->arr + *(ppp->task_id) - 1) = 'c';
		Sleep(10);
		ReleaseMutex(hmutex);
	}
}

int main()
{
	arr_p * pp = new arr_p();
	pp->cols = 5;
	pp->rows = 12;
	char arr[12][5];
	int task_id = 0;
	pp->task_id = &task_id;
	pp->arr = arr[0];
	hmutex = CreateMutex(NULL, FALSE, NULL);
	HANDLE h1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)arr_test_a, pp, 1, 0);
	ResumeThread(h1);
	CloseHandle(h1);
	//HANDLE h2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)arr_test_b, pp, 1, 0);
	//ResumeThread(h2);
	//CloseHandle(h2);
	HANDLE h3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)arr_test_c, pp, 1, 0);
	ResumeThread(h3);
	CloseHandle(h3);
	while (task_id < 60)
	{
		Sleep(500);
	}
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 5; j++)	cout << arr[i][j] << " ";
		cout << '\n';
	}
	system("pause");
	return 0;
}
