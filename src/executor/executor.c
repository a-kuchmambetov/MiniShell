#include "../../main.h"
#include <errno.h>
#include <stdbool.h>

static bool is_empty_cmd(t_cmd_node *cmd)
{
    if (!cmd || !cmd->cmd)
        return (true);
    return (cmd->cmd[0] == '\0');
}

static bool is_question_command(t_cmd_node *cmd)
{
    if (!cmd || !cmd->cmd)
        return (false);
    return (ft_strncmp(cmd->cmd, "$?", 3) == 0);
}

static int count_executable(t_cmd_node *node)
{
    int count;

    count = 0;
    while (node)
    {
        if (!is_empty_cmd(node))
            count++;
        node = node->next;
    }
    return (count);
}

static t_cmd_node *find_last_executable(t_cmd_node *node)
{
    t_cmd_node *last;

    last = NULL;
    while (node)
    {
        if (!is_empty_cmd(node))
            last = node;
        node = node->next;
    }
    return (last);
}

static size_t str_arr_len(char **arr)
{
    size_t len;

    len = 0;
    if (!arr)
        return (0);
    while (arr[len])
        len++;
    return (len);
}

static void free_string_array(char **arr)
{
    if (arr)
        free_str_arr(arr);
}

static int append_token(char ***arr, size_t *count, char *token)
{
    char **new_arr;
    size_t i;

    if (!arr || !count || !token)
        return (1);
    new_arr = ft_calloc(sizeof(char *), *count + 2);
    if (!new_arr)
        return (1);
    i = 0;
    while (i < *count)
    {
        new_arr[i] = (*arr)[i];
        i++;
    }
    free(*arr);
    new_arr[*count] = token;
    *arr = new_arr;
    (*count)++;
    return (0);
}

static char **split_args(const char *args, int *errored)
{
    char **result;
    size_t count;
    size_t i;
    size_t start;
    char quote;
    char *token;

    if (errored)
        *errored = 0;
    if (!args || !args[0])
        return (NULL);
    result = NULL;
    count = 0;
    i = 0;
    while (args[i])
    {
        while (args[i] && (args[i] == ' ' || args[i] == '\t'))
            i++;
        if (!args[i])
            break;
        start = i;
        while (args[i] && args[i] != ' ' && args[i] != '\t')
        {
            if (args[i] == '\'' || args[i] == '"')
            {
                quote = args[i++];
                while (args[i] && args[i] != quote)
                    i++;
                if (args[i] == quote)
                    i++;
            }
            else
                i++;
        }
        token = ft_strndup(args + start, i - start);
        if (!token || append_token(&result, &count, token))
        {
            free(token);
            free_string_array(result);
            if (errored)
                *errored = 1;
            return (NULL);
        }
    }
    return (result);
}

static char **build_argv(t_cmd_node *cmd)
{
    char **argv;
    char **extra;
    size_t extra_len;
    size_t i;
    int errored;

    if (!cmd || !cmd->cmd)
        return (NULL);
    errored = 0;
    extra = split_args(cmd->args, &errored);
    if (errored)
        return (NULL);
    extra_len = str_arr_len(extra);
    argv = ft_calloc(sizeof(char *), extra_len + 2);
    if (!argv)
        return (free_string_array(extra), NULL);
    argv[0] = ft_strdup(cmd->cmd);
    if (!argv[0])
        return (free_string_array(extra), free_string_array(argv), NULL);
    i = 0;
    while (i < extra_len)
    {
        argv[i + 1] = ft_strdup(extra[i]);
        if (!argv[i + 1])
            return (free_string_array(extra), free_string_array(argv), NULL);
        i++;
    }
    free_string_array(extra);
    return (argv);
}

static void cleanup_heredocs(t_cmd_node *cmd)
{
    while (cmd)
    {
        if (cmd->input_redir_type == REDIR_HEREDOC && cmd->input_redir)
            delete_here_doc(cmd->input_redir);
        cmd = cmd->next;
    }
}

static int redirect_input(t_cmd_node *cmd, int prev_fd)
{
    int fd;

    fd = -1;
    if (cmd->input_redir_type == REDIR_INPUT || cmd->input_redir_type == REDIR_HEREDOC)
    {
        fd = open(cmd->input_redir, O_RDONLY);
        if (fd < 0)
            return (perror(cmd->input_redir), -1);
        if (dup2(fd, STDIN_FILENO) < 0)
            return (perror("dup2"), close(fd), -1);
        close(fd);
    }
    else if (prev_fd != -1)
    {
        if (dup2(prev_fd, STDIN_FILENO) < 0)
            return (perror("dup2"), -1);
    }
    if (prev_fd != -1)
        close(prev_fd);
    return (0);
}

