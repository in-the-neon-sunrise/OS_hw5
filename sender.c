#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pid_t receiver_pid = 0;
int bit_cnt = 1;
int num;

void send_bit(int bit) {
  printf("Sent bit %d\n", bit);
  if (bit) {
    kill(receiver_pid, SIGUSR1); // Передача бита 1
  } else {
    kill(receiver_pid, SIGUSR2); // Передача бита 0
  }
}

void signal_handler(int signum) {
  (void)signal(SIGUSR1, signal_handler);
  if (receiver_pid == 0) return;
  send_bit((num & (1 << bit_cnt)) >> bit_cnt);
  ++bit_cnt;
  if (bit_cnt == 32) {
    printf("The number is sent completely: %d\n", num);
    kill(receiver_pid, SIGTERM);
    exit(0); // Завершение программы, когда все биты переданы
  }
  usleep(1000);  // Задержка между передачей битов
}

int main() {

  // Установка пользовательских обработчиков сигналов
  (void)signal(SIGUSR1, signal_handler);
  printf("Sender PID = %d\n", getpid());

  printf("Input receiver PID: ");
  scanf("%d", &receiver_pid);
  printf("Input integer: ");
  scanf("%d", &num);
  send_bit(num & 1);

  // Ожидание завершения передачи
  while (1);
  return 0;
}
