# 2D Ising Model Simulation (Metropolis-Hastings Algorithm)

This repository contains a high-performance **C implementation** of a 2D Ising Model. It uses **Markov Chain Monte Carlo (MCMC)** methods to simulate magnetic dipole interactions and observe phase transitions in a ferromagnetic system.



## 🛠️ Technical Features
* **Dynamic Memory:** Uses pointer-to-pointer (`int **`) allocation to handle user-defined lattice sizes ($N \times N$) efficiently.
* **Periodic Boundary Conditions:** Implemented using modular arithmetic `(x + 1) % N` to eliminate edge effects and simulate an infinite surface.
* **Convergence Monitoring:** Includes a `dMavg` threshold to detect when the system has reached a steady state (equilibrium) before moving to the next temperature step.
* **Command Line Interface:** Accepts 7 specific parameters for granular control over the simulation.

## 📉 Mathematical Foundation
The system is governed by the **Hamiltonian**:

$$H = -J \sum_{\langle i,j \rangle} s_i s_j$$

### The Metropolis Algorithm
To reach thermodynamic equilibrium, the code implements the following stochastic logic:
1.  **Selection:** A random spin is chosen from the lattice.
2.  **Energy Calculation:** The change in energy ($\Delta E$) for flipping that spin is calculated based on its 4 nearest neighbors.
3.  **Acceptance Criteria:** * If $\Delta E \leq 0$, the flip is **accepted**.
    * If $\Delta E > 0$, the flip is accepted with probability $P = e^{-\frac{\Delta E}{k_B T}}$.



## 🚀 How to Use

### 1. Compilation
Use `gcc` with the math library flag `-lm`:
```bash
gcc -o ising_sim ee25b052_ising2.c -lm
