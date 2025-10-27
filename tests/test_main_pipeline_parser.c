#include "../main.h"
#include "../src/built_in_commands/builtins.h"

// ===========================================================
// Helper: Print parsed command list (for visual verification)
// ===========================================================
void print_cmd_list(t_cmd_list *list, const char *test_name)
{
    t_cmd_node *current = list->first;
    int i = 0;

    printf(COLOR_BLUE "--- %s: Parsed List ---\n" COLOR_RESET, test_name);
    while (current)
    {
        printf("  [CMD %d]\n", i++);
        printf("    Command: %s\n", current->cmd ? current->cmd : "(NULL)");
        printf("    Args:    %s\n", current->args ? current->args : "(NULL)");
        printf("    Input:   %d %s\n", current->input_redir_type,
               current->input_redir ? current->input_redir : "(-)");
        printf("    Output:  %d %s\n", current->output_redir_type,
               current->output_redir ? current->output_redir : "(-)");
        printf("    Pipe IN: %d, Pipe OUT: %d\n",
               current->is_pipe_in, current->is_pipe_out);
        current = current->next;
    }
    printf("--------------------------------\n");
}

// ===========================================================
// Run a single parsing test
// ===========================================================
int run_parsing_test(t_shell_data *data, char *input, const char *test_name)
{
    char **str_arr = NULL;
    int result = 0;

    printf(COLOR_YELLOW "\nRunning Test: %s\n" COLOR_RESET, test_name);
    printf("Input: '%s'\n", input);

    // Clear previous data before new test
    free_cmd_list(&data->cmd_list);
    data->cmd_list.first = NULL;
    data->cmd_list.len = 0;

    // Tokenization
    str_arr = split_input_str(input);
    if (!str_arr)
    {
        printf(COLOR_RED "[FAIL] %s: split_input_str failed (probably quotes)\n" COLOR_RESET, test_name);
        return (1);
    }

    // Command list creation
    result = create_cmd_list(data, str_arr);
    free_str_arr(str_arr);

    if (result == 0 && data->cmd_list.len > 0)
    {
        printf(COLOR_GREEN "[OK] %s: Parsed successfully (%d commands)\n" COLOR_RESET, test_name, data->cmd_list.len);
        print_cmd_list(&data->cmd_list, test_name);
    }
    else if (result != 0)
    {
        printf(COLOR_RED "[FAIL] %s: Parser returned error code %d\n" COLOR_RESET, test_name, result);
        result = 1;
    }
    else
    {
        printf(COLOR_RED "[FAIL] %s: 0 commands created (empty or invalid input)\n" COLOR_RESET, test_name);
        result = 1;
    }

    free_cmd_list(&data->cmd_list);
    data->cmd_list.len = 0;
    return (result);
}

// ===========================================================
// MAIN
// ===========================================================
int main(int argc, char **argv, char **envp)
{
    t_shell_data data;

    (void)argc;
    (void)argv;

    init_shell_data(&data, envp);

    printf(COLOR_MAGENTA "=== PIPELINE AND REDIRECTION PARSER TESTS ===\n" COLOR_RESET);

    // =======================================================
    // 1. PIPELINE TESTS
    // =======================================================
    run_parsing_test(&data, "ls -l | grep minish", "TEST_1.1_PIPE_SIMPLE");
    run_parsing_test(&data, "echo hello | sed 's/o/a/' | wc -c", "TEST_1.2_PIPE_COMPLEX");

    // =======================================================
    // 2. REDIRECTION TESTS
    // =======================================================
    run_parsing_test(&data, "cat < input.txt > output.txt", "TEST_2.1_REDIRECTION_BOTH");
    run_parsing_test(&data, "echo append >> log.txt", "TEST_2.2_REDIRECTION_APPEND");
    run_parsing_test(&data, "ls > \"file with spaces\"", "TEST_2.3_REDIRECTION_QUOTES");

    // =======================================================
    // 3. HERE-DOCUMENT TESTS
    // =======================================================
    run_parsing_test(&data, "cat << EOF", "TEST_3.1_HEREDOC_SIMPLE");
    run_parsing_test(&data, "grep user << 'STOP'", "TEST_3.2_HEREDOC_QUOTES");

    // =======================================================
    // 4. MIXED PIPE + REDIR
    // =======================================================
    run_parsing_test(&data, "cat < file1 | grep key > output.txt", "TEST_4.1_MIX_REDIR_PIPE");
    run_parsing_test(&data, "ls | grep a > tmp.txt | wc -l", "TEST_4.2_MIX_OUT_THEN_PIPE");

    // =======================================================
    // 5. COMPLEX ARGUMENTS AND SPACES
    // =======================================================
    run_parsing_test(&data, "   ls -l   |   grep a   > out", "TEST_5.1_SPACES");
    run_parsing_test(&data, "echo \"Hello World\" 'Minishell test'", "TEST_5.2_ARG_QUOTES");

    free_shell_data(&data);

    printf(COLOR_MAGENTA "\n=== PARSER TESTS COMPLETED ===\n" COLOR_RESET);
    return (0);
}
