#include "parse_input_utils.h"

static int merge_splited(char ***dst_a, char **src_arr)
{
	const int src_size = arr_size(src_arr);
	t_polish_data dt;

	dt = (t_polish_data){0};
	if (!dst_a || !*dst_a)
		return (1);
	if (src_arr == NULL)
	{
		append_arr_f(dst_a, ft_strdup(""));
		return (free_str_arr(src_arr), 0);
	}
	if (is_delim(src_arr[0]) && protect_delim(src_arr))
		return (free_str_arr(src_arr), 1);
	dt.to = find_space_index(src_arr);
	if (append_arr_f(dst_a, join_from_to(src_arr, dt.from, dt.to)))
		return (free_str_arr(src_arr), 1);
	dt.from = dt.to;
	if (dt.from + 1 < src_size)
	{
		if (append_arr_f(dst_a, ft_strdup(" ")) ||
			append_arr_f(dst_a, join_from_to(src_arr, dt.from + 1, src_size)))
			return (free_str_arr(src_arr), 1);
	}
	free_str_arr(src_arr);
	return (0);
}

static int final_merge_util(char ***dst_a, char **src_arr, t_polish_data *dt)
{
	int code;

	code = 0;
	dt->to = dt->i;
	if (dt->from == dt->to)
		code = append_arr(dst_a, src_arr[dt->i]);
	else
	{
		code = append_arr_f(dst_a, join_from_to(src_arr, dt->from, dt->to));
		if (code)
			return (1);
		code = append_arr(dst_a, src_arr[dt->i]);
	}
	dt->from = dt->i + 1;
	if (code)
		return (1);
	return (0);
}

static int final_merge(char ***dst_a, char **src_arr)
{
	const int src_size = arr_size(src_arr);
	t_polish_data dt;

	dt = (t_polish_data){0};
	dt.arr = ft_calloc(sizeof(char *), 1);
	if (dt.arr == NULL)
		return (1);
	while (dt.i < src_size)
	{
		if (is_delim(src_arr[dt.i]))
		{
			if (final_merge_util(&dt.arr, src_arr, &dt))
				return (free_str_arr(dt.arr), 1);
		}
		dt.i++;
	}
	if (dt.from < src_size)
	{
		if (append_arr_f(&dt.arr, join_from_to(src_arr, dt.from, src_size)))
			return (free_str_arr(dt.arr), 1);
	}
	free_str_arr(*dst_a);
	free_str_arr(src_arr);
	*dst_a = dt.arr;
	return (0);
}

static char **split_space(t_polish_data dt)
{
	char **new_arr;
	char *trimmed_str;
	int code;

	new_arr = ft_calloc(sizeof(char *), 1);
	if (!new_arr)
		return (NULL);
	while (dt.arr[dt.i])
	{
		if (is_delim(dt.arr[dt.i]) && !is_quoted(dt.arr[dt.i]))
			code = append_arr(&new_arr, dt.arr[dt.i]);
		else if (is_quoted(dt.arr[dt.i]))
			code = append_arr_f(&new_arr, trim_quote(dt.arr[dt.i]));
		else
		{
			trimmed_str = ft_strtrim(dt.arr[dt.i], " ");
			code = !trimmed_str ||
				   merge_splited(&new_arr, split_input_str_r(trimmed_str));
			my_free(trimmed_str);
		}
		if (code)
			return (free_str_arr(new_arr), NULL);
		dt.i++;
	}
	return (new_arr);
}

int polish_input_arr(char ***arr)
{
	char **new_arr;
	t_polish_data dt;

	dt = (t_polish_data){0};
	if (!arr || !*arr)
		return (1);
	dt.arr = ft_calloc(sizeof(char *), 1);
	if (dup_arr(&dt.arr, *arr))
		return (free_str_arr(dt.arr), 1);
	new_arr = split_space(dt);
	if (!new_arr)
		return (free_str_arr(dt.arr), 1);
	free_str_arr(dt.arr);
	dt.arr = ft_calloc(sizeof(char *), 1);
	if (dup_arr(&dt.arr, new_arr))
		return (free_str_arr(new_arr), free_str_arr(dt.arr), 1);
	if (final_merge(&new_arr, dt.arr))
		return (free_str_arr(new_arr), free_str_arr(dt.arr), 1);
	free_str_arr(*arr);
	*arr = new_arr;
	return (0);
}
