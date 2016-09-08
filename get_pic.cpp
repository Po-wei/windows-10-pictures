#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <windows.h>
#include <Lmcons.h>
#include <shellapi.h>
#include <tchar.h>

#pragma comment(lib, "shell32.lib")
using namespace std;

int main()
{
	//get local username in windows 
	char username[UNLEN+1];
	DWORD username_len = UNLEN+1;
	GetUserName(username, &username_len);
	string UserName(username);
	
	vector<string> files; 
	DIR *pic_dir; //picture directory
	DIR *dest_dir; // destination directory
	struct dirent *dirp;
	
	string source = "C:\\Users\\"+UserName+"\\AppData\\Local\\Packages\\Microsoft.Windows.ContentDeliveryManager_cw5n1h2txyewy\\LocalState\\Assets";
	string dest = "C:\\Users\\"+UserName+"\\Desktop\\picture";
	mkdir(dest.c_str());
	
	if((pic_dir = opendir(source.c_str())) == NULL )
	{
		cout << "Cannot find pictures in Windows Spotlight." << endl ;
		cout << "Error(" << errno << ") opening " << pic_dir << endl;
	}
	if((dest_dir = opendir(dest.c_str())) == NULL )
	{
		cout << "Error(" << errno << ") opening " << dest_dir << endl;
	}
	
	//copy picture from sys to dest 	
	string to = dest+"\0" ;
	string from = source+"\\*\0" ;
	
	SHFILEOPSTRUCT fop;
	ZeroMemory(&fop, sizeof(fop));
	fop.wFunc = FO_COPY;
	fop.pTo = _T(to.c_str());
	fop.pFrom = _T(from.c_str());
	SHFileOperation(&fop);
	
	while((dirp = readdir(dest_dir)) != NULL )
	{
		files.push_back(string(dirp->d_name));
	}
	closedir(pic_dir);
	closedir(dest_dir);
	for(int i = 0; i < files.size() ; i++){
//		cout << files[i] << endl ;
		string oldname = dest + "\\" + files[i] ;
		string temp = dest + "\\" + to_string(i) + ".jpg";
		cout << temp <<endl ;
		const char* newname = temp.c_str(); 
		rename(oldname.c_str(),newname);       	
    }
    
    cout << "\n已在桌面建立資料夾picture" << endl ;
    cout << "複製完成......" << endl; 
    cout << "\n要重新執行前，先刪除picture 資料夾。" << endl; 
    cout << "按任意建結束......" << endl ; 
    cin.get();
}
