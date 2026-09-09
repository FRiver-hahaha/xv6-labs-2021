#include "kernel/types.h"
#include "user/user.h"

void prime(int left_fd) {
  int right_pipe[2];
  int first_num;
  int num;

  if (read(left_fd, &first_num, 4) != 4) {
    close(left_fd);
    exit(0);
  }

  printf("prime %d\n", first_num);

  pipe(right_pipe);

  int child_pid = fork();
  if (child_pid == 0) {    // 子进程
    close(right_pipe[1]);  // 关闭写端
    prime(right_pipe[0]);  // 递归
    close(right_pipe[0]);  // 递归结束，关闭读端
    exit(0);
  } else {  // 父进程
    close(right_pipe[0]);

    while (read(left_fd, &num, 4) == 4) {
      if (num % first_num != 0) {
        write(right_pipe[1], &num, 4);
      }
    }
    close(left_fd);
    close(right_pipe[1]);

    wait(0);
    exit(0);
  }
}

int main(void) {
  int input_pipe[2];
  pipe(input_pipe);

  int child_pid = fork();

  if (child_pid == 0) {  // 子进程
    close(input_pipe[1]);
    prime(input_pipe[0]);
    close(input_pipe[0]);
    exit(0);
  } else {  // 父进程
    for (int i = 2; i <= 35; ++i) {
      write(input_pipe[1], &i, 4);
    }

    close(input_pipe[1]);

    wait(0);
    exit(0);
  }
}
