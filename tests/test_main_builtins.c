#include "../main.h"
#include "../src/built_in_commands/builtins.h"

// ===========================================================
//   TEST MODE SAFETY: redefine exit() for unit testing
// ===========================================================
#ifdef TEST_MODE
# define exit mock_exit
void mock_exit(int status)
{
    printf(COLOR_YELLOW "[MOCK_EXIT] Called with status %d\n" COLOR_RESET, status);
}
#endif

// ===========================================================
//   Helper: Execute one command (built-in or external)
// ===========================================================
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

// ===========================================================
//   Simple strcmp implementation (if not in libft)
// ===========================================================
int ft_strcmp(const char *s1, const char *s2)
{
    size_t i = 0;
    while (s1[i] && s1[i] == s2[i])
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

// ===========================================================
//   Execute + check exit status
// ===========================================================
void execute_and_check(t_shell_data *data, char *input, int expected_status, const char *test_name)
{
    char *input_copy = ft_strdup(input);
    if (!input_copy)
    {
        printf(COLOR_RED "[FAIL] %s: MEMORY ALLOCATION ERROR\n" COLOR_RESET, test_name);
        return;
    }

    process_input(data, input_copy);

    int actual_status = WIFEXITED(data->last_exit_status)
        ? WEXITSTATUS(data->last_exit_status)
        : data->last_exit_status;

    if (actual_status == expected_status)
        printf(COLOR_GREEN "[OK] %s: '%s' -> Status: %d\n" COLOR_RESET, test_name, input, actual_status);
    else
        printf(COLOR_RED "[FAIL] %s: '%s' -> EXPECTED: %d, GOT: %d (raw: %d)\n" COLOR_RESET,
               test_name, input, expected_status, actual_status, data->last_exit_status);

    free(input_copy);
}

// ===========================================================
//   Helper: Check environment variable value
// ===========================================================
void check_env_value(t_shell_data *data, const char *key, const char *expected_value, const char *test_name)
{
    char *result_value = get_env_value(data->envp, key);

    if (result_value && ft_strncmp(result_value, expected_value, ft_strlen(expected_value)) == 0)
        printf(COLOR_GREEN "[OK] %s: %s='%s'\n" COLOR_RESET, test_name, key, result_value);
    else
        printf(COLOR_RED "[FAIL] %s: %s EXPECTED '%s', GOT '%s'\n" COLOR_RESET,
               test_name, key, expected_value, result_value ? result_value : "(NULL)");
}

// ===========================================================
//   Helper: Check if current working directory ends correctly
// ===========================================================
void check_cwd(const char *expected_end, const char *test_name)
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, PATH_MAX))
    {
        const char *last = ft_strrchr(cwd, '/');
        if (last && ft_strcmp(last + 1, expected_end) == 0)
            printf(COLOR_GREEN "[OK] %s: CWD ends with '%s'\n" COLOR_RESET, test_name, expected_end);
        else
            printf(COLOR_RED "[FAIL] %s: Expected */%s, got %s\n" COLOR_RESET,
                   test_name, expected_end, cwd);
    }
    else
        printf(COLOR_RED "[FAIL] %s: getcwd failed\n" COLOR_RESET, test_name);
}

