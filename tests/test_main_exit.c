#include "../main.h"
#include "../src/built_in_comands/builtins.h"

int get_bash_exit_code(const char *arg)
{
    pid_t pid = fork();
    if (pid < 0) { perror("fork failed"); return -1; }

    if (pid == 0)
    {
        char cmd[256];
        snprintf(cmd, sizeof(cmd), "exit %s", arg ? arg : "");
        execl("/bin/bash", "bash", "-c", cmd, NULL);
        perror("execl failed");
        exit(127);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            return WEXITSTATUS(status);
        return -1;
    }
}

/* Тест порівняння builtin_exit vs bash exit */
void test_exit_vs_bash(t_shell_data *data, char *input)
{
    int my_exit;
    pid_t pid = fork();
    if (pid < 0) { perror("fork failed"); return; }
    if (pid == 0)
    {
        // --- Розбиваємо input на аргументи ---
        char *args[10] = {"exit", NULL};
        if (input && *input)
        {
            char buf[128];
            strncpy(buf, input, sizeof(buf));
            buf[sizeof(buf) - 1] = '\0';

            int i = 1;
            char *token = strtok(buf, " \t");
            while (token && i < 9)
            {
                args[i++] = token;
                token = strtok(NULL, " \t");
            }
            args[i] = NULL;
        }

        builtin_exit(data, args);
        exit(1); // на випадок, якщо exit не спрацював
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        my_exit = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    }

    int bash_exit = get_bash_exit_code(input);

    printf("%-20s | my exit: %-3d | bash exit: %-3d | %s\n\n",
        input ? input : "NULL",
        my_exit,
        bash_exit,
        (my_exit == bash_exit ? "\033[0;32mPASS\033[0m" : "\033[0;31mFAIL\033[0m"));
}


int main(void)
{
    t_shell_data data = {0};

    const char *tests[] = {
        NULL, "", "0", "42", "-1", "255", "256", "999",
        "abc", "123abc", "--5", "9223372036854775807",
        "-9223372036854775808", "9223372036854775808", "-9223372036854775809",
        "   42   ", "  -15  ", " +123 ", "  12 34 "
    };

    printf("=== Тест порівняння builtin_exit vs bash exit ===\n");
    for (size_t i = 0; i < sizeof(tests)/sizeof(tests[0]); i++)
        test_exit_vs_bash(&data, (char *)tests[i]);

    return 0;
}