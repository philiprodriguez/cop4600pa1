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

void fcfs(Process * processes, int processCount, int runFor)
{
  printf("fcfs() not yet implemented!\n");
}

void sjf(Process * processes, int processCount, int runFor)
{
  printf("sjf() not yet implemented!\n");
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

  //TODO: sanitize input for processcount, runfor, algorithm

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

  //TODO: FREE EVERYTHING (processes list)

  //We're done with the file, so close it.
  fclose(inputFile);
}