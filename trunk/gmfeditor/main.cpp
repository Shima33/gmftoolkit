#include <windows.h>
#include <commctrl.h>
#include <string.h>
#include <stdio.h>

#include "cGMF.h"

cGMF* gmf;
char* buf;

char *StrToC(string str)
{
	strcpy(buf, str.c_str());
	return buf;
}

char *RGBToC(RGB color)
{
	char* ret = (char*)malloc(sizeof(char)*20);
	sprintf(ret, "%02X%02X%02X", color.red, color.green, color.blue);
	return ret;
}

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
char szClassName[ ] = "WindowsApp";

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "GMF Explorer (c) 2008 Serge Bazanski",       /* Title Text */
           WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           648,                 /* The programs width */
           514,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nFunsterStil);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

void populateTreeView(HWND hwndTV)
{
	TV_INSERTSTRUCT tvi;
	
	HTREEITEM Parent;
	HTREEITEM Root;
	HTREEITEM Materials;
	HTREEITEM Temp;
	
	tvi.hParent = NULL;
	tvi.hInsertAfter = TVI_ROOT;
	tvi.item.mask = TVIF_TEXT | TVIF_PARAM; 

	tvi.item.pszText = StrToC(gmf->scene->name);
	Root = (HTREEITEM)SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi); 
	
	tvi.hInsertAfter=TVI_LAST;
	
	tvi.hParent = Root;
	tvi.item.pszText = "Scene";
	Parent = (HTREEITEM)SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi); 
	
	int i;
	char* itoabuf = (char*)malloc(sizeof(char)*6);
	tvi.hParent = Parent;
	
	tvi.item.pszText = StrToC("Name: " + gmf->scene->name);
	SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi); 
	
	itoa(gmf->scene->firstFrame, itoabuf, 10);
	tvi.item.pszText = StrToC("First Frame: " + (string)itoabuf);
	SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi); 
	
	itoa(gmf->scene->lastFrame, itoabuf, 10);
	tvi.item.pszText = StrToC("Last Frame: " + (string)itoabuf);
	SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi); 
	
	itoa(gmf->scene->frameSpeed, itoabuf, 10);
	tvi.item.pszText = StrToC("Frame Speed: " + (string)itoabuf);
	SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi); 
	
	itoa(gmf->scene->ticksPerFrame, itoabuf, 10);
	tvi.item.pszText = StrToC("Ticks per Frame: " + (string)itoabuf);
	SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi); 
	
	tvi.item.pszText = StrToC("Background Static: " + (string)RGBToC(gmf->scene->backgroundStatic));
	SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi); 
	
	tvi.item.pszText = StrToC("Ambient Static: " + (string)RGBToC(gmf->scene->ambientStatic));
	SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi); 
	
	
	tvi.hParent = Root;
	tvi.item.pszText = "Materials";
	Materials = (HTREEITEM)SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi); 
	
	
	for (i = 0; i < gmf->materialList->number; i++)
	{
		tvi.hParent = Materials;		
		itoa(i, itoabuf, 10);
		tvi.item.pszText = StrToC((string)itoabuf + " (" + gmf->materialList->materials[i]->name + ")");
		Parent = (HTREEITEM)SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi); 
		
		tvi.hParent = Parent;
		
		itoa(gmf->materialList->materials[i]->ref, itoabuf, 10);
		tvi.item.pszText = StrToC("Ref No: " + (string)itoabuf);
		SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi); 
		
		tvi.item.pszText = StrToC("Name: " + gmf->materialList->materials[i]->name);
		SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi); 
		
		tvi.item.pszText = StrToC("Class: " + gmf->materialList->materials[i]->classname);
		SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi); 
		
		tvi.item.pszText = StrToC("Ambient: " + (string)RGBToC(gmf->materialList->materials[i]->ambient));
		SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi); 
		tvi.item.pszText = StrToC("Diffuse: " + (string)RGBToC(gmf->materialList->materials[i]->diffuse));
		SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi); 
		tvi.item.pszText = StrToC("Specular: " + (string)RGBToC(gmf->materialList->materials[i]->specular));
		SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi); 
		
		sprintf(itoabuf, "%f", gmf->materialList->materials[i]->shine);
		tvi.item.pszText = StrToC("Shine: " + (string)itoabuf);
		SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi);
		sprintf(itoabuf, "%f", gmf->materialList->materials[i]->shineStrength);
		tvi.item.pszText = StrToC("Shine Strength: " + (string)itoabuf);
		SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi); 
		sprintf(itoabuf, "%f", gmf->materialList->materials[i]->transparency);
		tvi.item.pszText = StrToC("Transparency: " + (string)itoabuf);
		SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi); 
		sprintf(itoabuf, "%f", gmf->materialList->materials[i]->wiresize);
		tvi.item.pszText = StrToC("Wire Size: " + (string)itoabuf);
		SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi); 
		
		if(gmf->materialList->materials[i]->texNum > 0)
		{
			itoa(gmf->materialList->materials[i]->texNum, itoabuf, 10);
			tvi.item.pszText = StrToC((string)itoabuf + (string)" texture(s)");
			tvi.hParent = (HTREEITEM)SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi);
			int j;
			for (j = 0; j < gmf->materialList->materials[i]->texNum; j++)
			{
				tvi.item.pszText = StrToC("Name: " + gmf->materialList->materials[i]->textureList->textures[j]->name);
				SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi);
				tvi.item.pszText = StrToC("Class: " + gmf->materialList->materials[i]->textureList->textures[j]->classname);
				SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi);
				tvi.item.pszText = StrToC("Bitmap: " + gmf->materialList->materials[i]->textureList->textures[j]->bitmap);
				SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi);
				
				sprintf(itoabuf, "%f", gmf->materialList->materials[i]->textureList->textures[j]->amount);
				tvi.item.pszText = StrToC("Amount: " + (string)itoabuf);
				SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi);
				
				itoa(gmf->materialList->materials[i]->textureList->textures[j]->style, itoabuf, 10);
				if (gmf->materialList->materials[i]->textureList->textures[j]->style == 1)
					tvi.item.pszText = StrToC("Style: " + (string)itoabuf + " (diffuse)");
				else if (gmf->materialList->materials[i]->textureList->textures[j]->style == 5)
					tvi.item.pszText = StrToC("Style: " + (string)itoabuf + " (selfillum)");
				else if (gmf->materialList->materials[i]->textureList->textures[j]->style == 6)
					tvi.item.pszText = StrToC("Style: " + (string)itoabuf + " (opacity)");
				else
					tvi.item.pszText = StrToC("Style: " + (string)itoabuf + " (diffuse)");
					
				SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi);
					
				itoa(gmf->materialList->materials[i]->textureList->textures[j]->type, itoabuf, 10);
				if (gmf->materialList->materials[i]->textureList->textures[j]->type == 4)
					tvi.item.pszText = StrToC("Type: " + (string)itoabuf + " (screen)");
				else
					tvi.item.pszText = StrToC("Type: " + (string)itoabuf + " (unknown)");
				
				SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi);
				
				sprintf(itoabuf, "%f", gmf->materialList->materials[i]->textureList->textures[j]->UO);
				tvi.item.pszText = StrToC("UVW U Offset: " + (string)itoabuf );
				SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi);
				
				sprintf(itoabuf, "%f", gmf->materialList->materials[i]->textureList->textures[j]->VO);
				tvi.item.pszText = StrToC("UVW V Offset: " + (string)itoabuf );
				SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi);
				
				sprintf(itoabuf, "%f", gmf->materialList->materials[i]->textureList->textures[j]->UT);
				tvi.item.pszText = StrToC("UVW U Tiling: " + (string)itoabuf );
				SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi);
				
				sprintf(itoabuf, "%f", gmf->materialList->materials[i]->textureList->textures[j]->VT);
				tvi.item.pszText = StrToC("UVW V Tiling: " + (string)itoabuf );
				SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi);
				
				sprintf(itoabuf, "%f", gmf->materialList->materials[i]->textureList->textures[j]->UVWAngle);
				tvi.item.pszText = StrToC("UVW Angle: " + (string)itoabuf );
				SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi);
				
				sprintf(itoabuf, "%f", gmf->materialList->materials[i]->textureList->textures[j]->UVWBlur);
				tvi.item.pszText = StrToC("UVW Blur: " + (string)itoabuf );
				SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi);
				
				sprintf(itoabuf, "%f", gmf->materialList->materials[i]->textureList->textures[j]->UVWBlurOffset);
				tvi.item.pszText = StrToC("UVW Blur Offset: " + (string)itoabuf );
				SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi);
				
				sprintf(itoabuf, "%f", gmf->materialList->materials[i]->textureList->textures[j]->UVWNoiseAmount);
				tvi.item.pszText = StrToC("UVW Noise Amount: " + (string)itoabuf );
				SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi);
				
				sprintf(itoabuf, "%f", gmf->materialList->materials[i]->textureList->textures[j]->UVWNoiseSize);
				tvi.item.pszText = StrToC("UVW Noise Size: " + (string)itoabuf );
				SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi);
				
				itoa(gmf->materialList->materials[i]->textureList->textures[j]->UVWNoiseLevel, itoabuf, 10);
				tvi.item.pszText = StrToC("UVW Noise Level: " + (string)itoabuf );
				SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi);
				
				sprintf(itoabuf, "%f", gmf->materialList->materials[i]->textureList->textures[j]->UVWNoisePhase);
				tvi.item.pszText = StrToC("UVW Noise Phase: " + (string)itoabuf );
				SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi);
				
				itoa(gmf->materialList->materials[i]->textureList->textures[j]->bitmapFilter, itoabuf, 10);
				if (gmf->materialList->materials[i]->textureList->textures[j]->bitmapFilter == 0)
					tvi.item.pszText = StrToC("Bitmap Filter: " + (string)itoabuf + " (pyramidal)");
				else
					tvi.item.pszText = StrToC("Bitmap Filter: " + (string)itoabuf + " (SAT)");
				
				SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi);
					
				itoa(gmf->materialList->materials[i]->textureList->textures[j]->bitmapChannel, itoabuf, 10);
				tvi.item.pszText = StrToC("Bitmap Channel: " + (string)itoabuf );
				SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvi);
				
			}	
		} 
	}	
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
		case WM_INITDIALOG:
			InitCommonControls();
			break;
		case WM_CREATE:
			buf = (char*)malloc(sizeof(char)*1024);
			try {
				gmf = new cGMF("in.gmf", 0, 0);
			}
			catch(char* exception)
			{
				MessageBoxA(NULL, exception, "An error has occured!", MB_OK | MB_ICONERROR );
			}
			static HINSTANCE hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
			HWND hwndtv;
			hwndtv = CreateWindowEx(	0, 
										WC_TREEVIEW, 
										"lol",  
										WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS, 
										0, 
										0, 
										643, 
										483, 
										hwnd, 
										(HMENU)500, 
										hInst, 
										NULL
										);
			populateTreeView(hwndtv);
			break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
