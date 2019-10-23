#define ALLEGRO_STATICLINK

// #define USE_CONSOLE 


#include <allegro.h>
#include "allegro/internal/aintern.h"
#include <winalleg.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream>

using namespace std;
using std::string;
using std::vector;

bool endp = false;
int BPP = 32;
int WIDTH;
int HEIGHT;
int WINDOWED_WIDTH = -1;
int WINDOWED_HEIGHT = -1;
int FULLSCREEN_WIDTH;
int FULLSCREEN_HEIGHT;
int SCREEN_MODE = -1; // 1 FULLSCREEN, 0 WINDOWED

int offsetmenu = 0;

vector<vector<string>> menus;
string header;

void my_exit(void)
{
	 endp = true;	 
}

std::wstring s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

string GetExePath() {
    TCHAR NPath[MAX_PATH];
	GetModuleFileName(NULL, NPath, MAX_PATH);
	//GetCurrentDirectory(MAX_PATH, NPath);
	wstring string_string (NPath);
	string simpleString;
	simpleString.assign(string_string.begin(), string_string.end());
	simpleString = simpleString.substr(0, simpleString.find_last_of( "\\/" ));	
	return simpleString;
}

string padTo(const string& s, const size_t num, const char paddingChar = ' ')
{
	string result = s;	
    if(num > result.size())
    	result.insert(result.end(), num - result.size(), paddingChar);

	return result;
}

 vector<string> split(const string& s, const string& delim, const bool keep_empty = true) {
    vector<string> result;
    if (delim.empty()) {
        result.push_back(s);
        return result;
    }
    string::const_iterator substart = s.begin(), subend;
    while (true) {
        subend = search(substart, s.end(), delim.begin(), delim.end());
        string temp(substart, subend);
        if (keep_empty || !temp.empty()) {
            result.push_back(temp);
        }
        if (subend == s.end()) {
            break;
        }
        substart = subend + delim.size();
    }
    return result;
}

 void load_ini()
{
    string Encabezado;
    string STRING;
	vector<string> auxlist;    
    ifstream infline;

	infline.open(GetExePath().append("\\ZXLauncher.ini").c_str()); 
 
	if (infline.is_open()) {
		while (!infline.eof()) {
		
			getline(infline,STRING);
			Encabezado = STRING;
			
		    if (Encabezado == "[header]")
            {
				getline(infline,STRING);
				if (STRING.length() > 30)
				{
					STRING = STRING.substr(0,27) + "...";
				}
                header = STRING;
            }

			if (Encabezado == "[screen_mode]")
            {
				getline(infline,STRING);
				SCREEN_MODE = atoi(STRING.c_str());
            }

			if (Encabezado == "[windowed_resolution]")
            {
				getline(infline,STRING);
                WINDOWED_WIDTH = atoi(STRING.c_str());
				getline(infline,STRING);
                WINDOWED_HEIGHT = atoi(STRING.c_str());
            }

			if (Encabezado == "[menu]")
            {				
				auxlist.clear();
				getline(infline,STRING);
				if (STRING.length() > 8)
				{
					STRING = STRING.substr(0,5) + "...";
				}
				auxlist.push_back(STRING);
				menus.push_back(auxlist);
				auxlist.clear();
				getline(infline,STRING);
				while (STRING != "")
                {
					if (split(STRING, ";")[0].length() > 13)					
					{
						STRING = split(STRING, ";")[0].substr(0,10) + "...;" + split(STRING, ";")[1] + ";" + split(STRING, ";")[2] + ";" + split(STRING, ";")[3] + ";" + split(STRING, ";")[4];						
					}
					if (split(STRING, ";")[1].length() > 25)					
					{
						STRING = split(STRING, ";")[0] + ";" + split(STRING, ";")[1].substr(0,22) + "...;" + split(STRING, ";")[2] + ";" + split(STRING, ";")[3] + ";" + split(STRING, ";")[4];
					}
					auxlist.push_back(STRING);
					getline(infline,STRING);
                }
				menus.push_back(auxlist);

            }
		}
	}
	infline.close();
	    
}

