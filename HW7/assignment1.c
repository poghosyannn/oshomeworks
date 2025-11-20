#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int response_time;
} Process;

void print_gantt(Process *procs, int n, const char *title) {
    printf("=== %s ===\n", title);
    printf("Gantt Chart: |");
    for (int i = 0; i < n; i++) {
        printf(" P%d |", procs[i].pid);
    }
    printf("\n");

    printf("PID     AT     BT     WT     TAT    RT\n");
    for (int i = 0; i < n; i++) {
        printf("%-7d %-6d %-6d %-6d %-6d %-6d\n",
               procs[i].pid,
               procs[i].arrival_time,
               procs[i].burst_time,
               procs[i].waiting_time,
               procs[i].turnaround_time,
               procs[i].response_time);
    }
}

int cmp_by_arrival(const void *a, const void *b) {
    const Process *p1 = (const Process *)a;
    const Process *p2 = (const Process *)b;

    if (p1->arrival_time != p2->arrival_time)
        return p1->arrival_time - p2->arrival_time;
    return p1->burst_time - p2->burst_time;
}

void run_fcfs(Process *procs, int n) {
    qsort(procs, n, sizeof(Process), cmp_by_arrival);

    int current_time = 0;
    double total_wt = 0, total_tat = 0, total_rt = 0;

    for (int i = 0; i < n; i++) {
        if (current_time < procs[i].arrival_time) {
            current_time = procs[i].arrival_time;
        }

        procs[i].waiting_time = current_time - procs[i].arrival_time;
        procs[i].response_time = procs[i].waiting_time;
        current_time += procs[i].burst_time;
        procs[i].turnaround_time = current_time - procs[i].arrival_time;

        total_wt  += procs[i].waiting_time;
        total_tat += procs[i].turnaround_time;
        total_rt  += procs[i].response_time;
    }

    print_gantt(procs, n, "First Come First Served (FCFS)");
    printf("Average Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Response Time: %.2f\n", total_rt / n);
}

void run_sjf(Process *procs, int n) {
    qsort(procs, n, sizeof(Process), cmp_by_arrival);

    int *done = calloc(n, sizeof(int));
    if (!done) {
        perror("calloc");
        return;
    }

    Process *order = malloc(n * sizeof(Process));
    if (!order) {
        perror("malloc");
        free(done);
        return;
    }

    int finished = 0;
    int current_time = procs[0].arrival_time;
    double total_wt = 0, total_tat = 0, total_rt = 0;
    int pos = 0;

    while (finished < n) {
        int idx = -1;
        int best_bt = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (!done[i] && procs[i].arrival_time <= current_time) {
                if (procs[i].burst_time < best_bt) {
                    best_bt = procs[i].burst_time;
                    idx = i;
                } else if (procs[i].burst_time == best_bt &&
                           procs[i].arrival_time < procs[idx].arrival_time) {
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            int next_arrival = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (!done[i] && procs[i].arrival_time < next_arrival) {
                    next_arrival = procs[i].arrival_time;
                }
            }
            current_time = next_arrival;
            continue;
        }

        procs[idx].waiting_time   = current_time - procs[idx].arrival_time;
        procs[idx].response_time  = procs[idx].waiting_time;
        current_time             += procs[idx].burst_time;
        procs[idx].turnaround_time = current_time - procs[idx].arrival_time;

        total_wt  += procs[idx].waiting_time;
        total_tat += procs[idx].turnaround_time;
        total_rt  += procs[idx].response_time;

        order[pos++] = procs[idx];  // keep execution order for Gantt
        done[idx] = 1;
        finished++;
    }

    print_gantt(order, n, "Shortest Job First (SJF)");
    printf("Average Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Response Time: %.2f\n", total_rt / n);

    free(done);
    free(order);
}

int main(void) {
    int n;
    printf("Enter the number of processes: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    Process *input = malloc(n * sizeof(Process));
    if (!input) {
        perror("malloc");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("Enter arrival time and burst time for process %d: ", i + 1);
        scanf("%d %d", &input[i].arrival_time, &input[i].burst_time);
        input[i].pid = i + 1;
        input[i].waiting_time = 0;
        input[i].turnaround_time = 0;
        input[i].response_time = 0;
    }

    Process *fcfs_procs = malloc(n * sizeof(Process));
    Process *sjf_procs  = malloc(n * sizeof(Process));
    if (!fcfs_procs || !sjf_procs) {
        perror("malloc");
        free(input);
        free(fcfs_procs);
        free(sjf_procs);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        fcfs_procs[i] = input[i];
        sjf_procs[i]  = input[i];
    }

    printf("\n");
    run_fcfs(fcfs_procs, n);
    printf("\n");
    run_sjf(sjf_procs, n);

    free(input);
    free(fcfs_procs);
    free(sjf_procs);
    return 0;
}