// ===========================================================
//   MAIN
// ===========================================================
int main(int argc, char **argv, char **envp)
{
    t_shell_data data;
    char orig_dir[PATH_MAX];
    char *home_dir = getenv("HOME");

    (void)argc;
    (void)argv;
    getcwd(orig_dir, PATH_MAX);

    init_shell_data(&data, envp);

    printf(COLOR_BLUE "=== MINISHELL TEST MODE ===\n" COLOR_RESET);

    // -----------------------------------------------------------
    // 1. $? and variable expansion
    // -----------------------------------------------------------
    printf("\n--- 1. $? and EXPANSION ---\n");

    // Bash returns 2 for missing file
    execute_and_check(&data, "ls file_not_found_127", 2, "TEST_1.1_LS_FAIL");

    // Note: $? expansion not yet implemented
    printf("Test 1.2: echo $? (expected expansion, not yet implemented)\n");
    process_input(&data, "echo $?");

    execute_and_check(&data, "/bin/ls", 0, "TEST_1.3_LS_SUCCESS");
    printf("Test 1.4: echo $? (expected expansion, not yet implemented)\n");
    process_input(&data, "echo $?");

    execute_and_check(&data, "export MY_NAME=Minishell", 0, "TEST_1.5_SETUP_VAR");
    printf("Test 1.6: echo \"Value: $MY_NAME\" -> expect 'Value: Minishell\\n'\n");
    process_input(&data, "echo \"Value: $MY_NAME\"");
    printf("Test 1.7: echo '$MY_NAME' -> expect literal '$MY_NAME'\\n\n");
    process_input(&data, "echo '$MY_NAME'");

    // -----------------------------------------------------------
    // 2. echo -n and quotes
    // -----------------------------------------------------------
    printf("\n--- 2. ECHO ---\n");
    printf("Test 2.1: echo -n -n Hello -> expect 'Hello' no newline\n");
    process_input(&data, "echo -n -n Hello");
    printf("\nTest 2.2: echo '' and echo \"\" -> expect newline twice\n");
    process_input(&data, "echo ''");
    process_input(&data, "echo \"\"");

    // -----------------------------------------------------------
    // 3. cd / pwd
    // -----------------------------------------------------------
    printf("\n--- 3. CD / PWD ---\n");

    execute_and_check(&data, "cd .", 0, "TEST_3.1_CD_DOT");
    check_cwd(ft_strrchr(orig_dir, '/') + 1, "TEST_3.1.1_CHECK_PWD_DOT");

    execute_and_check(&data, "cd ..", 0, "TEST_3.2_CD_PARENT");

    execute_and_check(&data, "cd", 0, "TEST_3.3_CD_HOME_NO_ARG");
    if (home_dir)
        check_env_value(&data, "PWD", home_dir, "TEST_3.3.1_CHECK_PWD_HOME");

    execute_and_check(&data, "cd /nonexistent_dir", 1, "TEST_3.4_CD_FAIL");

    printf("Test 3.5: pwd -> visual check\n");
    process_input(&data, "pwd");

    // -----------------------------------------------------------
    // 4. export / unset / env
    // -----------------------------------------------------------
    printf("\n--- 4. EXPORT / UNSET / ENV ---\n");
    execute_and_check(&data, "export MY_NEW_VAR=42", 0, "TEST_4.1_EXPORT_CREATE");
    check_env_value(&data, "MY_NEW_VAR", "42", "TEST_4.1.1_CHECK_EXPORT");

    execute_and_check(&data, "export NO_VAL", 0, "TEST_4.2_EXPORT_NO_VALUE");
    printf("Test 4.2.1: export (show NO_VAL)\n");
    process_input(&data, "export");

    // Restore PATH after test to keep env working
    execute_and_check(&data, "export PATH=/usr/bin:/bin:/usr/local/bin", 0, "TEST_4.3_RESTORE_PATH");

    printf("Test 4.4: env (visual check)\n");
    process_input(&data, "env");

    // -----------------------------------------------------------
    // 5. exit (mocked)
    // -----------------------------------------------------------
    printf("\n--- 5. EXIT (mocked) ---\n");
    execute_and_check(&data, "exit 42 1", 1, "TEST_5.1_EXIT_TOO_MANY");
    printf("Test 5.2: exit abc (should call mock_exit(2))\n");
    process_input(&data, "exit abc");
    printf("Test 5.3: exit (should call mock_exit with last status)\n");
    process_input(&data, "exit");

    // -----------------------------------------------------------
    // Cleanup
    // -----------------------------------------------------------
    chdir(orig_dir);
    free_shell_data(&data);

    printf(COLOR_BLUE "\n=== TESTS FINISHED ===\n" COLOR_RESET);
    return (0);
}
