#include "ee25b052_ee1103.h"

int **alloc_ising_grid(int N) {
    int **grid = malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++)
        grid[i] = malloc(N * sizeof(int));
    return grid;
}

void free_ising_grid(int **grid, int N) {
    for (int i = 0; i < N; i++)
        free(grid[i]);
    free(grid);
}

void initialize_random_spins(int **M, int N) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            M[i][j] = (rand() % 2) ? 1 : -1;
}

int neighbor_sum(int **M, int i, int j, int N) {
    int up    = M[(i - 1 + N) % N][j];
    int down  = M[(i + 1) % N][j];
    int left  = M[i][(j - 1 + N) % N];
    int right = M[i][(j + 1) % N];
    return up + down + left + right;
}

void metropolis_sweep(int **Mold, int **Mnew, int N, double J, double T) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int spin = Mold[i][j];
            int sum_neigh = neighbor_sum(Mold, i, j, N);
            double dE = 2.0 * J * spin * sum_neigh;
            if (dE <= 0.0) {
                Mnew[i][j] = -spin;
            } else {
                double r = (double)rand() / RAND_MAX;
                if (r < exp(-dE / T))
                    Mnew[i][j] = -spin;
                else
                    Mnew[i][j] = spin;
            }
        }
    }
}

double calculate_magnetization(int **M, int N) {
    int sum = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            sum += M[i][j];
    return (double)sum / (N * N);
}

void ising_simulation(int N, double J, double Tstart, double Tstop,
                      double dT, int max_iterations, double convergence_threshold) {
    FILE *fp = fopen("ising_output.txt", "w");
    if (!fp) {
        fprintf(stderr, "Error opening output file\n");
        return;
    }

    int **Mold = alloc_ising_grid(N);
    int **Mnew = alloc_ising_grid(N);
    initialize_random_spins(Mold, N);

    for (double T = Tstart; T <= Tstop; T += dT) {
        double Mavg_prev = 0.0;
        int steps = 0;
        for (int iter = 0; iter < max_iterations; iter++) {
            metropolis_sweep(Mold, Mnew, N, J, T);
            for (int i = 0; i < N; i++)
                for (int j = 0; j < N; j++)
                    Mold[i][j] = Mnew[i][j];
            double Mavg = calculate_magnetization(Mold, N);
            if (fabs(Mavg - Mavg_prev) < convergence_threshold) {
                steps = iter;
                break;
            }
            Mavg_prev = Mavg;
        }
        fprintf(fp, "%lf %lf %d\n", T, Mavg_prev, steps);
        printf("%lf\t%lf\t%d\n", T, Mavg_prev, steps);
    }

    fclose(fp);
    free_ising_grid(Mold, N);
    free_ising_grid(Mnew, N);
}

int main(int argc, char *argv[]) {
    if (argc != 8) {
        printf("Usage: %s N J Tstart Tstop dT max_iterations convergence_threshold\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    double J = atof(argv[2]);
    double Tstart = atof(argv[3]);
    double Tstop = atof(argv[4]);
    double dT = atof(argv[5]);
    int max_iterations = atoi(argv[6]);
    double threshold = atof(argv[7]);

    srand(time(NULL));
    ising_simulation(N, J, Tstart, Tstop, dT, max_iterations, threshold);
    return 0;
}
