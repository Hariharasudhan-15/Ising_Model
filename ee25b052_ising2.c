//ee25b052,ee25b051,ee25b154
//chatgpt,gemini
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


//sum of neighbour spins
int sumofspins(int **lattice, int N, int x, int y) {
    int sum = 0;
    sum += lattice[(x - 1 + N) % N][y];
    sum += lattice[(x + 1) % N][y];
    sum += lattice[x][(y - 1 + N) % N];
    sum += lattice[x][(y + 1) % N];
    return sum;
}
//calculating mavg
double Mavg(int **lattice, int N) {
    int s = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            s += lattice[i][j];
    return (double)s / (N * N);
}
//creating a dynamic memory for an array of pointers
int **allocate_lattice(int N) {
    int **lattice = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++)
        lattice[i] = (int *)malloc(N * sizeof(int));
    return lattice;
}
//to free up after using malloc
void free_lattice(int **lattice, int N) {
    for (int i = 0; i < N; i++)
        free(lattice[i]);
    free(lattice);
}


int main(int argc, char **argv) {
    if (argc != 8) {
        printf("Usage: %s <N> <J> <Tstart> <Tstop> <dT> <P> <dMavg>\n", argv[0]);
        return 1;
    }

    int N=atoi(argv[1]);
    double J=atof(argv[2]);
    double Tstart=atof(argv[3]);
    double Tstop=atof(argv[4]);
    double dT=atof(argv[5]);
    int P=atoi(argv[6]);
    double dMavg=atof(argv[7]);

    srand(time(0));
    //array of pointers
    int **lattice = allocate_lattice(N);
//random generation
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            lattice[i][j] = (rand() % 2) ? 1 : -1;

    printf("# T/J      M_avg\n"); // header 

    for (double T = Tstart; T >= Tstop; T -= dT) {
        double pre_Mavg = 100.0;
        double new_Mavg = Mavg(lattice, N);

        for (int k = 0; k < P; k++) {
            for (int step = 0; step < N * N; step++) {
                int x = rand() % N;
                int y = rand() % N;
                int s = sumofspins(lattice, N, x, y);
                int spin = lattice[x][y];
                double deltaE = 2.0 * J * spin * s;

                if (deltaE < 0 || ((double)rand() / RAND_MAX) < exp(-deltaE / T)) {
                    lattice[x][y] = -spin;
                }
            }

            pre_Mavg = new_Mavg;
            new_Mavg = Mavg(lattice, N);
            if (fabs(new_Mavg - pre_Mavg) < dMavg) {
                break;
            }
        }
        
        printf("%.4f %.5f\n", T / J, Mavg(lattice, N));
    }

    free_lattice(lattice, N);
    return 0;
}