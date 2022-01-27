#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <ucontext.h>
#include <stdlib.h>
#include <stdint.h>

extern void test_stack_overflow(void);

static void handler(int sig, siginfo_t *info, void *cntxt)
{
	int i = 0;
	int *paddr = NULL;
	
	paddr = (int *)((((ucontext_t *)(cntxt))->uc_mcontext).gregs[REG_RSP]);
	printf("Waoh, caught signal %s, Stack addr = %p\n",strsignal(sig), paddr);
	
	for (i = -20; i < 20; i++) {
		printf("0x%x\n", *(paddr + i));
	}
	sleep(2);

    _exit(EXIT_FAILURE);
}

void create_alt_stack(void)
{
	stack_t sigstack;
	
	memset(&sigstack, 0x00, sizeof(stack_t));
    sigstack.ss_sp = malloc(SIGSTKSZ);
    if( !sigstack.ss_sp)
    {
        printf("Err: malloc error\n");
        exit(EXIT_FAILURE);
    }
    sigstack.ss_size = SIGSTKSZ;
    sigstack.ss_flags = 0;
    if(sigaltstack(&sigstack, NULL) == -1)
    {
        printf("Err: sigaltstack error\n");
        exit(EXIT_FAILURE);
    }

    printf("Now the alternate signal stack is successfully allocated\n");
    printf("The address of signal stack is : %10p - %10p\n",sigstack.ss_sp,(char*)sbrk(0)-1);
	
	return;
}

void sig_action(void)
{
	struct sigaction act;
	
	memset(&act, 0x00, sizeof(struct sigaction));
	
	act.sa_flags = SA_SIGINFO | SA_NODEFER | SA_ONSTACK;
	sigemptyset(&act.sa_mask);
	act.sa_sigaction = handler;			//须挂在sa_sigaction  不应该挂在sa_handler
	sigaction(SIGSEGV, &act, NULL);
	sigaction(SIGSEGV, &act, NULL);
	
	return;
}

void stack_funcBx(void)
{
	test_stack_overflow();
	
	return;
}

void stack_funcAx(void)
{
	stack_funcBx();
	
	return;
}

int main(int argc, char *argv[])
{	
	create_alt_stack();
	sig_action();
	sleep(8);
	stack_funcAx();
	
	return 0;	
}