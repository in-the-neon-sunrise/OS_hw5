#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int received_number = 0;
int received_bits = 0;
pid_t sender_pid;

void get_bit(int signum) {
  if (signum == SIGUSR1) {
    received_number |= (1 << received_bits); // единичка
    printf("Received bit 1\n");
    (void)signal(SIGUSR1, get_bit); // bit 1
  } else if (signum == SIGUSR2) {
    printf("Received bit 0\n");
    (void)signal(SIGUSR2, get_bit); // bit 0
  }
  ++received_bits;
  usleep(1000);  // Задержка между передачей битов
  kill(sender_pid, SIGUSR1);
}

void finish(int signum) {
  printf("Received number: %d\n", received_number); // Вывод результата
  exit(0);
}

int main() {
  // Установка пользовательских обработчиков сигналов
  (void)signal(SIGUSR1, get_bit); // Сигнал для получения бита 1
  (void)signal(SIGUSR2, get_bit); // Сигнал для получения бита 0
  (void)signal(SIGTERM, finish);  // Завершение программы
  
  printf("Receiver PID = %d\n", getpid());
  printf("Enter sender PID: ");
  scanf("%d", &sender_pid);

  // Ожидание завершения передачи
  while (1);
  return 0;
}