int print_menu(int menuindex, int pos, BITMAP *mapscreen)
{
	textout_ex(mapscreen, font, padTo(split(menus[menuindex * 2][0], ";")[0],8).c_str(), 88, 80, makecol(255, 255, 255),
            makecol(0, 0, 0));
            
    if (pos < 0)
    {
		pos = menus[menuindex * 2 + 1].size() - 1;
		offsetmenu = pos - 4;		
		offsetmenu = offsetmenu < 0 ? 0 : offsetmenu;
		
    }
	else if (pos > menus[menuindex * 2 + 1].size() - 1)
    {
		pos = 0;
		offsetmenu = 0;
    }

    if (pos > offsetmenu + 4)
    {
        offsetmenu++;
    }

    if (pos < offsetmenu)
    {
        offsetmenu--;
    }

	for (int i = offsetmenu; i < (menus[menuindex * 2 + 1].size() < 5 ? menus[menuindex * 2 + 1].size() : offsetmenu + 5); i++)
    {
        if (i == pos)
        {
                    
            textout_ex(mapscreen, font, padTo(split(menus[menuindex * 2 + 1][i], ";")[0],13).c_str(), 88, 88 + (i - offsetmenu) * 8, makecol(0, 0, 0),
                makecol(0, 255, 255));

                    
            for (int i2 = 0; i2 < 8; i2++)
            {
                line(mapscreen, 192, i2 + 88 + (i - offsetmenu) * 8, 197, i2 + 88 + (i - offsetmenu) * 8, makecol(0, 255, 255));
            }
                    

            textout_ex(mapscreen, font, padTo(split(menus[menuindex * 2 + 1][i], ";")[1],25).c_str(), 32, 192, makecol(255, 255, 255),
                makecol(0, 0, 0));
        }
        else
        {
            textout_ex(mapscreen, font, padTo(split(menus[menuindex * 2 + 1][i], ";")[0],13).c_str(), 88, 88 + (i - offsetmenu) * 8, makecol(0, 0, 0),
                makecol(255, 255, 255));

                    
            for (int i2 = 0; i2 < 8; i2++)
            {
                line(mapscreen, 192, i2 + 88 + (i - offsetmenu) * 8, 197, i2 + 88 + (i - offsetmenu) * 8, makecol(255, 255, 255));
            }
                    
        }
    }

	if (menus[menuindex * 2 + 1].size() < 5)
    {
		for (int i2 = 0; i2 < (5 - menus[menuindex * 2 + 1].size()) * 8; i2++)
        {
			line(mapscreen, 88, 88 + i2 + (menus[menuindex * 2 + 1].size() * 8), 197, 88 + i2 + (menus[menuindex * 2 + 1].size() * 8), makecol(255, 255, 255));
        }
    }

	if (offsetmenu + 5 < menus[menuindex * 2 + 1].size())
    {
        for (int i2 = 3; i2 < 6; i2++)
        {
            line(mapscreen, 88 + i2 - 1, i2 + 88 + (8 * 4), 96 - i2 + 1, i2 + 88 + (8 * 4), makecol(0, 0, 0));
        }
                                
    }
    if (offsetmenu > 0)
    {

        for (int i2 = 3; i2 < 6; i2++)
        {
            line(mapscreen, 92 - i2 + 3, i2 + 88, 92 + i2 - 3, i2 + 88, makecol(0, 0, 0));
        }
                                
    }
            
	stretch_blit(mapscreen, screen, 0, 0, mapscreen->w, mapscreen->h, 0, 0, WIDTH, HEIGHT);
    return pos;
}

size_t ExecuteProcess(std::wstring FullPathToExe, std::wstring Parameters, size_t SecondsToWait) 
{ 
    size_t iMyCounter = 0, iReturnVal = 0, iPos = 0; 
    DWORD dwExitCode = 0; 
    std::wstring sTempStr = L""; 

    /* - NOTE - You should check here to see if the exe even exists */ 

    /* Add a space to the beginning of the Parameters */ 
    if (Parameters.size() != 0) 
    { 
        if (Parameters[0] != L' ') 
        { 
            Parameters.insert(0,L" "); 
        } 
    } 

    /* The first parameter needs to be the exe itself */ 
    sTempStr = FullPathToExe; 
    iPos = sTempStr.find_last_of(L"\\"); 
    sTempStr.erase(0, iPos +1); 
    Parameters = sTempStr.append(Parameters); 

     /* CreateProcessW can modify Parameters thus we allocate needed memory */ 
    wchar_t * pwszParam = new wchar_t[Parameters.size() + 1]; 
    if (pwszParam == 0) 
    { 
        return 1; 
    } 
    const wchar_t* pchrTemp = Parameters.c_str(); 
    wcscpy_s(pwszParam, Parameters.size() + 1, pchrTemp); 

    /* CreateProcess API initialization */ 
    STARTUPINFOW siStartupInfo; 
    PROCESS_INFORMATION piProcessInfo; 
    memset(&siStartupInfo, 0, sizeof(siStartupInfo)); 
    memset(&piProcessInfo, 0, sizeof(piProcessInfo)); 
    siStartupInfo.cb = sizeof(siStartupInfo); 

    if (CreateProcessW(const_cast<LPCWSTR>(FullPathToExe.c_str()), 
                            pwszParam, 0, 0, false, 
                            CREATE_DEFAULT_ERROR_MODE, 0, 0, 
                            &siStartupInfo, &piProcessInfo) != false) 
    { 
         /* Watch the process. */ 
        dwExitCode = WaitForSingleObject(piProcessInfo.hProcess, (SecondsToWait * 1000)); 
    } 
    else 
    { 
        /* CreateProcess failed */ 
        iReturnVal = GetLastError(); 
    } 

    /* Free memory */ 
    delete[]pwszParam; 
    pwszParam = 0; 

    /* Release handles */ 
    CloseHandle(piProcessInfo.hProcess); 
    CloseHandle(piProcessInfo.hThread); 

    return iReturnVal; 
} 

 int __stdcall WinMain(void *hInst, void *hPrev, char *Cmd, int nShow)
	 

