/*
* Copyright (c) 2022 XXIV
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#ifndef __WALLPAPER_H__
#define __WALLPAPER_H__

#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include <Lmcons.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	WALLPAPER_FILL = 10, 	// 10
	WALLPAPER_FIT = 6,		// 6
	WALLPAPER_STRETCH = 2,  // 2
	WALLPAPER_TILE = 1,		// 1
	WALLPAPER_CENTER = 0,   // 0
	WALLPAPER_SPAN = 22     // 22
} wallpaper_mode;

/*
* Get desktop background path.
* 
* Example:
* * *
* int main()
* {
*     char* file_path[260];
*     wallpaper_get(file_path);
*     printf("%s\n", file_path);
*     return 0;
* }
* * *
*
* @param file_path
* @return 0 on success and non zero value on failure
*/
int wallpaper_get(char* file_path);

/*
* Set desktop background from file.
* 
* Example:
* * *
* int main()
* {
*     wallpaper_set("path\to\background");
*     return 0;
* }
* * *
*
* @param file_path
* @return 0 on success and non zero value on failure
*/
int wallpaper_set(const char* file_path);

/*
* Set desktop background mode e.g. fit.
* 
* Example:
* * *
* int main()
* {
*     wallpaper_set_mode(WALLPAPER_FIT);
*     return 0;
* }
* * *
*
* @param mode wallpaper_mode
* @return 0 on success and non zero value on failure
*/
int wallpaper_set_mode(wallpaper_mode mode);

/*
* Set desktop background from url.
* 
* Example:
* * *
* int main()
* {
*     wallpaper_set_from_url("https://path/to/background");
*     return 0;
* }
* * *
*
* @param url
* @return 0 on success and non zero value on failure
*/
int wallpaper_set_from_url(const char* url);

#ifdef __cplusplus
}
#endif

int wallpaper_get(char* file_path)
{
	if (SystemParametersInfoA(
		SPI_GETDESKWALLPAPER,
		MAX_PATH,
		(void*) file_path,
		0
	) == 0)
	{
		return -1;
	}
	return 0;
}

int wallpaper_set(const char* file_path)
{
	if (SystemParametersInfoA(
		SPI_SETDESKWALLPAPER,
		0,
		(void*) file_path,
		SPIF_UPDATEINIFILE | SPIF_SENDCHANGE
	) == 0)
	{
		return -1;
	}
	return 0;
}

// @return desktop background path from Temp folder downloaded
// using download_file function.
char* wallpaper_path()
{
	char username[UNLEN + 1];
	long unsigned int username_length = UNLEN;
	if (GetUserNameA(username, &username_length) == 0)
	{
		return NULL;
	}
	char* path = (char*) malloc(38 + username_length * sizeof(char));
	sprintf(path, "C:\\users\\%s\\AppData\\Local\\Temp\\wallpaper", username);
	return path;
}

// download from url and save it to a file in Temp folder.
int download_file(const char* url)
{
	char* path = wallpaper_path();
	if (path == NULL)
	{
		return -1;
	}
	if (URLDownloadToFile(
		NULL,
		url,
		path,
		0,
		NULL
	) != S_OK)
	{
		free(path);
		return -1;
	}
	free(path);
	return 0;
}

int wallpaper_set_mode(wallpaper_mode mode)
{
	HKEY hkey;
	if (RegCreateKeyExA(
		HKEY_CURRENT_USER,
		"Control Panel\\Desktop",
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_SET_VALUE,
		NULL,
		&hkey,
		NULL
	) != 0)
	{
		return -1;
	}
	char value[3] = "0";
	char tile[2]  = "0";
	switch (mode)
	{
		case WALLPAPER_FILL:
			strcpy(value, "10");
			break;
		case WALLPAPER_FIT:
			strcpy(value, "6");
			break;
		case WALLPAPER_STRETCH:
			strcpy(value, "2");
			break;
		case WALLPAPER_TILE:
			 strcpy(tile, "1");
			 // strcpy(value, "0");
			 break;
		case WALLPAPER_CENTER:
			// strcpy(value, "0");
			 break;
		case WALLPAPER_SPAN:
			 strcpy(value, "22");
			 break;
	}
	if (RegSetValueExA(
		hkey,
		"TileWallpaper",
		0,
		REG_SZ,
		(const BYTE*)tile,
		sizeof(tile)	
	) != 0)
	{
		RegCloseKey(hkey);
		return -1;
	}

	if (RegSetValueExA(
		hkey,
		"WallpaperStyle",
		0,
		REG_SZ,
		(const BYTE*)value,
		sizeof(value)	
	) != 0)
	{
		RegCloseKey(hkey);
		return -1;
	}

	if (RegCloseKey(hkey) != 0)
	{
		return -1;
	}

	char path[MAX_PATH];
	if (wallpaper_get(path) != 0)
	{
		return -1;
	}
	if (wallpaper_set(path) != 0)
	{
		return -1;
	}
	return 0;
}

int wallpaper_set_from_url(const char* url)
{
	if (download_file(url) != 0)
	{
		return -1;
	}
	char* path = wallpaper_path();
	if (path == NULL)
	{
		return -1;
	}
	if (wallpaper_set(path) != 0)
	{
		free(path);
		return -1;
	}
	free(path);
	return 0;
}

#endif // __WALLPAPER_H__