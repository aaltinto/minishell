#include "minishell.h"

void    sig_c(int sig)
{
    ft_putchar_fd("\n", 1);
    if (!g_l)
    {
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

void    sig_backslash(int sig)
{
    ft_putstr_fd("Quit: ", 2);
	ft_putnbr_fd(sig, 2);
	ft_putchar_fd('\n', 2);
}

void    init_signals()
{
    g_l = 0;
    signal(SIGINT, sig_c);
    signal(SIGQUIT, SIG_IGN);
}

void    init_signals2()
{
    g_l = 1;
    signal(SIGINT, sig_c);
    signal(SIGQUIT, sig_backslash);
}