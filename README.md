# wallpaper.h

[![](https://img.shields.io/github/v/tag/thechampagne/wallpaper.h?label=version)](https://github.com/thechampagne/wallpaper.h/releases/latest) [![](https://img.shields.io/github/license/thechampagne/wallpaper.h)](https://github.com/thechampagne/wallpaper.h/blob/main/LICENSE)

A **C/C++** library to get and set desktop background.

### API

```c
int wallpaper_get(char* file_path);

int wallpaper_set(const char* file_path);

int wallpaper_set_mode(wallpaper_mode mode);

int wallpaper_set_from_url(const char* url);
```

### Supported desktops

* Windows

### License

This repo is released under the [MIT](https://github.com/thechampagne/wallpaper.h/blob/main/LICENSE).