static int redirect_output(t_cmd_node *cmd, int pipefd[2])
{
    int fd;
    int flags;

    fd = -1;
    if (cmd->output_redir_type == REDIR_OUTPUT || cmd->output_redir_type == REDIR_APPEND)
    {
        flags = O_WRONLY | O_CREAT;
        if (cmd->output_redir_type == REDIR_OUTPUT)
            flags |= O_TRUNC;
        else
            flags |= O_APPEND;
        fd = open(cmd->output_redir, flags, 0644);
        if (fd < 0)
            return (perror(cmd->output_redir), -1);
        if (dup2(fd, STDOUT_FILENO) < 0)
            return (perror("dup2"), close(fd), -1);
        close(fd);
        if (pipefd[1] != -1)
            close(pipefd[1]);
    }
    else if (pipefd[1] != -1)
    {
        if (dup2(pipefd[1], STDOUT_FILENO) < 0)
            return (perror("dup2"), -1);
        close(pipefd[1]);
    }
    if (pipefd[0] != -1)
        close(pipefd[0]);
    return (0);
}

static int setup_child_fds(t_cmd_node *cmd, int prev_fd, int pipefd[2])
{
    if (redirect_input(cmd, prev_fd) < 0)
        return (-1);
    if (redirect_output(cmd, pipefd) < 0)
        return (-1);
    return (0);
}

static char *resolve_command_path(t_shell_data *data, const char *command)
{
    char *full_path;
    size_t cmd_len;
    size_t dir_len;
    int i;

    if (!data->paths || !command)
        return (NULL);
    cmd_len = ft_strlen(command);
    i = 0;
    while (data->paths[i])
    {
        dir_len = ft_strlen(data->paths[i]);
        full_path = malloc(dir_len + cmd_len + 2);
        if (!full_path)
            return (NULL);
        ft_strlcpy(full_path, data->paths[i], dir_len + 1);
        ft_strlcat(full_path, "/", dir_len + 2);
        ft_strlcat(full_path, command, dir_len + cmd_len + 2);
        if (access(full_path, X_OK) == 0)
            return (full_path);
        free(full_path);
        i++;
    }
    return (NULL);
}

static void exec_external(t_shell_data *data, char **argv)
{
    char *cmd;
    char *path;
    int exit_code;

    cmd = argv[0];
    if (!cmd || cmd[0] == '\0')
    {
        free_string_array(argv);
        exit(0);
    }
    if (ft_strchr(cmd, '/'))
    {
        execve(cmd, argv, data->envp);
        exit_code = (errno == EACCES) ? 126 : 127;
        perror(cmd);
        free_string_array(argv);
        exit(exit_code);
    }
    path = resolve_command_path(data, cmd);
    if (!path)
    {
        ft_print_err("%s: command not found\n", cmd);
        free_string_array(argv);
        exit(127);
    }
    execve(path, argv, data->envp);
    exit_code = (errno == EACCES) ? 126 : 127;
    perror(cmd);
    free(path);
    free_string_array(argv);
    exit(exit_code);
}

static void child_execute(t_shell_data *data, t_cmd_node *cmd, int prev_fd, int pipefd[2])
{
    char **argv;
    int status;

    if (setup_child_fds(cmd, prev_fd, pipefd) < 0)
        exit(1);
    argv = build_argv(cmd);
    if (!argv)
        exit(1);
    if (is_builtin(cmd->cmd))
    {
        status = exec_builtin(data, argv);
        free_string_array(argv);
        exit(status);
    }
    exec_external(data, argv);
}

static bool should_run_parent_builtin(t_cmd_node *cmd, int exec_count)
{
    if (!cmd || is_empty_cmd(cmd))
        return (false);
    if (!is_builtin(cmd->cmd))
        return (false);
    if (exec_count != 1)
        return (false);
    if (cmd->is_pipe_in || cmd->is_pipe_out)
        return (false);
    return (true);
}

static int apply_parent_redirs(t_cmd_node *cmd, int saved[2])
{
    int fd;
    int flags;

    saved[0] = dup(STDIN_FILENO);
    saved[1] = dup(STDOUT_FILENO);
    if (saved[0] < 0 || saved[1] < 0)
        return (perror("dup"), -1);
    if (cmd->input_redir_type == REDIR_INPUT || cmd->input_redir_type == REDIR_HEREDOC)
    {
        fd = open(cmd->input_redir, O_RDONLY);
        if (fd < 0)
            return (perror(cmd->input_redir), -1);
        if (dup2(fd, STDIN_FILENO) < 0)
            return (perror("dup2"), close(fd), -1);
        close(fd);
    }
    if (cmd->output_redir_type == REDIR_OUTPUT || cmd->output_redir_type == REDIR_APPEND)
    {
        flags = O_WRONLY | O_CREAT;
        if (cmd->output_redir_type == REDIR_OUTPUT)
            flags |= O_TRUNC;
        else
            flags |= O_APPEND;
        fd = open(cmd->output_redir, flags, 0644);
        if (fd < 0)
            return (perror(cmd->output_redir), -1);
        if (dup2(fd, STDOUT_FILENO) < 0)
            return (perror("dup2"), close(fd), -1);
        close(fd);
    }
    return (0);
}

