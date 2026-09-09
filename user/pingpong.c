#include "kernel/types.h"
#include "user/user.h"

int main(void) {
  int p1[2], p2[2];
  char buffer[1];

  if (pipe(p1) < 0 || pipe(p2) < 0) {
    fprintf(2, "pipe failed!");
    exit(1);
  }

  int child_pid = fork();

  if (child_pid < 0) {
    fprintf(2, "fork failed!");
    exit(1);
  }

  if (child_pid == 0) {  // 子进程
    close(p1[1]);
    close(p2[0]);

    read(p1[0], buffer, 1);  // 读取
    close(p1[0]);
    printf("child = %d: received ping\n", getpid());

    buffer[0] = 'p';
    write(p2[1], buffer, 1);
    close(p2[1]);
    exit(0);
  } else {  // 父进程
    close(p1[0]);
    close(p2[1]);

    buffer[0] = 'c';
    write(p1[1], buffer, 1);
    close(p1[1]);
    read(p2[0], buffer, 1);
    close(p2[0]);
    printf("parent = %d: received pong\n", getpid());
    wait(0);
    exit(0);
  }
}
