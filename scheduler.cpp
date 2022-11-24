// this is the only file you should modify and submit for grading

#include "scheduler.h"
#include "common.h"

// this is the function you should implement
//
// runs Round-Robin scheduling simulator
// input:
//   quantum = time slice
//   max_seq_len = maximum length of the reported executing sequence
//   processes[] = list of process with populated IDs, arrivals, and bursts
// output:
//   seq[] - will contain the execution sequence but trimmed to max_seq_len size
//         - idle CPU will be denoted by -1
//         - other entries will be from processes[].id
//         - sequence will be compressed, i.e. no repeated consecutive numbers
//   processes[]
//         - adjust finish_time and start_time for each process
//         - do not adjust other fields
//


void simulate_rr(
    int64_t quantum, 
    int64_t max_seq_len,
    std::vector<Process> & processes,
    std::vector<int> & seq
) {
    //clear any previous reuslts in seq
    seq.clear();

    // initializing the variables according to Appendix 2 - hints for question 2
    int64_t curr_time = -1;
    int curr_process = 0;
    int64_t remaining_slice = 0;
    int cpu = -1;
    std::vector<int> rq, jq;
    int curr_jq;
    std::vector<int64_t> remaining_bursts;
    int curr_rq;

    // if the processes vector is empty, then there is no process to be followed
    if(processes.empty())
    {
        return;
    }

    // From Appendix 2 - hints for question 2
    for(int i = 0;i<long(processes.size());i++)
    {
        // You would initialize JQ with all processes, and remove them from JQ as they ‘arrive’.
        jq.push_back(processes[i].id);

        //Since you cannot modify processes[], you need to keep track of this in your own data
        remaining_bursts.push_back(processes[i].burst);
    }

    // to keep track of the size of jq and rq
    long jobs_remaining = (long)jq.size();
    long ready_size = (long)rq.size();

    // initializing remaining_slice to the quantum
    remaining_slice = quantum;

    //implementing a do while loop, so that the current time stats from 0
    do
    {
        // if the process has ended, then return
        if(jobs_remaining == 0 && ready_size == 0 )
        {
            break;
        }

        //adding a process in rq and removing it from jq
        // on the conditon that jq is not empty (means we have one or more processes that are to be handled) and current time matches the arrival time of the process        
        if(jobs_remaining !=0 && curr_time == processes[jq[0]].arrival)
        {
            curr_jq = jq[0];

            // adding process to rq
            rq.push_back(curr_jq);

            //removing the same process from jq from the begining
            jq.erase(jq.begin());
            jobs_remaining--;
            ready_size++;
            continue;
        }

        // affirming that rq is not empty
        if(ready_size !=0 )
        {
            // to check if the current process has within the quantum has finished but the original process still has not finished
            if(remaining_slice==0 )
            {
                if( remaining_bursts[rq[0]]!=0 )
                {
                    // re-initialising remaining slices to quantum nmuber
                    remaining_slice = quantum;
                    // storing current rq into a temporary varibale so that we do not lose trac of it becuse in the next sentence we are 
                    // erasing it
                    curr_process = rq[0];
                    rq.erase(rq.begin());
                    // pushing the current process to the back of rq since it is to be revisited since it has not finished yet
                    rq.push_back(curr_process);
                }
            }
      
            // if the process has finally finished
            if(remaining_bursts[rq[0]]==0)
            {
                curr_rq = rq[0];
                processes[curr_rq].finish_time = curr_time;
                // erasing the process completely since it is finished
                rq.erase(rq.begin());
                ready_size--;
                remaining_slice = quantum;

            }
       
            // one burst job on cpu       
            if(remaining_slice > 0)
            {
                if(!(remaining_slice > quantum))
                {     
                    curr_rq = rq[0];   
                    // executing 1 quantum of time
                    remaining_bursts[curr_rq] = remaining_bursts[curr_rq] -1;  

                    // starting a new process
                    if(processes[curr_rq].start_time == -1)
                    {
                        processes[curr_rq].start_time=curr_time;
                    }
                    //updating remaining slices
                    remaining_slice = remaining_slice - 1;
                    cpu = curr_rq;
                }
            
            }
        }
     
        // the size of seq should not increase more than max_seq_len and we do not want duplicate consecutive entries
        if(seq.back() !=cpu && (long)seq.size()<max_seq_len)
        {
            seq.push_back(cpu);
        }
        curr_time++;

    }while(true);
}
