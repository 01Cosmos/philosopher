# philo — Dining Philosophers

A multithreaded simulation of the classic **Dining Philosophers Problem**, built in C with POSIX threads and mutexes as part of the 42 curriculum.

---

## The Problem

*N* philosophers sit around a circular table. Each philosopher alternates between three states: **thinking**, **eating**, and **sleeping**. A fork is placed between every two adjacent philosophers — so there are exactly *N* forks. To eat, a philosopher must pick up **both** the fork to their left and the fork to their right. If a philosopher goes too long without eating, they **die**. The simulation ends when a philosopher dies, or when every philosopher has eaten a required number of meals.

---

## Usage

```bash
make
./philo <num_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [num_meals]
```

| Argument            | Description                                                               |
|---------------------|---------------------------------------------------------------------------|
| `num_philosophers`  | Number of philosophers (and forks). Max: 200.                             |
| `time_to_die`       | Time in ms a philosopher can go without eating before dying.              |
| `time_to_eat`       | Time in ms it takes a philosopher to eat.                                 |
| `time_to_sleep`     | Time in ms a philosopher sleeps after eating.                             |
| `num_meals`         | *(Optional)* Simulation stops once every philosopher has eaten this many times. |

All values must be **positive integers**.

### Examples

```bash
# 5 philosophers, classic setup — one will eventually die
./philo 5 800 200 200

# 4 philosophers, each must eat at least 3 times before the sim ends
./philo 4 410 200 200 3

# Edge case: single philosopher — always dies (only one fork)
./philo 1 800 200 200
```

---

## How It Works

Each philosopher runs in its own **thread**. Forks are protected by **mutexes** — one mutex per fork (left fork is owned by each philosopher; right fork is borrowed from the neighbor).

**Deadlock prevention** is handled by fork-ordering: even-numbered philosophers pick up their left fork first, odd-numbered philosophers pick up their right fork first.

A dedicated **monitor thread** watches for two conditions:
- A philosopher has exceeded `time_to_die` since their last meal → prints `died` and stops the simulation.
- All philosophers have eaten `num_meals` times → stops the simulation cleanly.

### Output format

Every state change is timestamped in milliseconds from simulation start:

```
timestamp_ms  philosopher_id  action
```

Possible actions: `has taken a fork`, `is eating`, `is sleeping`, `is thinking`, `died`.

No output is printed after a death event (except the death message itself).

---

## Build

```bash
make        # build
make clean  # remove objects
make fclean # remove objects + binary
make re     # fclean + build
```

---

## Notes

- Tested on Linux (Ubuntu). macOS behavior may differ slightly due to `usleep` precision.
- Use [valgrind](https://valgrind.org/) with `--tool=helgrind` or `--tool=drd` to check for data races.
- The single-philosopher case is handled explicitly: the philosopher picks up one fork, waits `time_to_die` ms, then dies — since a second fork never becomes available.
