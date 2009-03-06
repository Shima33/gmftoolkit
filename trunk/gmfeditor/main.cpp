#include <windows.h>
#include <commctrl.h>
#include <string.h>
#include <stdio.h>

#include "cGMF.h"
#include "main.h"
#include "populate.h"


cGMF* gmf;
char* buf;

char *StrToC(string str)
{
        strcpy(buf, str.c_str());
        return buf;
}


TV_INSERTSTRUCT tvi;
HWND hwndTV;

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
    wincl.hIcon = LoadIcon(hThisInstance, MAKEINTRESOURCE(ICON_MAIN));
    wincl.hIconSm = LoadIcon(hThisInstance, MAKEINTRESOURCE(ICON_MAIN));
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

void populateTreeView()
{	
	HTREEITEM Parent;
	HTREEITEM Root;
	HTREEITEM Materials;
	HTREEITEM Textures;
	HTREEITEM Objects;
	HTREEITEM Temp;
	
	tvi.hParent = NULL;
	tvi.hInsertAfter = TVI_ROOT;
	tvi.item.mask = TVIF_TEXT | TVIF_PARAM; 

	tvi.item.pszText = StrToC(gmf->scene->name);
	Root = Add(); 
	
	tvi.hInsertAfter=TVI_LAST;
	
	tvi.hParent = Root;
	Parent = AddLabel("Scene", gmf->scene, 0);
	
	int i;
	char* itoabuf = (char*)malloc(sizeof(char)*6);
	tvi.hParent = Parent;
	
	AddString("Name", &gmf->scene->name);	
	AddInt("First Frame", (int*)&gmf->scene->firstFrame);	
	AddInt("Last Frame", (int*)&gmf->scene->lastFrame);	
	AddInt("Frame Speed", (int*)&gmf->scene->frameSpeed);	
	AddInt("Ticks per Frame", (int*)&gmf->scene->ticksPerFrame);
	AddRGB("Background Static", &gmf->scene->backgroundStatic);
	AddRGB("Ambient Static", &gmf->scene->ambientStatic);
	
	tvi.hParent = Root;
	Materials = AddLabel("Materials", gmf->materialList, 0);
	ReadMaterials(Materials, gmf->materialList);
	
	tvi.hParent = Root;
	Objects = AddLabel("Objects", gmf->objectList, 0);
	tvi.hParent = Objects;
	HTREEITEM Geometry = AddLabel("Geometry Objects", NULL, 0);
	HTREEITEM Havok = AddLabel("Havok Objects", NULL, 0);
	HTREEITEM Attachment = AddLabel("Attachment Points", NULL, 0);
	HTREEITEM Unhandled = AddLabel("Unhandled Objects", NULL, 0);
	for (i = 0; i < gmf->objectList->number; i++)
	{
		tvi.hParent = Objects;
		switch(gmf->objectList->objects[i]->getType())
		{
			/*case -1:
				tvi.hParent = Unhandled;
				AddLabel("UNHANDLED OBJECT", NULL, 0);
				break;
			case GEOMOBJECT:
				tvi.hParent = Geometry;
				tvi.hParent = AddLabel(StrToC(gmf->objectList->objects[i]->name), &gmf->objectList->objects[i]->name, 0);						
				AddString("Name", &gmf->objectList->objects[i]->name);
				AddString("Parent", &((cGMFObjectGeo*)gmf->objectList->objects[i])->parent);
				AddInt("Shade Verts", &((cGMFObjectGeo*)gmf->objectList->objects[i])->shadeVerts);
				
				ReadTM(gmf->objectList->objects[i]->tm);
				
				tvi.hParent = AddLabel("Mesh", &((cGMFObjectGeo*)gmf->objectList->objects[i])->mesh, 0);
				ReadMesh(((cGMFObjectGeo*)gmf->objectList->objects[i])->mesh);
				break;
				
				
			case GMID_HAVOK_SIMOBJECT:
				tvi.hParent = Havok;
				tvi.hParent = AddString("Simulation Object", &gmf->objectList->objects[i]->name);						
				AddString("Name", &gmf->objectList->objects[i]->name);
				break;
				
				
			case GMID_ATTACHMENTPT:
				tvi.hParent = Attachment;
				tvi.hParent = AddLabel(StrToC(gmf->objectList->objects[i]->name), &gmf->objectList->objects[i]->name, 0);						
				AddString("Name", &gmf->objectList->objects[i]->name);
				
				ReadTM(gmf->objectList->objects[i]->tm);
				
				AddString("User Data", &((cGMFObjectAttachment*)gmf->objectList->objects[i])->userData);
				break;
				
				
			case GMID_HAVOK_RBCOLLECTION:
				tvi.hParent = Havok;
				tvi.hParent = AddString("Rigid Body Collection", &gmf->objectList->objects[i]->name);						
				AddString("Name", &gmf->objectList->objects[i]->name);
				break;*/
		}
		
	} 
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	TVITEM item;
	HTREEITEM Selected;
	int* pointer;
	HWND hEdit;
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
			hwndTV = CreateWindowEx(	0, 
										WC_TREEVIEW, 
										"lol",  
										WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_EDITLABELS, 
										0, 
										0, 
										643, 
										483, 
										hwnd, 
										(HMENU)500, 
										hInst, 
										NULL
										);
			populateTreeView();
			break;
			
		case WM_NOTIFY:
			itoa(LOWORD(wParam), buf, 10);
			switch (LOWORD(wParam))
			{
				case 500:
					if(((LPNMHDR)lParam)->code == NM_RCLICK)
					{
						Selected=(HTREEITEM)SendMessage(hwndTV,TVM_GETNEXTITEM,TVGN_DROPHILITE,0);
						SendMessage(hwndTV,TVM_SELECTITEM,TVGN_CARET,(LPARAM)Selected);
						TreeView_EditLabel(hwndTV, Selected);
					}
					if(((LPNMHDR)lParam)->code == TVN_BEGINLABELEDIT)
					{
						memset(&item,0,sizeof(item));
						Selected = (HTREEITEM)SendMessage(hwndTV, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)Selected);
						
						hEdit = TreeView_GetEditControl(hwndTV);
						
						item.mask=TVIF_PARAM;
						item.hItem=Selected;
						
						if(SendMessage(hwndTV, TVM_GETITEM, TVGN_CARET, (LPARAM)&item))
						{
							itoa(((Uni*)item.lParam)->type, buf, 10);
							switch(((Uni*)item.lParam)->type)
							{
								case 1: //int
									sprintf(buf, "%i", *(int*)((Uni*)item.lParam)->object);
									SetWindowText(hEdit, buf);
									break;
								case 2: //float
									sprintf(buf, "%f", *(float*)((Uni*)item.lParam)->object);
									SetWindowText(hEdit, buf);
									break;
								case 3: //string	
									SetWindowText(hEdit, StrToC(*(string*)((Uni*)item.lParam)->object));
									break;
								case 4: //RGB
									SetWindowText(hEdit, StrToC("#" + (string)RGBToC(*(RGB*)((Uni*)item.lParam)->object)));
									break;
							}
						}
						
					}
					if(((LPNMHDR)lParam)->code == TVN_ENDLABELEDIT)
					{
						memset(&item,0,sizeof(item));
						Selected = (HTREEITEM)SendMessage(hwndTV, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)Selected);
						
						item.mask=TVIF_TEXT;
						item.hItem=Selected;
						
						hEdit = TreeView_GetEditControl(hwndTV);
						
						if(SendMessage(hwndTV, TVM_GETITEM, TVGN_CARET, (LPARAM)&item))
						{
							itoa(((Uni*)item.lParam)->type, buf, 10);
							switch(((Uni*)item.lParam)->type)
							{
								case 1: //int
									GetWindowText(hEdit, buf, 10);
									*(int*)((Uni*)item.lParam)->object = atoi(buf);
									item.pszText = StrToC(((Uni*)item.lParam)->label + ": " + buf);
									SendMessage(hwndTV,TVM_SETITEM,0,(WPARAM)&item);
									break;
								case 2: //float
									GetWindowText(hEdit, buf, 10);
									*(float*)((Uni*)item.lParam)->object = atof(buf);
									sprintf(buf, "%f", atof(buf));
									item.pszText = StrToC(((Uni*)item.lParam)->label + ": " + buf);
									SendMessage(hwndTV,TVM_SETITEM,0,(WPARAM)&item);
									break;
								case 3: //string	
									GetWindowText(hEdit, buf, 10);
									*(string*)((Uni*)item.lParam)->object = buf;
									item.pszText = StrToC(((Uni*)item.lParam)->label + ": " + buf);
									SendMessage(hwndTV,TVM_SETITEM,0,(WPARAM)&item);
									break;
								case 4: //RGB
									SetWindowText(hEdit, StrToC("#" + (string)RGBToC(*(RGB*)((Uni*)item.lParam)->object)));
									break;
							}
						}
						
					}
					break;
			}
			break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
