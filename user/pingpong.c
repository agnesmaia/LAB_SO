#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
	int p[2];  // file descriptors for pipe
	char recv_buf[128]; // buffer para a mensagem que será recebiba

	pipe(p); // cria o pipe

	if (fork() == 0) {  // processo filho é criado
		read(p[0], recv_buf, 4); // processo filho lê o pipe
		close(p[0]); // fecha o fd

		printf("%d: received %s\n", getpid(), recv_buf);

		write(p[1], "pong", 4); // processo filho escreve no pipe
		close(p[1]);

		exit(0);
	} else {  // parent
		write(p[1], "ping", 4); // processo pai escreve no pipe
		close(p[1]); // fecha o fd

		read(p[0], recv_buf, 4); // processo pai lê o pipe
		close(p[0]);

		printf("%d: received %s\n", getpid(), recv_buf);

		exit(0);
	}
}
