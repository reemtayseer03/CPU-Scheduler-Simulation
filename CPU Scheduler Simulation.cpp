#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <queue>
#include <climits> // For INT_MAX
using namespace std;

struct Process {
    string Name;
    int ATime;      // Arrival time
    int BurstTime;  // Burst time
    int RTime;      // Remaining time
    int FTime;      // Finish time
    int WTime;      // Waiting time
    int TATime;     // Turnaround time
};
void ReadtFile(const string& fileName, vector<Process>& processes, int& quantum) {
    ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        cout << "Error: Could not open the file!" << endl;
        exit(1);
    }
    int num; // Number of processes
    inputFile >> num;
    for (int i = 0; i < num; ++i) {
        Process process;
        inputFile >> process.Name >> process.ATime >> process.BurstTime;
        process.RTime = process.BurstTime;
        processes.push_back(process);
    }
    inputFile >> quantum;
    inputFile.close();
}
void calculate(vector<Process>& processes) {
    for (auto& process : processes) {
        process.TATime = process.FTime - process.ATime;
        process.WTime = process.TATime - process.BurstTime;
    }
}
void GanttChart(const vector<pair<string, int>>& timeline) {
    cout << "\nGantt Chart:" << endl;
    for (const auto& slot : timeline) {
        cout << "| " << setw(3) << slot.first << " ";
    }
    cout << "|" << endl;
    for (const auto& slot : timeline) {
        cout << "  " << setw(3) << slot.second << "  ";
    }
    cout << endl;
}
void FCFS(vector<Process> processes) {//first come first served
    cout << "\n--- First-Come, First-Served (FCFS) ---" << endl;
    vector<pair<string, int>> timeline;
    int curr = 0;

    for (auto& process : processes) {
        if (curr < process.ATime) {
            curr = process.ATime;
        }
        timeline.emplace_back(process.Name, curr);
        curr += process.BurstTime;
        process.FTime = curr;
    }

    calculate(processes);
    GanttChart(timeline);

    cout << "\nProcess Metrics:\n";
    for (const auto& process : processes) {
        cout << process.Name << "\tFinish: " << process.FTime
            << "\tWaiting: " << process.WTime
            << "\tTurnaround: " << process.TATime << endl;
    }
}
void SRT(vector<Process> processes) {// shortest remaining time
    cout << "\n--- Shortest Remaining Time (SRT) ---" << endl;
    vector<pair<string, int>> timeline;
    int curr = 0;
    int completed = 0;

    while (completed < processes.size()) {
        int shortestIdx = -1;
        int minRemainingTime = INT_MAX;

        for (int i = 0; i < processes.size(); ++i) {
            if (processes[i].ATime <= curr && processes[i].RTime > 0) {
                if (processes[i].RTime < minRemainingTime) {
                    minRemainingTime = processes[i].RTime;
                    shortestIdx = i;
                }
            }
        }
        if (shortestIdx == -1) {
            curr++;
            continue;
        }
        timeline.emplace_back(processes[shortestIdx].Name, curr);
        curr++;
        processes[shortestIdx].RTime--;

        if (processes[shortestIdx].RTime == 0) {
            processes[shortestIdx].FTime = curr;
            completed++;
        }
    }

    calculate(processes);
    GanttChart(timeline);

    cout << "\nProcess Metrics:\n";
    for (const auto& process : processes) {
        cout << process.Name << "\tFinish: " << process.FTime
            << "\tWaiting: " << process.WTime
            << "\tTurnaround: " << process.TATime << endl;
    }
}
void RoundRobin(vector<Process> processes, int quantum) { // round robin
    cout << "\n--- Round Robin (RR) ---" << endl;
    vector<pair<string, int>> timeline;
    queue<int> Queue;
    int curr = 0;
    int completed = 0;
    for (int i = 0; i < processes.size(); ++i) {
        if (processes[i].ATime <= curr) {
            Queue.push(i);
        }
    }
    while (completed < processes.size()) {
        if (Queue.empty()) {
            curr++;
            for (int i = 0; i < processes.size(); ++i) {
                if (processes[i].ATime <= curr && processes[i].RTime > 0) {
                    Queue.push(i);
                }
            }
            continue;
        }
        int idx = Queue.front();
        Queue.pop();
        timeline.emplace_back(processes[idx].Name, curr);
        int execTime = min(quantum, processes[idx].RTime);
        curr += execTime;
        processes[idx].RTime -= execTime;

        for (int i = 0; i < processes.size(); ++i) {
            if (processes[i].ATime <= curr && processes[i].RTime > 0 && i != idx) {
                Queue.push(i);
            }
        }

        if (processes[idx].RTime > 0) {
            Queue.push(idx);
        }
        else {
            processes[idx].FTime = curr;
            completed++;
        }
    }
    calculate(processes);
    GanttChart(timeline);

    cout << "\nProcess Metrics:\n";
    for (const auto& process : processes) {
        cout << process.Name << "\tFinish: " << process.FTime
            << "\tWaiting: " << process.WTime
            << "\tTurnaround: " << process.TATime << endl;
    }
}
int main() {
    vector<Process> processes;
    int quantum;
    string filename = "C:\\Users\\Reem\\OneDrive\\”ÿÕ «·„ﬂ »\\processess.txt";
    cout << "Looking for file at: " << filename << endl;
    ReadtFile(filename, processes, quantum);
    FCFS(processes);
    SRT(processes);
    RoundRobin(processes, quantum);
    return 0;
}
