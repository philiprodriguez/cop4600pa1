// Programming Assignment 1
// COP4600
// Philip Rodriguez, Ryan Beck, Steven Chen

/*
  ~~~~~ Imports ~~~~~
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  ~~~~~ Structures ~~~~~
*/
typedef struct Process {
  char * name;
  int arrival;
  int burst;
} Process;

/*
  ~~~~~ Function Declarations ~~~~~
*/
int main();
void parseInputAndDelegateWork(char * inputFilePath, int printVerbose);
void fcfs(Process * processes, int processCount, int runFor);
void sjf(Process * processes, int processCount, int runFor);
void rr(Process * processes, int processCount, int runFor, int quantum);

/*
  ~~~~~ Function Definitions ~~~~~

  Description:
    The actual meaty meat of the code.
*/


// Program Entry Point
int main()
{
  parseInputAndDelegateWork("processes.in", 0);
  return 0;
}

/*
  Input:
    [processes] is a list of processes
    [processCount] is the size of the array [processes]
    [runFor] is the running time
  Output:
    Returns nothing. Creates an output file with the output.
  Description:
    Runs the first come first serve algorithm on the input. Authored by Steven.
*/
void fcfs(Process * processes, int processCount, int runFor)
{
  // get output file handle
  //FILE * outputFile = fopen("processes.out", "w+");
  printf("%d\n", runFor);
  int i, order, left = 0;
  int *arrived = malloc(sizeof(int) * processCount);
  int *selected = malloc(sizeof(int) * processCount);
  int *wait = malloc(sizeof(int) * processCount);
  int *turnaround = malloc(sizeof(int) * processCount);
  int *finished = malloc(sizeof(int) * processCount);
  int *remaining = malloc(sizeof(int) * processCount);
  int *justFinished = malloc(sizeof(int) * processCount);
  int *processOrder = malloc(sizeof(int) * processCount);
  for (i = 0; i < processCount; i++) {
    arrived[i] = 0;
    wait[i] = 0;
    turnaround[i] = 0;
    finished[i] = 0;
    justFinished[i] = 0;
    processOrder[i] = 0;
    selected[i] = 0;
    remaining[i] = processes[i].burst;
  }
  printf("%d processes\n", processCount);
  printf("Using First Come First Served\n\n");

  // get order
  for (int t = 0; t < runFor; t++) {
    for (int a = 0; a < processCount; a++) {
      if (processes[a].arrival == t) {
        processOrder[order] = a;
        order++;
      }
    }
  }

  order = 0;

  for (int t = 0; t <= runFor; t++) {
    int idle = 0;

    // if it is done
    for (int i = 0; i < processCount; i++) {
      if (justFinished[i] == 1) {
        printf("Time %d: %s finished\n", t, processes[i].name);
        justFinished[i] = 0;
        left--;
        idle = 1;
      }
    }

    // if arrived
    for (int p = 0; p < processCount; p++) {
      if (processes[p].arrival == t) {
        printf("Time %d: %s arrived\n", t, processes[p].name);
        arrived[processOrder[order]] = 1;
        left++;
      }
    }

    if (order < processCount) {
      if (selected[processOrder[order]] != 1) {
        if ((processes[processOrder[order]].arrival == t && left > 0) || (left > 0 && finished[processOrder[order]] == 0)) {
          printf("Time %d: %s selected (burst %d)\n", t, processes[processOrder[order]].name, processes[processOrder[order]].burst);
          selected[processOrder[order]] = 1;
          wait[processOrder[order]] = t - processes[processOrder[order]].arrival;
        }
      }

      // if selected, run it
      if (selected[processOrder[order]] == 1 && remaining[processOrder[order]] != 0) {
        remaining[processOrder[order]]--;
      }

      // if the selected is finished, do stuff
      if (finished[processOrder[order]] == 0 && selected[processOrder[order]] == 1 && remaining[processOrder[order]] == 0) {
        justFinished[processOrder[order]] = 1;
        selected[processOrder[order]] = 0;
        finished[processOrder[order]] = 1;

        turnaround[processOrder[order]] = t - processes[processOrder[order]].arrival + 1;
        order++;
      }
    }

    // check for idle
    int k = 0;
    for (int j = 0; j < processCount; j++) {
      if (selected[j] != 0 || justFinished[j] != 0) {
        k = 1;
      }
    }
    if (idle == 0 && k == 0) {
      printf("Time %d: Idle\n", t);
    }
  }

  printf("Finished at time %d\n\n", runFor);

  // print wait and turnaround times
  for (i = 0; i < processCount; i++) {
    printf("%s wait %d turnaround %d\n", processes[i].name, wait[i], turnaround[i]);
  }

  //fclose(outputFile);
  free(arrived);
  free(selected);
  free(wait);
  free(turnaround);
  free(finished);
  free(justFinished);
  free(processOrder);

}


