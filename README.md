# 42 Cursus

## Philosophers 😏

The philosophers project challenges you to simulate the dining philosophers problem using parallel threads.  
The problem involves a group of philosophers sitting around a dining table, each thinking, eating and sleeping. To eat, a philosopher must use two forks, the one on their left and the one on their right. You have to avoid deadlocks and data races.  
In this project you explore concurrent computing and synchronization techniques. Each philosopher represents a concurrent thread, and the forks represent shared resources.  

## How to use:

1. Clone the repository:
```c
git clone https://github.com/aldferna/Philosophers.git
```
2. Compile the project:
```c
make
```
3. Run the program with the following syntax:
```c
./philos [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [optional: number_of_times_each_philosopher_must_eat]
```

## Visualizer
You can visualize the result copy-pasting the result [here](https://nafuka11.github.io/philosophers-visualizer/).  