static void restore_parent_fds(int saved[2])
{
    if (saved[0] != -1)
    {
        dup2(saved[0], STDIN_FILENO);
        close(saved[0]);
    }
    if (saved[1] != -1)
    {
        dup2(saved[1], STDOUT_FILENO);
        close(saved[1]);
    }
}

static int execute_builtin_parent(t_shell_data *data, t_cmd_node *cmd)
{
    int saved[2];
    char **argv;
    int status;

    saved[0] = -1;
    saved[1] = -1;
    if (apply_parent_redirs(cmd, saved) < 0)
    {
        restore_parent_fds(saved);
        return (-1);
    }
    argv = build_argv(cmd);
    if (!argv)
    {
        restore_parent_fds(saved);
        return (-1);
    }
    status = exec_builtin(data, argv);
    free_string_array(argv);
    restore_parent_fds(saved);
    update_last_exit_status(data, status);
    return (status);
}

static int wait_children(pid_t *pids, int count, pid_t last_child)
{
    int status;
    int last_status;
    int i;
    pid_t pid;

    last_status = 0;
    i = 0;
    while (i < count)
    {
        status = 0;
        pid = waitpid(pids[i], &status, 0);
        if (pid == last_child)
        {
            if (WIFEXITED(status))
                last_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                last_status = 128 + WTERMSIG(status);
        }
        i++;
    }
    return (last_status);
}

int execute_command_list(t_shell_data *data)
{
    t_cmd_node *cmd;
    t_cmd_node *last_exec;
    int exec_count;
    pid_t *pids;
    int child_count;
    pid_t last_child;
    int prev_pipe_read;
    bool parent_handled_last;
    bool had_error;
    bool status_updated;
    int pipefd[2];
    int input_fd;

    if (!data)
        return (1);
    cmd = data->cmd_list.first;
    exec_count = count_executable(cmd);
    if (exec_count == 0)
        return (cleanup_heredocs(data->cmd_list.first), 0);
    last_exec = find_last_executable(cmd);
    pids = ft_calloc(sizeof(pid_t), exec_count);
    if (!pids)
        return (cleanup_heredocs(data->cmd_list.first),
                ft_print_err("Memory allocation error\n"), 1);
    child_count = 0;
    prev_pipe_read = -1;
    parent_handled_last = false;
    had_error = false;
    status_updated = false;
    last_child = -1;
    while (cmd)
    {
        bool iteration_error;

        if (is_empty_cmd(cmd))
        {
            cmd = cmd->next;
            continue;
        }
        if (is_question_command(cmd))
        {
            ft_printf("%d: command not found\n", data->last_exit_status);
            cmd = cmd->next;
            continue;
        }
        pipefd[0] = -1;
        pipefd[1] = -1;
        iteration_error = false;
        if (cmd->is_pipe_out && pipe(pipefd) < 0)
        {
            perror("pipe");
            iteration_error = true;
        }
        input_fd = prev_pipe_read;
        if (!iteration_error && should_run_parent_builtin(cmd, exec_count))
        {
            if (execute_builtin_parent(data, cmd) < 0)
            {
                iteration_error = true;
                if (pipefd[0] != -1)
                    close(pipefd[0]);
                if (pipefd[1] != -1)
                    close(pipefd[1]);
            }
            else
            {
                parent_handled_last = true;
                status_updated = true;
            }
        }
        else if (!iteration_error)
        {
            pids[child_count] = fork();
            if (pids[child_count] == 0)
                child_execute(data, cmd, input_fd, pipefd);
            if (pids[child_count] < 0)
            {
                perror("fork");
                iteration_error = true;
                if (pipefd[0] != -1)
                    close(pipefd[0]);
                if (pipefd[1] != -1)
                    close(pipefd[1]);
            }
            else
            {
                if (cmd == last_exec)
                    last_child = pids[child_count];
                child_count++;
            }
        }
        if (iteration_error)
        {
            had_error = true;
            if (input_fd != -1)
            {
                close(input_fd);
                prev_pipe_read = -1;
            }
            break;
        }
        if (input_fd != -1)
            close(input_fd);
        if (cmd->is_pipe_out)
        {
            if (pipefd[1] != -1)
                close(pipefd[1]);
            prev_pipe_read = pipefd[0];
        }
        else
        {
            if (pipefd[0] != -1)
                close(pipefd[0]);
            prev_pipe_read = -1;
        }
        cmd = cmd->next;
    }
    if (prev_pipe_read != -1)
        close(prev_pipe_read);
    if (!parent_handled_last && child_count > 0 && last_child != -1)
    {
        update_last_exit_status(data, wait_children(pids, child_count, last_child));
        status_updated = true;
    }
    else
        wait_children(pids, child_count, last_child);
    free(pids);
    cleanup_heredocs(data->cmd_list.first);
    if (had_error && !status_updated)
        update_last_exit_status(data, 1);
    return (had_error);
}