{

	int menuindex = 0;
    int posmenu = 0;
		
	BITMAP *mapscreen;
	BITMAP *bmpfont;
    PALETTE palette;	
	
	load_ini();
	
	allegro_init();		
	install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL); 
	
	set_volume(255,0);
	SAMPLE *beep_sample;
    
	set_window_title("ZXLauncher");

    set_color_depth(BPP);    
	if ((BPP = desktop_color_depth()) != 0) {
		set_color_depth(BPP);
    }

	SCREEN_MODE = SCREEN_MODE == -1 ? 1 : SCREEN_MODE;
	WINDOWED_WIDTH = WINDOWED_WIDTH == -1 ? 640 : WINDOWED_WIDTH;
	WINDOWED_HEIGHT = WINDOWED_HEIGHT == -1 ? 480 : WINDOWED_HEIGHT;
		
	get_desktop_resolution(&FULLSCREEN_WIDTH, &FULLSCREEN_HEIGHT);
	

	if (SCREEN_MODE == 1)
	{		
		WIDTH = FULLSCREEN_WIDTH;
		HEIGHT = FULLSCREEN_HEIGHT;		
		if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, WIDTH, HEIGHT, 0, 0) != 0)
		{
			// MessageBox(NULL, L"FullScreen resolution not supported.", L"ZXLauncher", MB_OK);

			WIDTH = WINDOWED_WIDTH;
			HEIGHT = WINDOWED_HEIGHT;			
			if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, WIDTH, HEIGHT, 0, 0) != 0)
			{		
				 MessageBox(NULL, L"Windowed resolution not supported.", L"ZXLauncher", MB_OK);
				 allegro_exit();
				 return -1;
			}
			else 
			{
				SCREEN_MODE == 0;
			}

		}

	}
	else
	{
		WIDTH = WINDOWED_WIDTH;
		HEIGHT = WINDOWED_HEIGHT;			
		if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, WIDTH, HEIGHT, 0, 0) != 0)
		{		
			 MessageBox(NULL, L"Windowed resolution not supported.", L"ZXLauncher", MB_OK);
			 allegro_exit();
			 return -1;
		}
	}

	LOCK_FUNCTION(close_button_handler);
    set_close_button_callback(my_exit);
	
	
	DATAFILE *dfile;
	
	//ExecuteProcess(s2ws(GetExePath().append("\\calc.exe")), L"", -1);
      
#ifdef _DEBUG

	dfile = load_datafile(GetExePath().append("\\ZXLauncher.dat").c_str());

#else
		
	dfile = load_datafile("#"); // Cargamos el .dat comprimido desde el overlay (guardado con exedat.exe)

