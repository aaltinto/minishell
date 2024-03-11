#include "minishell.h"
#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>
#include "libft/libft.h"

void	sig_c(int sig)
{
	if (!g_l)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (g_l == 42)
	{
		printf("\n");
		exit(EXIT_FAILURE);
	}
		
}

void	sig_backslash(int sig)
{
	printf("Quit: 3\n");
}

void	init_signals(void)
{
	g_l = 0;
	signal(SIGINT, sig_c);
	signal(SIGQUIT, SIG_IGN);
}

void	init_signals2(void)
{
	g_l = 1;
	signal(SIGINT, sig_c);
	signal(SIGQUIT, sig_backslash);
}