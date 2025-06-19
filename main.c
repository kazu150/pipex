#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

char *parse_input_command(char **argv)
{
    int file_len;
    int cmd_len;
    int i;

    file_len = 0;
    cmd_len = 0;
    i = 0;
    while (argv[2][cmd_len])
        cmd_len++;
    while (argv[1][file_len])
        file_len++;
    char *command = malloc(cmd_len + file_len + 2);
    if (!command)
        return NULL;
    while (*argv[2])
        command[i++] = *argv[2]++;
    command[i] = ' ';
    i = 0;
    while (*argv[1])
        command[cmd_len + 1 + i++] = *argv[1]++;
    command[cmd_len + 1 + i] = 0;
    return command;
}

//  "./a.out file1 "grep a1" |  wc -w > file2" ここまではできる
int main(int argc, char **argv)
{
    // TODO: pipe以降を書くタイミングで直す
    if (argc < 3)
        return 1;

    char *command = parse_input_command(argv);
    if (!command)
        return 1;
    pid_t pid = fork();
    if (pid < 0)
        return 1;
    if (pid == 0){
        char *args[] = {"sh", "-c", command, NULL};
        execve("/bin/sh", args, 0);
        return 0;
    }
    return 0;
}
