#include <iostream>
#include <unistd.h>    // fork, pipe, read, write, getpid
#include <sys/wait.h>  // wait
#include <cstdlib>     // rand, srand
#include <ctime>       // time

using namespace std;

int main() {
    const int SIZE = 20;
    int arr[SIZE];

    // Seed random number generator
    srand(time(nullptr));

    // Fill array with random numbers (range 1–100)
    cout << "Array elements: ";
    for (int i = 0; i < SIZE; i++) {
        arr[i] = rand() % 100 + 1;
        cout << arr[i] << " ";
    }
    cout << "\n\n";

    // Create pipe
    int fd[2];
    if (pipe(fd) == -1) {
        cerr << "Pipe failed!" << endl;
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Fork failed!" << endl;
        return 1;
    }

    if (pid == 0) {
        // ===== Child Process =====
        close(fd[0]); // close read end

        int minChild = arr[SIZE / 2];
        for (int i = SIZE / 2; i < SIZE; i++) {
            if (arr[i] < minChild) {
                minChild = arr[i];
            }
        }

        cout << "Child Process (PID " << getpid()
             << ") found minimum in 2nd half: " << minChild << endl;

        // Send result to parent
        write(fd[1], &minChild, sizeof(minChild));
        close(fd[1]);

        return 0; // Child exits
    } else {
        // ===== Parent Process =====
        close(fd[1]); // close write end

        int minParent = arr[0];
        for (int i = 0; i < SIZE / 2; i++) {
            if (arr[i] < minParent) {
                minParent = arr[i];
            }
        }

        int minChild;
        read(fd[0], &minChild, sizeof(minChild));
        close(fd[0]);

        cout << "Parent Process (PID " << getpid()
             << ") found minimum in 1st half: " << minParent << endl;

        // Wait for child to finish
        wait(nullptr);

        // Overall minimum
        int overallMin = (minParent < minChild) ? minParent : minChild;
        cout << "\nOverall minimum value in array: " << overallMin << endl;
    }

    return 0;
}