#endif

	mapscreen = (BITMAP *)dfile[0].dat;
    font = (FONT *)dfile[1].dat;
	beep_sample = (SAMPLE *)dfile[2].dat;

	install_keyboard();		
	set_keyboard_rate(800,40);
	
	// Encabezado
	textout_centre_ex(mapscreen, font, header.c_str(), 160, 32, makecol(0, 0, 0), makecol(198, 198, 198));
	
	// Menu inicial
    posmenu = print_menu(menuindex, posmenu, mapscreen);

    while (!endp)
    {
        //Wait for a key to be pressed

        while (!keypressed() && !endp) { }   
		clear_keybuf();	

        if (key[KEY_ESC])
        {
            if (menuindex == 0)
                continue;

            posmenu = 0;
            if (split(menus[menuindex * 2 + 1][posmenu], ";")[2] == "Menu")
            {				
                menuindex = atoi(split(menus[menuindex * 2 + 1][posmenu], ";")[3].c_str());                        
                posmenu = print_menu(menuindex, posmenu, mapscreen);
            }
			continue;
        }

        if (key[KEY_ENTER])
        {
			
			if (key_shifts & KB_ALT_FLAG) 
			{
				if (SCREEN_MODE == 1)
				{
					WIDTH = WINDOWED_WIDTH;
					HEIGHT = WINDOWED_HEIGHT;	
					SCREEN_MODE = 0;
					if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, WIDTH, HEIGHT, 0, 0) != 0)
					{
						MessageBox(NULL, L"Windowed resolution not supported.", L"ZXLauncher", MB_OK);
						allegro_exit();
						return -1;
					}
					posmenu = print_menu(menuindex, posmenu, mapscreen);					
				}
				else
				{
					WIDTH = FULLSCREEN_WIDTH;
					HEIGHT = FULLSCREEN_HEIGHT;	
					SCREEN_MODE = 1;
					if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, WIDTH, HEIGHT, 0, 0) != 0)
					{		
							// MessageBox(NULL, L"FullScreen resolution not supported.", L"ZXLauncher", MB_OK);

							WIDTH = WINDOWED_WIDTH;
							HEIGHT = WINDOWED_HEIGHT;			
							if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, WIDTH, HEIGHT, 0, 0) != 0)
							{		
								 MessageBox(NULL, L"Windowed resolution not supported.", L"ZXLauncher", MB_OK);
								 allegro_exit();
								 return -1;
							}
							else 
							{
								SCREEN_MODE == 0;
							}
					}					
					posmenu = print_menu(menuindex, posmenu, mapscreen);
				}								
								
			} 
			else 
			{			

				if (split(menus[menuindex * 2 + 1][posmenu], ";")[2] == "Exit")
					break;
								
				if (split(menus[menuindex * 2 + 1][posmenu], ";")[2] == "Menu")
				{
					offsetmenu = 0;
					menuindex = atoi(split(menus[menuindex * 2 + 1][posmenu], ";")[3].c_str());
					posmenu = 0;
					posmenu = print_menu(menuindex, posmenu, mapscreen);
				}

				if (split(menus[menuindex * 2 + 1][posmenu], ";")[2] == "Launch")
				{									

					if (set_gfx_mode(GFX_TEXT, NULL, NULL, NULL, NULL) != 0)
					{
						 MessageBox(NULL, L"Text mode not supported.", L"ZXLauncher", MB_OK);
						 allegro_exit();
						 return -1;
					}

					wstring aa = s2ws(GetExePath().append("\\" + split(menus[menuindex * 2 + 1][posmenu], ";")[3]));
					wstring bb = s2ws(GetExePath().append("\\" + split(menus[menuindex * 2 + 1][posmenu], ";")[4]));
				    wstring cc = s2ws(split(menus[menuindex * 2 + 1][posmenu], ";")[4]);
					//chdir -> cambia el current directory
					chdir(GetExePath().append("\\" + split(split(menus[menuindex * 2 + 1][posmenu], ";")[3],"\\")[0]).c_str());
					//ExecuteProcess(s2ws(GetExePath().append("\\" + split(menus[menuindex * 2 + 1][posmenu], ";")[3])), s2ws(GetExePath().append("\\" + split(menus[menuindex * 2 + 1][posmenu], ";")[4])), -1);					
					
					//ExecuteProcess(s2ws(GetExePath().append("\\" + split(menus[menuindex * 2 + 1][posmenu], ";")[3])), s2ws(split(menus[menuindex * 2 + 1][posmenu], ";")[4]), -1);

					ExecuteProcess(aa, bb, -1);

					if (set_gfx_mode(SCREEN_MODE == 1 ? GFX_AUTODETECT_FULLSCREEN : GFX_AUTODETECT_WINDOWED, WIDTH, HEIGHT, 0, 0) != 0)
					{		
							// MessageBox(NULL, L"FullScreen resolution not supported.", L"ZXLauncher", MB_OK);

							WIDTH = WINDOWED_WIDTH;
							HEIGHT = WINDOWED_HEIGHT;			
							if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, WIDTH, HEIGHT, 0, 0) != 0)
							{		
								 MessageBox(NULL, L"Windowed resolution not supported.", L"ZXLauncher", MB_OK);
								 allegro_exit();
								 return -1;
							}
							else 
							{
								SCREEN_MODE == 0;
							}
					}	
					
					posmenu = print_menu(menuindex, posmenu, mapscreen);
										
					
				}
			}

			continue;
        }
		
        if (key[KEY_DOWN])
        {
            posmenu++;
            posmenu = print_menu(menuindex, posmenu, mapscreen);
        }

        if (key[KEY_UP])
        {
            posmenu--;
            posmenu = print_menu(menuindex, posmenu, mapscreen);

        }
		
		
		play_sample(beep_sample, 128, 128, 1000, 0);
		        

    }
 
	destroy_bitmap(mapscreen);
	allegro_exit();
	return 0;
}