/*
  Input:
    [processes] is a list of processes
    [processCount] is the size of the array [processes]
    [runFor] is the running time
  Output:
    Returns nothing. Creates an output file with the output. TODO: make is actually print to a file
  Description:
    Runs the shortest job first algorithm on the input. Authored by Philip.
*/
void sjf(Process * processes, int processCount, int runFor)
{
  //Get output file handle
  FILE * outputFile = fopen("processes.out", "w+");

  //Parallel arrays to processes
  int * remainingBurstTimes = (int*)malloc(sizeof(int)*processCount);
  int * waitTimes = (int*)malloc(sizeof(int)*processCount);
  int * turnaroundTimes = (int*)malloc(sizeof(int)*processCount);
  int * justFinished = (int*)malloc(sizeof(int)*processCount);

  //Initialize our parallel arrays!
  for(int p = 0; p < processCount; p++)
  {
    remainingBurstTimes[p] = processes[p].burst;
    waitTimes[p] = 0;
    turnaroundTimes[p] = -1;
    justFinished[p] = 0;
  }

  //Go through every tick...
  for (int t = 0; t < runFor; t++)
  {
    //Print finished messages
    for(int p = 0; p < processCount; p++)
    {
      if (justFinished[p])
      {
        fprintf(outputFile, "Time %d: %s finished\n", t, processes[p].name);
        justFinished[p] = 0;
      }
    }

    //Arrive everybody that should get arrived!
    for(int p = 0; p < processCount; p++)
    {
      if (processes[p].arrival == t)
      {
        fprintf(outputFile, "Time %d: %s arrived\n", t, processes[p].name);
      }
    }

    //Find minimum remaining burst time for processes that have arrived...
    int minBurstIndex = -1;
    for(int p = 0; p < processCount; p++)
    {
      if (t < processes[p].arrival)
      {
        // This process has not arrived and should not be considered...
        continue;
      }
      if (remainingBurstTimes[p] <= 0)
      {
        // This process already finished! Skip!
        continue;
      }
      if (minBurstIndex == -1 || remainingBurstTimes[p] < remainingBurstTimes[minBurstIndex])
      {
        // I am the min burst index processes so far!
        minBurstIndex = p;
      }
    }

    //Update waiting times
    for(int p = 0; p < processCount; p++)
    {
      // If we already arrived and we're not going to be run and we have not finished... we're waiting.
      if (processes[p].arrival <= t && minBurstIndex != p && remainingBurstTimes[p] > 0)
      {
        waitTimes[p]++;
      }
    }


    if (minBurstIndex == -1)
    {
      //Nobody to run... idle
      fprintf(outputFile, "Time %d: Idle\n", t);
    }
    else
    {
      //Run the dude!
      fprintf(outputFile, "Time %d: %s selected (burst %d)\n", t, processes[minBurstIndex].name, remainingBurstTimes[minBurstIndex]);
      //Update burst time
      remainingBurstTimes[minBurstIndex]--;
      if (remainingBurstTimes[minBurstIndex] == 0)
      {
        // We just finished!
        justFinished[minBurstIndex] = 1;
        turnaroundTimes[minBurstIndex] = t+1-processes[minBurstIndex].arrival;
      }
    }
  }

  //Print finished messages
  for(int p = 0; p < processCount; p++)
  {
    if (justFinished[p])
    {
      fprintf(outputFile, "Time %d: %s finished\n", runFor, processes[p].name);
      justFinished[p] = 0;
    }
  }

  fprintf(outputFile, "Finished at time %d\n\n", runFor);

  //Print wait and turnaround times
  for(int p = 0; p < processCount; p++)
  {
    fprintf(outputFile, "%s wait %d turnaround %d\n", processes[p].name, waitTimes[p], turnaroundTimes[p]);
  }

  free(remainingBurstTimes);
  free(waitTimes);
  free(turnaroundTimes);
  free(justFinished);
  fclose(outputFile);

  printf("sjf ran successfully. See processes.out file for output!\n");
}

