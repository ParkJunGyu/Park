/*
	같은 디렉토리 안에서의 모든 zip파일을 읽어온 후
	처음 2byte를 확인하여 zip파일인지 아닌지의 유무를 확인하는 프로그램
*/

#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <string.h>

DWORD zipcnt = 0;//진짜 zip파일의 개수

 TCHAR* SignatureCmp(HANDLE handle) {
	char signature[3];//시그니처 저장장소
	memset(signature, 0, sizeof(signature));//시그니처 저장소 초기화
	DWORD result;//읽은 바이트 수

	char cmp[3];//시그니처 비교
	memset(cmp, 0, sizeof(cmp));//시그니처 비교할 변수 초기화
	strcpy_s(cmp, "PK");//시그니처 pk 저장

	if (ReadFile(handle, signature, sizeof(char) * 2, &result, NULL) == 0)//파일 2바이트만큼 읽어와서 signature에 저장
	{
		_tprintf(_T("ReadFile error. Error is %u\n"), GetLastError());
		exit(1);
	}

	if (strcmp(signature, cmp) == 0)//시그니처 비교 부분
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
	HANDLE hFind;//FirstFile 핸들 저장
	//HANDLE File[1000];//찾은 zip파일 핸들 저장
	HANDLE hFile;//zip파일 핸들 저장
	TCHAR Path[MAX_PATH];//작업디렉토리 저장할 곳
	WIN32_FIND_DATA wfd;//찾은 파일, 서브디렉토리 정보 담기
	TCHAR filename[MAX_PATH];//파일이름 저장할 버퍼
	DWORD dwError;//에러메세지 저장
	DWORD cnt=0;//파일 개수 저장

	/*
	char signature[3];//시그니처 저장장소
	memset(signature, 0, sizeof(signature));//시그니처 저장소 초기화
	DWORD result;//읽은 바이트 수
	*/

	//현재 작업 디렉토리 얻어오기
	GetCurrentDirectory(MAX_PATH, Path);
	//작업 디렉토리 끝에 \*.zip 문자열 붙이기
	_tcsncat_s(Path, _T("\\*.zip"), 7);
	//현재 작업 디렉토리 출력
	_tprintf(_T("Current Directory : %s\n"), Path);
	
	hFind = FindFirstFile(Path,&wfd);//제일 처음 파일 핸들 얻기
	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("Invalid file handle. Error is %u\n", GetLastError());
		return -1;
	}
	else
	{
		_tcsncpy_s(filename, wfd.cFileName, 260);//파일 이름 저장
		hFile = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			printf("Invalid file handle. Error is %u\n", GetLastError());
			return -1;
		}
		_tprintf(_T("file name : %s\t%s\n"), filename, SignatureCmp(hFile));//파일 이름 출력
		//File[cnt] = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);//파일 핸들 얻어오기
		

		cnt++;//파일 개수 1 증가

		while (FindNextFile(hFind, &wfd) != 0)//다음 파일 찾기
		{
			_tcsncpy_s(filename, wfd.cFileName, 260);//파일 이름 저장
			hFile = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
			_tprintf(_T("file name : %s\t%s\n"), filename, SignatureCmp(hFile));//파일 이름 출력
			if (hFile == INVALID_HANDLE_VALUE)
			{
				printf("Invalid file handle. Error is %u\n", GetLastError());
				return -1;
			}
			//File[cnt] = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);//파일 핸들 얻어오기
			cnt++;//파일 개수 1 증가
		}
		dwError = GetLastError();//에러메세지 저장
		FindClose(hFind);//찾기 그만
		if (dwError != ERROR_NO_MORE_FILES)//더이상 파일 없어서 끝난게 아닐때
		{
			_tprintf(_T("FindNextFile error. Error is %u\n"), dwError);
			return -1;
		}
	}
	
	_tprintf(_T("Total zip file count : %d\n"), cnt);//총 zip파일 개수 출력
	_tprintf(_T("Real zip file count : %d\n"), zipcnt);//진짜 zip파일 개수 출력
	/*
	char cmp[3];//시그니처 비교
	memset(cmp, 0, sizeof(cmp));//시그니처 비교할 변수 초기화
	strcpy_s(cmp, "PK");//시그니처 pk 저장

	for (int i = 0; i < cnt; i++)
	{
		ReadFile(File[i], signature, sizeof(char) * 2, &result, NULL);//파일 2바이트만큼 읽어와서 signature에 저장
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