#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 10

// Process structure to hold process details
typedef struct {
    int pid;            // Process ID
    int arrival_time;   // Arrival Time
    int burst_time;     // Burst Time
    int service_time;
    int waiting_time;
    int turnaround_time;
    int remaining_time;
    int response_time;
} Process;

// Function prototypes
void FCFS(Process processes[], int n);
void RR(Process processes[], int n, int quantum);
void SPN(Process processes[], int n);
void SRT(Process processes[], int n);
void HRRT(Process processes[], int n);
void FB(Process processes[], int n, int quantum);
void CFS(Process processes[], int n);

int main() {


}