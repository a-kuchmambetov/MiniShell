#include "../main.h"
#include <signal.h>
#include <sys/wait.h>
#include "../src/built_in_commands/builtins.h"

/* ===========================================================
   UTILS
   =========================================================== */
void process_input(t_shell_data *data, char *input)
{
    char **args;

    args = ft_split(input, ' ');
    if (!args)
        return;

    if (is_builtin(args[0]))
        data->last_exit_status = exec_builtin(data, args);
    else
        exec_cmd(data, args[0], args);

    free_str_arr(args);
}
void safe_process_input(t_shell_data *data, const char *input)
{
    char *copy = ft_strdup(input);
    if (!copy)
        return;
    process_input(data, copy);
    free(copy);
}

int ft_strcmp(const char *s1, const char *s2)
{
    size_t i = 0;
    while (s1[i] && s1[i] == s2[i])
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

/* ===========================================================
   EXEC TESTS (run commands, check status)
   =========================================================== */
void execute_and_check(t_shell_data *data, const char *input, int expected_status, const char *test_name)
{
    pid_t pid = fork();
    int status;

    if (pid == 0)
    {
        char *copy = ft_strdup(input);
        char **args = ft_split(copy, ' ');
        if (is_builtin(args[0]))
            data->last_exit_status = exec_builtin(data, args);
        else
            exec_cmd(data, args[0], args);
        free(copy);
        free_str_arr(args);
        _exit(WEXITSTATUS(data->last_exit_status));
    }
    waitpid(pid, &status, 0);
    int actual = WEXITSTATUS(status);
    if (actual == expected_status)
        printf(COLOR_GREEN "[OK] %s ('%s') -> %d\n" COLOR_RESET, test_name, input, actual);
    else
        printf(COLOR_RED "[FAIL] %s ('%s'): expected %d, got %d\n" COLOR_RESET, test_name, input, expected_status, actual);
}

/* ===========================================================
   SIGNAL TEST
   =========================================================== */
void test_signal_behavior()
{
    pid_t pid = fork();
    if (pid == 0)
    {
        printf("[Child] waiting 2s (should die by SIGINT)\n");
        sleep(2);
        printf(COLOR_RED "[FAIL] Child survived signal!\n" COLOR_RESET);
        _exit(1);
    }
    else
    {
        sleep(1);
        kill(pid, SIGINT);
        int st; waitpid(pid, &st, 0);
        if (WIFSIGNALED(st) && WTERMSIG(st) == SIGINT)
            printf(COLOR_GREEN "[OK] SIGINT handled properly (Ctrl+C)\n" COLOR_RESET);
        else
            printf(COLOR_RED "[FAIL] SIGINT behavior wrong (signal %d)\n" COLOR_RESET, WTERMSIG(st));
    }
}

/* ===========================================================
   ENV CHECK
   =========================================================== */
void check_env_value(t_shell_data *data, const char *key, const char *expected, const char *test_name)
{
    char *val = get_env_value(data->envp, key);
    if (val && ft_strcmp(val, expected) == 0)
        printf(COLOR_GREEN "[OK] %s: %s='%s'\n" COLOR_RESET, test_name, key, val);
    else
        printf(COLOR_RED "[FAIL] %s: expected '%s', got '%s'\n" COLOR_RESET, test_name, expected, val ? val : "(NULL)");
}

/* ===========================================================
   MAIN
   =========================================================== */
int main(int argc, char **argv, char **envp)
{
    (void)argc; (void)argv;
    t_shell_data data;
    init_shell_data(&data, envp);

    printf(COLOR_MAGENTA "=== MINISHELL FULL TEST SUITE ===\n" COLOR_RESET);

    /* -------------------------------------------------------
       1. SIMPLE COMMANDS
       ------------------------------------------------------- */
    printf("\n--- 1. SIMPLE COMMANDS ---\n");
    execute_and_check(&data, "/bin/ls", 0, "Basic ls");
    execute_and_check(&data, "ls does_not_exist", 2, "ls fail");
    execute_and_check(&data, "/bin/nonexistent", 127, "invalid path");
    execute_and_check(&data, "nonexistent_command", 127, "command not found");

    /* -------------------------------------------------------
       2. BUILTINS
       ------------------------------------------------------- */
    printf("\n--- 2. BUILTINS ---\n");
    safe_process_input(&data, "export TESTVAR=Minishell");
    check_env_value(&data, "TESTVAR", "Minishell", "Export var");
    safe_process_input(&data, "echo $TESTVAR");
    safe_process_input(&data, "unset TESTVAR");
    safe_process_input(&data, "echo $TESTVAR");

    safe_process_input(&data, "pwd");
    safe_process_input(&data, "cd ..");
    safe_process_input(&data, "pwd");
    safe_process_input(&data, "cd -");
    safe_process_input(&data, "pwd");

    /* -------------------------------------------------------
       3. PIPES AND REDIRECTIONS
       ------------------------------------------------------- */
    printf("\n--- 3. PIPES & REDIRECTIONS ---\n");
    safe_process_input(&data, "echo hello | wc -c");
    safe_process_input(&data, "echo hi > out.txt");
    safe_process_input(&data, "cat < out.txt");
    safe_process_input(&data, "echo again >> out.txt");
    safe_process_input(&data, "cat < out.txt > copy.txt");

    /* -------------------------------------------------------
       4. SYNTAX ERRORS
       ------------------------------------------------------- */
    printf("\n--- 4. INVALID SYNTAX ---\n");
    safe_process_input(&data, "| ls");
    safe_process_input(&data, "cat <");
    safe_process_input(&data, "ls || echo fail");
    safe_process_input(&data, "ls && echo ok");

    /* -------------------------------------------------------
       5. QUOTES & EXPANSIONS
       ------------------------------------------------------- */
    printf("\n--- 5. QUOTES & EXPANSION ---\n");
    safe_process_input(&data, "echo \"$USER\"");
    safe_process_input(&data, "echo '$USER'");
    safe_process_input(&data, "echo \"Hello World\"");
    safe_process_input(&data, "echo 'Hello World'");
    safe_process_input(&data, "echo \"Mix '$USER' end\"");

    /* -------------------------------------------------------
       6. ENV & PATH
       ------------------------------------------------------- */
    printf("\n--- 6. ENV & PATH ---\n");
    safe_process_input(&data, "export PATH=/usr/bin:/bin");
    execute_and_check(&data, "ls", 0, "PATH search");
    safe_process_input(&data, "unset PATH");
    execute_and_check(&data, "ls", 127, "no PATH");

    /* -------------------------------------------------------
       7. EMPTY INPUT
       ------------------------------------------------------- */
    printf("\n--- 7. EMPTY INPUT ---\n");
    safe_process_input(&data, "");
    safe_process_input(&data, "   ");
    safe_process_input(&data, "\t");

    /* -------------------------------------------------------
       8. SIGNALS
       ------------------------------------------------------- */
    printf("\n--- 8. SIGNALS ---\n");
    test_signal_behavior();

    /* -------------------------------------------------------
       9. EXIT
       ------------------------------------------------------- */
    printf("\n--- 9. EXIT (manual visual check) ---\n");
    printf("Run ./minishell and test Ctrl+D, Ctrl+C manually.\n");

    printf(COLOR_MAGENTA "\n=== ALL TESTS COMPLETED ===\n" COLOR_RESET);
    free_shell_data(&data);
    return 0;
}
#include "../main.h"
#include <signal.h>
#include <sys/wait.h>
#include "../src/built_in_commands/builtins.h"

/* ===========================================================
   UTILS
   =========================================================== */
void process_input(t_shell_data *data, char *input)
{
    char **args;

    args = ft_split(input, ' ');
    if (!args)
        return;

    if (is_builtin(args[0]))
        data->last_exit_status = exec_builtin(data, args);
    else
        exec_cmd(data, args[0], args);

    free_str_arr(args);
}
void safe_process_input(t_shell_data *data, const char *input)
{
    char *copy = ft_strdup(input);
    if (!copy)
        return;
    process_input(data, copy);
    free(copy);
}

int ft_strcmp(const char *s1, const char *s2)
{
    size_t i = 0;
    while (s1[i] && s1[i] == s2[i])
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

/* ===========================================================
   EXEC TESTS (run commands, check status)
   =========================================================== */
void execute_and_check(t_shell_data *data, const char *input, int expected_status, const char *test_name)
{
    pid_t pid = fork();
    int status;

    if (pid == 0)
    {
        char *copy = ft_strdup(input);
        char **args = ft_split(copy, ' ');
        if (is_builtin(args[0]))
            data->last_exit_status = exec_builtin(data, args);
        else
            exec_cmd(data, args[0], args);
        free(copy);
        free_str_arr(args);
        _exit(WEXITSTATUS(data->last_exit_status));
    }
    waitpid(pid, &status, 0);
    int actual = WEXITSTATUS(status);
    if (actual == expected_status)
        printf(COLOR_GREEN "[OK] %s ('%s') -> %d\n" COLOR_RESET, test_name, input, actual);
    else
        printf(COLOR_RED "[FAIL] %s ('%s'): expected %d, got %d\n" COLOR_RESET, test_name, input, expected_status, actual);
}

/* ===========================================================
   SIGNAL TEST
   =========================================================== */
void test_signal_behavior()
{
    pid_t pid = fork();
    if (pid == 0)
    {
        printf("[Child] waiting 2s (should die by SIGINT)\n");
        sleep(2);
        printf(COLOR_RED "[FAIL] Child survived signal!\n" COLOR_RESET);
        _exit(1);
    }
    else
    {
        sleep(1);
        kill(pid, SIGINT);
        int st; waitpid(pid, &st, 0);
        if (WIFSIGNALED(st) && WTERMSIG(st) == SIGINT)
            printf(COLOR_GREEN "[OK] SIGINT handled properly (Ctrl+C)\n" COLOR_RESET);
        else
            printf(COLOR_RED "[FAIL] SIGINT behavior wrong (signal %d)\n" COLOR_RESET, WTERMSIG(st));
    }
}

/* ===========================================================
   ENV CHECK
   =========================================================== */
void check_env_value(t_shell_data *data, const char *key, const char *expected, const char *test_name)
{
    char *val = get_env_value(data->envp, key);
    if (val && ft_strcmp(val, expected) == 0)
        printf(COLOR_GREEN "[OK] %s: %s='%s'\n" COLOR_RESET, test_name, key, val);
    else
        printf(COLOR_RED "[FAIL] %s: expected '%s', got '%s'\n" COLOR_RESET, test_name, expected, val ? val : "(NULL)");
}

/* ===========================================================
   MAIN
   =========================================================== */
int main(int argc, char **argv, char **envp)
{
    (void)argc; (void)argv;
    t_shell_data data;
    init_shell_data(&data, envp);

    printf(COLOR_MAGENTA "=== MINISHELL FULL TEST SUITE ===\n" COLOR_RESET);

    /* -------------------------------------------------------
       1. SIMPLE COMMANDS
       ------------------------------------------------------- */
    printf("\n--- 1. SIMPLE COMMANDS ---\n");
    execute_and_check(&data, "/bin/ls", 0, "Basic ls");
    execute_and_check(&data, "ls does_not_exist", 2, "ls fail");
    execute_and_check(&data, "/bin/nonexistent", 127, "invalid path");
    execute_and_check(&data, "nonexistent_command", 127, "command not found");

    /* -------------------------------------------------------
       2. BUILTINS
       ------------------------------------------------------- */
    printf("\n--- 2. BUILTINS ---\n");
    safe_process_input(&data, "export TESTVAR=Minishell");
    check_env_value(&data, "TESTVAR", "Minishell", "Export var");
    safe_process_input(&data, "echo $TESTVAR");
    safe_process_input(&data, "unset TESTVAR");
    safe_process_input(&data, "echo $TESTVAR");

    safe_process_input(&data, "pwd");
    safe_process_input(&data, "cd ..");
    safe_process_input(&data, "pwd");
    safe_process_input(&data, "cd -");
    safe_process_input(&data, "pwd");

    /* -------------------------------------------------------
       3. PIPES AND REDIRECTIONS
       ------------------------------------------------------- */
    printf("\n--- 3. PIPES & REDIRECTIONS ---\n");
    safe_process_input(&data, "echo hello | wc -c");
    safe_process_input(&data, "echo hi > out.txt");
    safe_process_input(&data, "cat < out.txt");
    safe_process_input(&data, "echo again >> out.txt");
    safe_process_input(&data, "cat < out.txt > copy.txt");

    /* -------------------------------------------------------
       4. SYNTAX ERRORS
       ------------------------------------------------------- */
    printf("\n--- 4. INVALID SYNTAX ---\n");
    safe_process_input(&data, "| ls");
    safe_process_input(&data, "cat <");
    safe_process_input(&data, "ls || echo fail");
    safe_process_input(&data, "ls && echo ok");

    /* -------------------------------------------------------
       5. QUOTES & EXPANSIONS
       ------------------------------------------------------- */
    printf("\n--- 5. QUOTES & EXPANSION ---\n");
    safe_process_input(&data, "echo \"$USER\"");
    safe_process_input(&data, "echo '$USER'");
    safe_process_input(&data, "echo \"Hello World\"");
    safe_process_input(&data, "echo 'Hello World'");
    safe_process_input(&data, "echo \"Mix '$USER' end\"");

    /* -------------------------------------------------------
       6. ENV & PATH
       ------------------------------------------------------- */
    printf("\n--- 6. ENV & PATH ---\n");
    safe_process_input(&data, "export PATH=/usr/bin:/bin");
    execute_and_check(&data, "ls", 0, "PATH search");
    safe_process_input(&data, "unset PATH");
    execute_and_check(&data, "ls", 127, "no PATH");

    /* -------------------------------------------------------
       7. EMPTY INPUT
       ------------------------------------------------------- */
    printf("\n--- 7. EMPTY INPUT ---\n");
    safe_process_input(&data, "");
    safe_process_input(&data, "   ");
    safe_process_input(&data, "\t");

    /* -------------------------------------------------------
       8. SIGNALS
       ------------------------------------------------------- */
    printf("\n--- 8. SIGNALS ---\n");
    test_signal_behavior();

    /* -------------------------------------------------------
       9. EXIT
       ------------------------------------------------------- */
    printf("\n--- 9. EXIT (manual visual check) ---\n");
    printf("Run ./minishell and test Ctrl+D, Ctrl+C manually.\n");

    printf(COLOR_MAGENTA "\n=== ALL TESTS COMPLETED ===\n" COLOR_RESET);
    free_shell_data(&data);
    return 0;
}
