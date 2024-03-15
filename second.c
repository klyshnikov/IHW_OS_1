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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

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
    int fd_first;
    int fd_second;

    size_t size_1, size_2;
    char  file_buffer[file_buf_size];
    char message_1_buffer[mes_1_size];
    char message_2_buffer[mes_2_size];

    char* name_1_2 = "1-2.fifo";
    char* name_2_3 = "2-3.fifo";

    char numbers_buffer[100];


    (void)umask(0);
    mknod(name_1_2, S_IFIFO | 0666, 0);
    mknod(name_2_3, S_IFIFO | 0666, 0);

    int result_1 = fork();

    if (result_1 > 0) {
        // Process 1
        fd_file = open("/home/misha/CppProj/IDZ1/input", O_RDONLY);

        int read_bytes = read(fd_file, file_buffer, file_buf_size);

        file_buffer[read_bytes] = '\0';

        fd_first = open(name_1_2, O_WRONLY);

        int tmp = sizeof(file_buffer);
        size_1 = write(fd_first, file_buffer, sizeof(file_buffer));

        close(fd_first);
    } else {
        int result_2 = fork();

        if (result_2 > 0) {
            //Process 2

            fd_first = open(name_1_2, O_RDONLY);

            size_1 = read(fd_first, message_1_buffer, mes_1_size);

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

            close(fd_first);

            fd_second = open(name_2_3, O_WRONLY);

            sleep(1);
            size_1 = write(fd_second, number1, sizeof(number1));
            close(fd_second);

        } else {
            // Process 3

            fd_second = open(name_2_3, O_RDONLY);

            size_1 = read(fd_second, numbers_buffer, 3);
            puts(numbers_buffer);

            fd_file = open("/home/misha/CppProj/IDZ1/output2", O_WRONLY, 0666);

            int write_bytes = write(fd_file, numbers_buffer, 3);
            int g = write(fd_file, "qwe", 3);


            close(fd_second);
        }

    }
}
