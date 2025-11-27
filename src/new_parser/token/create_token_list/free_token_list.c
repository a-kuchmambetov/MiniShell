#include "create_token_list.h"

void free_token_list(t_token_list *tkn_li)
{
	t_token_node *current;
	t_token_node *next;

	if (tkn_li)
		current = tkn_li->head;
	else
		current = NULL;
	while (current)
	{
		next = current->next;
		my_free(current->value);
		my_free(current);
		current = next;
	}
	my_free(tkn_li);
}