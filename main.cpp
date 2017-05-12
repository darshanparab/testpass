#include <iostream>
#include <windows.h>
#include <wininet.h>
using namespace std;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
HANDLE hFile;
HINTERNET HINETOPEN, HINETCONNECT;
TCHAR uAgent [20]="CheckAdminPass";
DWORD AccessType = INTERNET_OPEN_TYPE_DIRECT;
TCHAR ftpServer[20] = "ftpserver";
TCHAR ftpUname[20]="ftpuser", ftpPass[30]="ftppass";
DWORD inetService = INTERNET_SERVICE_FTP;
BOOL dfwpasspresent=FALSE;

int main(int argc, char** argv)
{
	wchar_t username[20]=L"Administrator";
	wchar_t password[20]=L"p@ssw0rd";
	wchar_t resultfile[256];
	PROCESS_INFORMATION procinfo;
	wchar_t domain[20];
	DWORD size=20;
	FILE *fp=NULL;
	
	if(GetComputerNameW(domain,&size))
	{
		if(CreateProcessWithLogonW(username,domain,password,0,L"C:\\Windows\\System32\\ping.exe",NULL,CREATE_DEFAULT_ERROR_MODE,NULL,L"C:\\Windows\\System32",NULL,&procinfo))
		{
			dfwpasspresent=TRUE;
			GetCurrentDirectoryW(256,resultfile);
			wcscat(resultfile,L"\\");
			wcscat(resultfile,domain);
			fp=_wfopen(resultfile,L"w");
			fputws(L"",fp);
			fclose(fp);
	
	/*------------------------------------------ Open network connection ------------------------------------------*/
			HINETOPEN = InternetOpen(uAgent,AccessType,NULL,NULL,INTERNET_FLAG_ASYNC);
		
	/*------------------------------------------ Connect to Remote FTP service -----------------------------------------*/
			HINETCONNECT = InternetConnect(HINETOPEN,ftpServer,21,ftpUname,ftpPass,inetService,INTERNET_FLAG_PASSIVE,0);
	
	/*------------------------------------------ Upload file to FTP server ------------------------------------------*/
			FtpPutFileW(HINETCONNECT,resultfile,domain,FTP_TRANSFER_TYPE_BINARY,0);
		}
	}
	
	return 0;
}
