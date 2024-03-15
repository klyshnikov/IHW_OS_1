#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include <string.h>

int isLetterSmall(int letter) {
    //97-122
    if (letter <= 122 && letter >= 97) {
        return 1;
    }
    return 0;
}

int isLetterBig(int letter) {
    //65-90
    if (letter <= 90 && letter >= 65) {
        return 1;
    }
    return 0;
}

int getDigitNumber(int number) {
    return 1;
}

int main() {
    int smallLetters = 0;
    int bigLetters = 0;

    const int file_buf_size = 2048;
    const int mes_1_size = 14;
    const int mes_2_size = 100;
    unsigned long int two_numbers_size = 0;

    int fd_file;
    int fd_first[2];
    int fd_second[2];

    size_t size_1, size_2;
    char  file_buffer[file_buf_size];
    char message_1_buffer[mes_1_size];
    char message_2_buffer[mes_2_size];

    char numbers_buffer[100];

    pipe(fd_first);
    pipe(fd_second);

    int result_1 = fork();

    if (result_1 > 0) {
        // Process 1
        fd_file = open("/home/misha/CppProj/IDZ1/input", O_RDONLY);

        int read_bytes = read(fd_file, file_buffer, file_buf_size);

        file_buffer[read_bytes] = '\0';

        // Send in pipe
        close(fd_first[0]);

        int tmp = sizeof(file_buffer);
        size_1 = write(fd_first[1], file_buffer, sizeof(file_buffer));

        close(fd_first[1]);

        close(fd_second[1]);

        sleep(1);
        size_1 = read(fd_second[0], numbers_buffer, 3);
        puts(numbers_buffer);

        fd_file = open("/home/misha/CppProj/IDZ1/output3", O_WRONLY, 0666);

        int write_bytes = write(fd_file, numbers_buffer, 3);
        int g = write(fd_file, "qwe", 3);
    } else {
        close(fd_first[1]);

        size_1 = read(fd_first[0], message_1_buffer, mes_1_size);

        for (int i = 0; i<mes_1_size; ++i) {
            int currentLetter = message_1_buffer[i];
            if (isLetterSmall(currentLetter)) {
                smallLetters++;
            }

            if (isLetterBig(currentLetter)) {
                bigLetters++;
            }
        }

        char number1[(int) (getDigitNumber(smallLetters) + 2)];
        char number2[(int) (getDigitNumber(bigLetters) + 2)];
        char space[1] = " ";

        snprintf(number1, sizeof(number1), "%d", smallLetters);
        snprintf(number2, sizeof(number2), "%d", bigLetters);

        //puts(number1);
        //puts(number2);

        strncat(number1, space, 10);
        strncat(number1, number2, 10);
        //printf("%lu", sizeof(number1));
        two_numbers_size = sizeof(number1);

        //sleep(1);
        size_1 = write(fd_second[1], number1, sizeof(number1));
    }
}
