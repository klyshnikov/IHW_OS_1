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

int main(int argc, char* argv[]) {
    int smallLetters = 0;
    int bigLetters = 0;

    const int file_buf_size = 2048;
    const int mes_1_size = 100;
    const int mes_2_size = 100;
    unsigned long int two_numbers_size = 100;

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
        fd_file = open(argv[1], O_RDONLY);

        int read_bytes = read(fd_file, file_buffer, file_buf_size);

        file_buffer[read_bytes] = '\0';

        // Send in pipe
        close(fd_first[0]);

        int tmp = sizeof(file_buffer);
        size_1 = write(fd_first[1], file_buffer, sizeof(file_buffer));

        close(fd_first[1]);
    } else {
        int result_2 = fork();

        if (result_2 > 0) {
            //Process 2

            close(fd_first[1]);

            for (int i = 0; i<100; ++i) {
                message_1_buffer[i] = ' ';
            }

            size_1 = read(fd_first[0], message_1_buffer, mes_1_size);


            for (int i = 0; i<mes_1_size; ++i) {
                int currentLetter = message_1_buffer[i];
                if (message_1_buffer[i] == '\0') {break;}
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


            strncat(number1, space, 1);
            strncat(number1, number2, 10);
            //printf("%lu", sizeof(number1));
            two_numbers_size = sizeof(number1);

            size_1 = write(fd_second[1], number1, 50);
            
        } else {
            // Process 3
            sleep(1);
            close(fd_second[1]);

            char new_buffer[100];

            //sleep(1);

            size_1 = read(fd_second[0], new_buffer, 100);
            for (int i = 0; i<100; ++i) {
                if (new_buffer[i] < 48 || new_buffer[i] > 122) {
                    new_buffer[i] = ' ';
                }
            }

            //sleep(1);
            //puts(new_buffer);

            //puts(new_buffer);

            fd_file = open(argv[2], O_WRONLY, 0666);

            int writeWords1 = write(fd_file, "Small letters (first), big letters (second): ", 45);
            int write_bytes = write(fd_file, new_buffer, 100);

            //close(fd_file);

        }

    }
}
