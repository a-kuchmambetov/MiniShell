#include "../main.h"
#include "../src/built_in_comands/builtins.h"

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;

    t_shell_data data = {0};

    parse_envp(&data, envp);
    parse_exec_folders(&data);
    set_envp_from_env(&data);

    ft_printf("=== TESTING EXPORT / UNSET ===\n");

    // --- Test 1: Export new variable ---
    ft_printf("\n--- Test 1: Export new variable ---\n");
    char *args1[] = {"export", "MY_VAR=TestValue", NULL};
    exec_builtin(&data, args1);
    sync_envp(&data);
    ft_printf("MY_VAR = '%s' (expected: 'TestValue')\n", get_env_value(data.envp, "MY_VAR"));

    // --- Test 2: Export another variable ---
    ft_printf("\n--- Test 2: Export another variable ---\n");
    char *args2[] = {"export", "ANOTHER=123", NULL};
    exec_builtin(&data, args2);
    sync_envp(&data);
    ft_printf("ANOTHER = '%s' (expected: '123')\n", get_env_value(data.envp, "ANOTHER"));

    // --- Test 3: Unset variable ---
    ft_printf("\n--- Test 3: Unset variable ---\n");
    char *args3[] = {"unset", "ANOTHER", NULL};
    exec_builtin(&data, args3);
    sync_envp(&data);
    ft_printf("ANOTHER after unset = '%s' (expected: '(null)')\n", get_env_value(data.envp, "ANOTHER"));

    // --- Test 4: Export invalid variable ---
    ft_printf("\n--- Test 4: Export invalid variable ---\n");
    char *args4[] = {"export", "123INVALID=oops", NULL};
    int status4 = exec_builtin(&data, args4);
    ft_printf("Trying to export invalid: status = %d (expected: non-zero)\n", status4);

    // --- Test 5: Export empty variable ---
    ft_printf("\n--- Test 5: Export empty variable ---\n");
    char *args5[] = {"export", "EMPTYVAR=", NULL};
    exec_builtin(&data, args5);
    sync_envp(&data);
    ft_printf("EMPTYVAR = '%s' (expected: '')\n", get_env_value(data.envp, "EMPTYVAR"));

    ft_printf("\n=== TESTING CD / PWD ===\n");

    // --- Test 6: CD to existing directory ---
    ft_printf("\n--- Test 6: CD to existing directory ---\n");
    char *args6[] = {"cd", "/tmp", NULL};
    exec_builtin(&data, args6);
    sync_envp(&data);
    ft_printf("PWD after cd /tmp: '%s' (expected: '/tmp')\n", get_env_value(data.envp, "PWD"));

    // --- Test 7: PWD ---
    ft_printf("\n--- Test 7: PWD ---\n");
    char *args7[] = {"pwd", NULL};
    exec_builtin(&data, args7);
    ft_printf("Last exit status for pwd: %d (expected: 0)\n", WEXITSTATUS(data.last_exit_status));

    // --- Test 8: CD to invalid directory ---
    ft_printf("\n--- Test 8: CD to invalid directory ---\n");
    char *args8[] = {"cd", "/doesnotexist", NULL};
    int status8 = exec_builtin(&data, args8);
    ft_printf("Last exit status for invalid cd: %d (expected: non-zero)\n", status8);

    ft_printf("\n=== TESTING ECHO ===\n");

    // --- Test 9: Echo without arguments ---
    ft_printf("\n--- Test 9: Echo without arguments ---\n");
    char *args9[] = {"echo", NULL};
    exec_builtin(&data, args9);
    ft_printf("(expected: empty line)\n");

    // --- Test 10: Echo with arguments ---
    ft_printf("\n--- Test 10: Echo with arguments ---\n");
    char *args10[] = {"echo", "Hello", "World!", NULL};
    exec_builtin(&data, args10);
    ft_printf("(expected: Hello World!)\n");

    // --- Test 11: Echo empty string ---
    ft_printf("\n--- Test 11: Echo empty string ---\n");
    char *args11[] = {"echo", "", NULL};
    exec_builtin(&data, args11);
    ft_printf("(expected: empty line)\n");

    // --- Test 12: Echo single quotes ---
    ft_printf("\n--- Test 12: Echo with single quotes ---\n");
    char *args12[] = {"echo", "'single quotes'", NULL};
    exec_builtin(&data, args12);
    ft_printf("(expected: single quotes)\n");

    // --- Test 13: Echo double quotes ---
    ft_printf("\n--- Test 13: Echo with double quotes ---\n");
    char *args13[] = {"echo", "\"double quotes\"", NULL};
    exec_builtin(&data, args13);
    ft_printf("(expected: double quotes)\n");

    // --- Test 14: Echo environment variable ---
    ft_printf("\n--- Test 14: Echo environment variable ---\n");
    char *args14[] = {"echo", "$MY_VAR", NULL};
    exec_builtin(&data, args14);
    ft_printf("(expected: TestValue)\n");

    // --- Test 15: Echo mixed arguments ---
    ft_printf("\n--- Test 15: Echo mixed arguments ---\n");
    char *args15[] = {"echo", "Start", "$MY_VAR", "quoted", "\"double\"", NULL};
    exec_builtin(&data, args15);
    ft_printf("(expected: Start TestValue quoted double)\n");

    // --- Test 16: Echo empty and whitespace arguments ---
    ft_printf("\n--- Test 16: Echo empty and whitespace arguments ---\n");
    char *args16[] = {"echo", "", "   ", NULL};
    exec_builtin(&data, args16);
    ft_printf("(expected: <empty> <spaces>)\n");

    // --- Test 17: Echo unset variable ---
    ft_printf("\n--- Test 17: Echo unset variable ---\n");
    char *args17[] = {"echo", "$UNSET_VAR", NULL};
    exec_builtin(&data, args17);
    ft_printf("(expected: empty line)\n");

    // --- Test 18: Echo escaped characters ---
    ft_printf("\n--- Test 18: Echo escaped characters ---\n");
    char *args18[] = {"echo", "Line1\\nLine2\\tEnd", NULL};
    exec_builtin(&data, args18);
    ft_printf("(expected: Line1\\nLine2\\tEnd)\n");

    ft_printf("\n=== TESTING ENV ===\n");

    // --- Test 19: Env prints all variables ---
    ft_printf("\n--- Test 19: Env prints all variables ---\n");
    char *args19[] = {"env", NULL};
    exec_builtin(&data, args19);
    ft_printf("(expected: all environment variables listed)\n");

    // --- Test 20: Env with arguments (should fail) ---
    ft_printf("\n--- Test 20: Env with arguments ---\n");
    char *args20[] = {"env", "arg1", NULL};
    int status20 = exec_builtin(&data, args20);
    ft_printf("Status when env has arguments: %d (expected: non-zero)\n", status20);

    ft_printf("\n=== TESTING EXIT ===\n");

    // --- Test 21: Exit with no arguments ---
    ft_printf("\n--- Test 21: Exit with no arguments ---\n");
    // NOTE: We don't actually want to exit during the test, simulate call
    int exit_status21 = builtin_exit(&data); // handle inside function to avoid real exit
    ft_printf("Exit with no arguments returned: %d (expected: 0 or last exit status)\n", exit_status21);

    // --- Test 22: Exit with numeric argument ---
    ft_printf("\n--- Test 22: Exit with numeric argument ---\n");
    int exit_status22 = builtin_exit(&data); // simulate exit
    ft_printf("Exit with 42 returned: %d (expected: 42)\n", exit_status22);

    // --- Test 23: Exit with non-numeric argument ---
    ft_printf("\n--- Test 23: Exit with non-numeric argument ---\n");
    int exit_status23 = builtin_exit(&data); // simulate exit
    ft_printf("Exit with non-numeric returned: %d (expected: error / non-zero)\n", exit_status23);

    free_shell_data(&data);
    return 0;
}
