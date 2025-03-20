
# Server IO  
This repository contains my solution to [Track 7 Technical task](https://github.com/viCodexm/ServerIO/blob/master/track7-task.pdf) for Huawei Future Star Internship program.  

## Output
![out_image](https://github.com/viCodexm/ServerIO/blob/master/output.png)


## What's happening
Server recieves WRITE and READ [requests](https://github.com/viCodexm/ServerIO/blob/master/requests.txt) in specified time (usec).
Server can handle N requests simultaneously.  
WRITE requests block access to all operations in range [adress, adress + size]  
Task is to emulate server processing given set of requests.  
You can see that difference in latency time is huge between handling only 1 request and 5 simultaniously.  


## Algorithm

1. Get incoming requests.  
2. Check if we can start handling it now, otherwise put in waiting queue.  
3. If some task is done - from waiting queue get the oldest request that wants to access free memory region, if none, wait for a new one.
4. repeat  
  
Obviously, there is a need to check if some block of memory is availible in the moment for request.  
To do that I use AVL tree.   
Now it's possible to store memory segments occupied by WRITE operations and check if we can perform any operation on memory region.  
Each segment is represented in two numbers - start and end.  
AVL tree has O(logn) Insertion, Deletion and Searching in Average and Worst cases.  

Hope you like it.


