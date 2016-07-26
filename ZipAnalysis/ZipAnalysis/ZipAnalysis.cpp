/*
	���� ���丮 �ȿ����� ��� zip������ �о�� ��
	ó�� 2byte�� Ȯ���Ͽ� zip�������� �ƴ����� ������ Ȯ���ϴ� ���α׷�
*/

#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <string.h>

DWORD zipcnt = 0;//��¥ zip������ ����

 TCHAR* SignatureCmp(HANDLE handle) {
	char signature[3];//�ñ״�ó �������
	memset(signature, 0, sizeof(signature));//�ñ״�ó ����� �ʱ�ȭ
	DWORD result;//���� ����Ʈ ��

	char cmp[3];//�ñ״�ó ��
	memset(cmp, 0, sizeof(cmp));//�ñ״�ó ���� ���� �ʱ�ȭ
	strcpy_s(cmp, "PK");//�ñ״�ó pk ����

	if (ReadFile(handle, signature, sizeof(char) * 2, &result, NULL) == 0)//���� 2����Ʈ��ŭ �о�ͼ� signature�� ����
	{
		_tprintf(_T("ReadFile error. Error is %u\n"), GetLastError());
		exit(1);
	}

	if (strcmp(signature, cmp) == 0)//�ñ״�ó �� �κ�
	{
		zipcnt++;
		TCHAR *res = _T("Real zip File");
		return res;
	}
	TCHAR *res = _T("No");
	return res;
}

int _tmain(int argc, LPTSTR argv[])
{
	HANDLE hFind;//FirstFile �ڵ� ����
	//HANDLE File[1000];//ã�� zip���� �ڵ� ����
	HANDLE hFile;//zip���� �ڵ� ����
	TCHAR Path[MAX_PATH];//�۾����丮 ������ ��
	WIN32_FIND_DATA wfd;//ã�� ����, ������丮 ���� ���
	TCHAR filename[MAX_PATH];//�����̸� ������ ����
	DWORD dwError;//�����޼��� ����
	DWORD cnt=0;//���� ���� ����

	/*
	char signature[3];//�ñ״�ó �������
	memset(signature, 0, sizeof(signature));//�ñ״�ó ����� �ʱ�ȭ
	DWORD result;//���� ����Ʈ ��
	*/

	//���� �۾� ���丮 ������
	GetCurrentDirectory(MAX_PATH, Path);
	//�۾� ���丮 ���� \*.zip ���ڿ� ���̱�
	_tcsncat_s(Path, _T("\\*.zip"), 7);
	//���� �۾� ���丮 ���
	_tprintf(_T("Current Directory : %s\n"), Path);
	
	hFind = FindFirstFile(Path,&wfd);//���� ó�� ���� �ڵ� ���
	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("Invalid file handle. Error is %u\n", GetLastError());
		return -1;
	}
	else
	{
		_tcsncpy_s(filename, wfd.cFileName, 260);//���� �̸� ����
		hFile = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			printf("Invalid file handle. Error is %u\n", GetLastError());
			return -1;
		}
		_tprintf(_T("file name : %s\t%s\n"), filename, SignatureCmp(hFile));//���� �̸� ���
		//File[cnt] = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);//���� �ڵ� ������
		

		cnt++;//���� ���� 1 ����

		while (FindNextFile(hFind, &wfd) != 0)//���� ���� ã��
		{
			_tcsncpy_s(filename, wfd.cFileName, 260);//���� �̸� ����
			hFile = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
			_tprintf(_T("file name : %s\t%s\n"), filename, SignatureCmp(hFile));//���� �̸� ���
			if (hFile == INVALID_HANDLE_VALUE)
			{
				printf("Invalid file handle. Error is %u\n", GetLastError());
				return -1;
			}
			//File[cnt] = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);//���� �ڵ� ������
			cnt++;//���� ���� 1 ����
		}
		dwError = GetLastError();//�����޼��� ����
		FindClose(hFind);//ã�� �׸�
		if (dwError != ERROR_NO_MORE_FILES)//���̻� ���� ��� ������ �ƴҶ�
		{
			_tprintf(_T("FindNextFile error. Error is %u\n"), dwError);
			return -1;
		}
	}
	
	_tprintf(_T("Total zip file count : %d\n"), cnt);//�� zip���� ���� ���
	_tprintf(_T("Real zip file count : %d\n"), zipcnt);//��¥ zip���� ���� ���
	/*
	char cmp[3];//�ñ״�ó ��
	memset(cmp, 0, sizeof(cmp));//�ñ״�ó ���� ���� �ʱ�ȭ
	strcpy_s(cmp, "PK");//�ñ״�ó pk ����

	for (int i = 0; i < cnt; i++)
	{
		ReadFile(File[i], signature, sizeof(char) * 2, &result, NULL);//���� 2����Ʈ��ŭ �о�ͼ� signature�� ����
		printf("signature : %s\n", signature);
		_tprintf(_T("read byte : %d\n"), result);

		printf("%d\n", strlen(signature));


		printf("%d\n", strlen(cmp));
	}*/

	CloseHandle(hFind);
	CloseHandle(hFile);

	system("pause");
	return 0;
}