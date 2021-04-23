
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#define BUF_SIZE 0x400
#define CMDLINE 1
#define ENVIRON 2
#define STAT 3
#define MAPS 4
#define STATM 5

char *stat_fields[] = {
    "pid",          // ID процесса
    "comm",         // Имя исполняемого файла
    "state",        // Состояние процесса (RSDZTW)
    "ppid",         // ID родительского процесса
    "pgrp",         // ID группы процесса
    "session",      // ID сессии процесса
    "tty_nr",       // Терминал tty, который использует процесс (управляющий терминал)
    "tpgid",        // Идентификатор группы процесса,
                    //который в настоящий момент владеет терминалом tty,
                    //к которому подключен данный процесс
    "flags",        // Флаги процесса
    "minflt",       // Количество незначительных сбоев,
                    // которые возникли при работе процесса
                    // и которые не требуют загрузки страницы памяти с диска.
    "cminflt",      // Количество незначительных сбоев,
                    // которые возникли при ожидании окончания работы процессов-потомков
    "majflt",       // Количество значительных сбоев,
                    // которые возникли при работе процесса
                    // и которые потребовали загрузки страницы памяти с диска
    "cmajflt",      // Количество значительных сбоев,
                    // которые возникли при ожидании окончания работы процессов-потомков
    "utime",        // Количество тиков, которое получил процесс, выполняясь в режиме задачи
    "stime",        // Количество тиков, которое получил процесс, выполняясь в режиме ядра
    "cutime",       // Количество тиков, которое поулчил процесс и его потомки, выполняясь в режиме задачи
    "cstime",       // Количество тиков, которое получил процесс, выполняясь в режиме ядра в ожидании
                    // завершения работы процессов-потомков
    "priority",     // Приоритет процесса
    "nice",         // Значение для nice в диапазоне от 19 (низший приоритет) до -19 (наивысший приоритет)
    "num_threads",  // Количество потоков
    "itrealvalue",  // Время (в тиках) перед следующей посылкой процессу SIGALRM из-за внутреннего таймера
    "start_tiime",  // Время (в тиках) запуска процесса после начальной загрузки системы
    "vsize",        // Размер виртуальной памяти в байтах
    "rss",          // Resident Set Size: Количество страниц процесса в физической памяти
    "rsslim",         // Текущий лимит в байтах на резидентный размер процесса (обычно 4294967295 на i386)
    "startcode",    // Адрес, выше которого может выполняться код программы
    "endcode",      // дрес, ниже которого может выполняться код программы
    "startstack",   // Адрес начала (т. е. дна) стека
    "kstkesp",      // Текущее значение ESP (Stack pointer), найденное на странице стека ядра для данного процесса
    "kstkeip",      // Текущее значение EIP (instruction pointer)
    "signal",       // Карта ожидающих сигналов (обычно 0).
    "blocked",      // Карта блокированных сигналов (обычно 0, или 2 для командных интерпретаторов)
    "sigignore",    // Карта игнорируемых сигналов
    "sigcatch",     // Карта перехваченных сигналов
    "wchan",        // Канал, в котором происходит ожидание процесса
    "nswap",        // Количество страниц на своппинге
    "cnswap",       // Суммарное nswap для процессов-потомков
    "exit_signal",  // Сигнал, который будет послан родителю, когда процесс умрёт
    "processor",    // Номер процессора, на котором последний раз выполнялся процесс
    "rt_priority",  // Приоритет планирования в реальном времени - число в диапазоне от 1 до 99 для процессов,                //запланированных в соответствии с политикой реального времени
    "policy",       // Политика планирования
    "delayacct_blkio_ticks", // Общие блочные задержки ввода/вывода
    "quest_time",   // Гостевое время процесса (time spent running a
                    //virtual CPU for a guest operating system)
    "cquest_time",  // Гостевое время  дочерних процессов
    "start_data",   // Адрес, над которым размещаются инициализированные и неинициализированные данные программы (BSS).
    "end_data",     // Адрес, под которым размещаются инициализированные и неинициализированные данные программы (BSS).
    "start_brk",    // Адрес, выше которого куча программ может быть расширена с помощью brk.
    "arg_start",    // Адрес, над которым размещаются аргументы командной строки программы (argv).
    "arg_end",      // Адрес, под которым размещаются аргументы командной строки программы (argv).
    "env_start",    // Адрес, над которым размещена программная среда
    "env_end",      // Адрес, под которым размещена программная среда
    "exit_code"     // Состояние выхода потока в форме, сообщаемой waitpid.

};

char *maps_fields[] = {
    "начальный адрес диапазона адресов",
    "конечный адрес диапазона адресов",
    "права доступа",
    "смещение",
    "старшие и младшие номера, которыми идентифицируются внешние устройства",
    "номер inode"

};

static int i = 0;

void MapsOutput(char *buf, FILE *res_file)
{
    int len = strlen(buf);
    int currentName = 0;
    char *pch = strtok(buf, " ");

    while (pch != NULL && i < 6)
    {
        fprintf(res_file, "\n%10s:\t %s", maps_fields[i], pch);
        pch = strtok(NULL, " ");
        i++;
    }
}

void StatOutput(char *buf, FILE *res_file)
{
    int len = strlen(buf);
    int currentName = 0;
    char *pch = strtok(buf, " ");

    while (pch != NULL && i < 52)
    {
        fprintf(res_file, "\n%10s:\t %s", stat_fields[i], pch);
        pch = strtok(NULL, " ");
        i++;
    }
}

void SimpleOutput(char *buf, FILE *res_file)
{
    fprintf(res_file, "%s\n", buf);
}

void read_file(char *filename, void (*output_func)(char*, FILE*), FILE *res_file, int id)
{
    char buf[BUF_SIZE];
    int len, i;
    FILE *f;

    f = fopen(filename, "r");
    
    if (id == 1)
        fprintf(res_file, "\n%s\n", "CMDLINE - файл, содержащий полную командную строку запуска процесса\n");
    else if (id == 2)
        fprintf(res_file, "\n%s\n", "ENVIRON - файл, содержащий окружение процесса\n");
    else if (id == 3)
        fprintf(res_file, "\n%s\n", "STAT - файл, содержащий информацию о статусе процесса\n");
    else if (id == 4)
        fprintf(res_file, "\n%s\n", "MAPS - файл, содержащий информацию о выделенных участках памяти процессу\n");
    else
        fprintf(res_file, "\n%s\n", "STATM - файл, содержащий информацию о состоянии памяти в страницах\n");

        
    while ((len = fread(buf, 1, BUF_SIZE, f)) > 0)
    {
        for (i = 0; i < len; i++)
            if (buf[i] == 0)
                buf[i] = 10;
        buf[len - 1] = 0;
        
        output_func(buf, res_file);
    }
    fclose(f);
}
int main(int argc, char *argv[])
{
    FILE *res_file = fopen("proc_info", "a");
    
    read_file("/proc/18053/cmdline",  SimpleOutput, res_file, 1);
    read_file("/proc/18053/environ",  SimpleOutput, res_file, 2);
    read_file("/proc/18053/stat", StatOutput, res_file, 3);
    read_file("/proc/18053/maps", SimpleOutput, res_file, 4);
    read_file("/proc/18053/statm", SimpleOutput, res_file, 5);
    
    
    
    
    
    //chdir("/proc/self/fd");
    //execlp("/bin/ls","ls","-l",0);
    fclose(res_file);
    return 0;
}
