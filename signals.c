#include "minishell.h"
#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>

void	sig_c(int sig)
{
	printf("\n");
	if (!g_l)
	{
		printf("\033[2K\r");
		fflush(stdout);
		printf("> ");
		fflush(stdout);
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