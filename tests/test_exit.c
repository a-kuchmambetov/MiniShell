#include "../main.h"
#include "../src/built_in_comands/builtins.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

void test_exit_builtin(t_shell_data *data, char *arg, const char *desc)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        // дочірній процес
        char *args[] = {"exit", arg, NULL};
        exec_builtin(data, args);
        _exit(0); // на випадок, якщо exec_builtin не завершить
    }
    else if (pid > 0)
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            printf("%s -> exit status = %d\n", desc, WEXITSTATUS(status));
        else
            printf("%s -> abnormal termination\n", desc);
    }
    else
    {
        perror("fork failed");
    }
}

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;

    t_shell_data data = {0};

    parse_envp(&data, envp);
    parse_exec_folders(&data);
    set_envp_from_env(&data);

    printf("=== TESTING EXIT BUILTIN ===\n\n");

    // --- Test 1: Exit with no arguments ---
    test_exit_builtin(&data, NULL, "--- Test 1: Exit with no arguments ---");

    // --- Test 2: Exit with numeric argument 42 ---
    test_exit_builtin(&data, "42", "--- Test 2: Exit with numeric argument '42' ---");

    // --- Test 3: Exit with numeric argument 0 ---
    test_exit_builtin(&data, "0", "--- Test 3: Exit with numeric argument '0' ---");

    // --- Test 4: Exit with negative number ---
    test_exit_builtin(&data, "-7", "--- Test 4: Exit with negative number '-7' ---");

    // --- Test 5: Exit with non-numeric argument ---
    test_exit_builtin(&data, "abc", "--- Test 5: Exit with non-numeric argument 'abc' ---");

    // --- Test 6: Exit with multiple arguments ---
    // Для цього можна змінити exec_builtin, щоб передати кілька аргументів
    pid_t pid = fork();
    if (pid == 0)
    {
        char *args[] = {"exit", "1", "2", NULL};
        exec_builtin(&data, args);
        _exit(0);
    }
    else if (pid > 0)
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            printf("--- Test 6: Exit with multiple arguments '1 2' -> exit status = %d (expected: 1)\n", WEXITSTATUS(status));
        else
            printf("--- Test 6: Exit with multiple arguments '1 2' -> abnormal termination\n");
    }

    free_shell_data(&data);
    return 0;
}
