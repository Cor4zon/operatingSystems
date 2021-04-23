#include "apue.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>


#define FTW_F 1 // файл, не являющийся каталогом
#define FTW_D 2 // каталог
#define FTW_DNR 3 // каталог, который не доступен для чтения
#define FTW_NS 4 // файл, информацию о котором невозможно получить с помощью stat

#define WHITE "\033[0m"
#define GREEN "\033[0;32m"


typedef int Myfunc(const char *, const struct stat *, int);
static Myfunc myfunc;
static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;


static int dopath(const char *filename, int depth, Myfunc *func)
{
    struct stat statbuf;
    struct dirent * dirp;
    DIR *dp;
    int ret = 0;

    if (lstat(filename, &statbuf) < 0) // ошибка lstat
        return(func(filename, &statbuf, FTW_NS));

    for (int i = 0; i < depth; ++i)
        printf("%s*\t", WHITE);

    if (S_ISDIR(statbuf.st_mode) == 0) // не каталог
        return(func(filename, &statbuf, FTW_F)); // отобразить в дереве

    /* каталог */
    if ((ret = func(filename, &statbuf, FTW_D)) != 0)
        return ret;

    if ((dp = opendir(filename)) == NULL) // каталог недоступен
        return(func(filename, &statbuf, FTW_DNR));

    chdir(filename);
    
    for (int i = 0; i < 2; i++, readdir(dp));
    while ((dirp = readdir(dp)) != NULL && ret == 0)
    {
        ret = dopath(dirp->d_name, depth + 1, func);
    }

    chdir("..");

    if (closedir(dp) < 0)
        perror("Невозможно закрыть каталог");

    return(ret);
}


static char *file_type(const struct stat *statbuf)
{
    char *ptr;
    
    if (S_ISREG(statbuf->st_mode))
        ptr = "Обычный файл";
    else if (S_ISDIR(statbuf->st_mode))
        ptr = "Каталог";
    else if (S_ISCHR(statbuf->st_mode))
        ptr = "Файл символьного устройства";
    else if (S_ISBLK(statbuf->st_mode))
        ptr = "Файл блочного устройства";
    else if (S_ISFIFO(statbuf->st_mode))
        ptr = "Fifo";
    else if (S_ISLNK(statbuf->st_mode))
        ptr = "Символическая ссылка";
    else if (S_ISSOCK(statbuf->st_mode))
        ptr = "Сокет";
    else
        ptr = "** Неизвестный тип файла **";
    
    return ptr;
}


static int show_file(const char *pathname, const struct stat *statbuf, int type)
{
    switch(type)
    {
        case FTW_F:
            printf( "%s-- %s  - %s%s\n", WHITE, pathname, GREEN, file_type(statbuf));
            break;
        case FTW_D:
            printf( "%s-- %s/  - %s%s\n", WHITE, pathname, GREEN, file_type(statbuf));
            break;
        case FTW_DNR:
            perror("К одному из каталогов закрыт доступ.");
            return -1;
        case FTW_NS:
            perror("Ошибка функции stat.");
            return -1;
        default:
            perror("Неизвестый тип файла.");
            return -1;
    }
    printf("%s", WHITE);
    
    return 0;
}


int main(int argc, char * argv[])
{
    int ret = -1;
    
    if (argc != 2)
    {
        printf("Ошибка ввода: укажите путь к каталогу\n");
        exit(-1);
    }

    ret = dopath(argv[1], 0, show_file);

    exit(ret);
}