void rr(Process * processes, int processCount, int runFor, int quantum)
{
  printf("rr() not yet implemented!\n");
}

/*
  Input:
    [inputFilePath] is the relative path to the desired input file.
    [printVerbose] is a boolean flag controlling whether additional info gets printed
  Output:
    Returns nothing.
  Description:
    Reads in input from the input file, parses it, and calls one of the
    algorithm methods to do the rest of the job. Frees resources after the
    call to the algorithm method is finished. Authored by Philip.

    Assumes the input doesn't start listing off processes before listing the
    process count.
*/
void parseInputAndDelegateWork(char * inputFilePath, int printVerbose)
{
  printf("Opening and reading input file...\n");

  //Open that thing
  FILE * inputFile = fopen(inputFilePath, "r");

  //Thinks we want to know...
  int processCount = -1;
  int runFor = -1;
  int algorithm = -1; // 0 = fcfs, 1 = sjf, 2 = rr
  int quantum = -1;

  int curProcess = -1; // Used to add processes to the array
  Process * processes = NULL;

  //Create and initialize an empty 1KB token buffer
  char tokenBuffer[1024];
  strcpy(tokenBuffer, "");

  //While there is something to scan
  while(fscanf(inputFile, "%s", tokenBuffer) > 0)
  {
    if (printVerbose)
    {
      printf("Token = \"%s\"\n", tokenBuffer);
    }

    if (strcmp("end", tokenBuffer) == 0)
    {
      // We're done.
      printf("End token encountered. Finished reading input.\n");
      break;
    }
    else if (strcmp("processcount", tokenBuffer) == 0)
    {
      //Read the number!
      int result = fscanf(inputFile, "%d", &processCount);
      if (result > 0)
      {
        //We get out number! Now we can allocate space for the processes!
        if (processes == NULL)
        {
          //Everything seems okay, allocate spaces!
          curProcess = 0;
          processes = (Process*)malloc(sizeof(Process)*processCount);
        }
        else
        {
          //Why on God's green earth did the input have two processcounts?
          printf("Invalid input contains more than one processcount keyword! Exiting...\n");
          return;
        }
      }
      else
      {
        printf("Invalid input syntax. Integer expected after processcount keyword! Exiting...\n");
        return;
      }
    }
    else if (strcmp("runfor", tokenBuffer) == 0)
    {
      //Read the number!
      int result = fscanf(inputFile, "%d", &runFor);
      if (result > 0)
      {
        //We're done.
      }
      else
      {
        printf("Invalid input syntax. Integer expected after runfor keyword! Exiting...\n");
        return;
      }
    }
    else if (strcmp("use", tokenBuffer) == 0)
    {
      int result = fscanf(inputFile, "%s", tokenBuffer);
      if (result > 0)
      {
        //We read something!
        if (strcmp("fcfs", tokenBuffer) == 0)
        {
          algorithm = 0;
        }
        else if (strcmp("sjf", tokenBuffer) == 0)
        {
          algorithm = 1;
        }
        else if (strcmp("rr", tokenBuffer) == 0)
        {
          algorithm = 2;
        }
        else
        {
          printf("Invalid input syntax. Algorithm expected after use keyword, found \"%s\" instead. Exiting...\n", tokenBuffer);
          return;
        }
      }
      else
      {
        printf("Invalid input syntax. Algorithm expected after use keyword! Exiting...\n");
        return;
      }
    }
    else if (strcmp("quantum", tokenBuffer) == 0)
    {
      //Read the number!
      int result = fscanf(inputFile, "%d", &quantum);
      if (result > 0)
      {
        //We're done.
      }
      else
      {
        printf("Invalid input syntax. Integer expected after quantum keyword! Exiting...\n");
        return;
      }
    }
    else if (strcmp("process", tokenBuffer) == 0)
    {
      //Enforce that the processcount was hit first!
      if (processes == NULL)
      {
        printf("Invalid input. Process count must be provided before listing processes. Exiting...\n");
        return;
      }

      //Make sure we have space!
      if (curProcess >= processCount)
      {
        printf("Invalid input. More processes listed than process count indicates. Exiting...\n");
        return;
      }

      //Scan in the silly name word, and ignore it.
      fscanf(inputFile, "%s", tokenBuffer);

      //Scan in the ACTUAL name.
      int result = fscanf(inputFile, "%s", tokenBuffer);
      if (result > 0)
      {
        char * pname = (char*)malloc(sizeof(char)*strlen(tokenBuffer+1));
        strcpy(pname, tokenBuffer);

        //Scan in the silly arrival word, and ignore it.
        fscanf(inputFile, "%s", tokenBuffer);

        //Scan in the arrival time
        int parrival = -1;
        result = fscanf(inputFile, "%d", &parrival);
        if (result > 0)
        {
          //We read in parrival successfully

          //Scan in the silly burst word, and ignore it.
          fscanf(inputFile, "%s", tokenBuffer);

          //Scan in the burst time
          int pburst = -1;
          result = fscanf(inputFile, "%d", &pburst);
          if (result > 0)
          {
            //We have pname, parrival, and pburst ready to go! Put it in our
            //process list!
            processes[curProcess].name = pname;
            processes[curProcess].arrival = parrival;
            processes[curProcess].burst = pburst;
            curProcess++;
          }
          else
          {
            printf("Invalid input syntax. Process burst time expected but not found! Exiting...\n");
            return;
          }
        }
        else
        {
          printf("Invalid input syntax. Process arrival time expected but not found! Exiting...\n");
          return;
        }
      }
      else
      {
        printf("Invalid input syntax. Process name expected but not found! Exiting...\n");
        return;
      }
    }
    else if (tokenBuffer[0] == '#')
    {
      //Eat the comment!
      while(fgetc(inputFile) != '\n')
      {
        //Waste it!
      }
    }
    else
    {
      printf("Ignoring an invalid token: \"%s\"\n", tokenBuffer);
    }
  }

  if (printVerbose)
  {
    printf("Process Count = %d\n", processCount);
    printf("runFor = %d\n", runFor);
    printf("algorithm = %d\n", algorithm);
    printf("quantum = %d\n", quantum);
    printf("Process List:\n");
    for (int i = 0; i < curProcess; i++)
    {
      printf("   Process Name = \"%s\", Arrival = %d, Burst = %d\n", processes[i].name, processes[i].arrival, processes[i].burst);
    }
  }

  //Sanitize input for processcount, runfor, algorithm, quantum
  if (processCount == -1)
  {
    printf("Invalid input. Process count never set! Exiting...\n");
    return;
  }
  if (processCount != curProcess)
  {
    printf("Invalid input. Process count not equal to the number of processes supplied. Exiting...\n");
    return;
  }
  if (runFor == -1)
  {
    printf("Invalid input. Running time (runfor) never set! Exiting...\n");
    return;
  }
  if (algorithm == -1)
  {
    printf("Invalid input. Algorithm to use never set! Exiting...\n");
    return;
  }
  if (algorithm == 2 && quantum == -1)
  {
    printf("Invalid input. Round Robin algorithm selected, but quantum never set! Exiting...");
    return;
  }

  //Now, call the appropriate next step...
  if (algorithm == 0)
  {
    fcfs(processes, processCount, runFor);
  }
  else if (algorithm == 1)
  {
    sjf(processes, processCount, runFor);
  }
  else if (algorithm == 2)
  {
    rr(processes, processCount, runFor, quantum);
  }
  else
  {
    //Literally cannot happen
    printf("Invalid algorithm. Exiting...\n");
    return;
  }

  //Free any dynamically allocated resources!
  for (int p = 0; p < processCount; p++)
  {
    free(processes[p].name);
  }
  free(processes);

  //We're done with the file, so close it.
  fclose(inputFile);
}
