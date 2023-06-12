# Message Encoding/Decoding System :closed_lock_with_key:

This project aims to implement a multi-processing approach for sending encoded messages that can only be decoded by the intended receiver. The system involves the use of several processes, including a sender process, receiver process, master spy process, helper processes, and spy processes. The system operates under the assumption that a successful operation occurs when the receiver can decode the messages before the master spy, while a failed operation occurs if the master spy decodes the messages first.

## 🚀 Application Scenario :clipboard:

The system follows the following scenario:

1. A parent process creates the necessary child processes: a single sender process, a single receiver process, a single master spy process, a user-defined number of helper processes, and a user-defined number of spy processes.
2. The sender process retrieves the message from an input file (e.g., `sender.txt`). The message may contain multiple lines and paragraphs.
3. The sender process splits the input file into columns based on the blank character between words.
4. Each child process is responsible for encoding a specific column message received from the sender process. The encoding process is as follows:
   - For the first column, each word is modified by adding 1 to the first character modulo 26, 2 to the second character modulo 26, and so on. The encoded message is then placed in the first location of the shared memory.
   - For the second column, each word is modified by adding 2 to the first character modulo 26, 4 to the second character modulo 26, and so on. The encoded message is then placed in the second location of the shared memory.
   - The same logic applies to subsequent columns and words in each column.
   - Special characters and numbers are encoded based on specific rules mentioned in the project requirements.
   - Each encoded column in the shared memory is identified by a unique prefix or suffix.
5. Helper processes continuously swap the messages present in the shared memory to hinder spy processes from obtaining all the columns of the file.
6. Spy processes randomly access shared memory locations to retrieve encoded messages and send them to the master spy process.
7. The master spy process attempts to order the columns in the correct order after receiving them from the spy processes. It drops any columns it has already received. Once confident it has received all the columns, it tries to decode the messages present in the `spy.txt` file.
8. The receiver process also randomly accesses shared memory locations to retrieve encoded messages. Similar to the master spy process, it orders the received columns correctly and discards any duplicates. Once confident it has received all the columns, it attempts to decode the messages present in the `receiver.txt` file.
9. The parent process determines whether the receiver process was able to obtain the correct file from the sender before the master spy process. If the receiver succeeds, the operation is considered successful; otherwise, it is labeled as a failed operation.
10. The simulation ends if any of the following conditions are met:
    - The number of failed decoding operations by the receiver exceeds a user-defined threshold.
    - The number of successful decoding operations by the receiver exceeds a user-defined threshold.

## 📝 To-Do List :pencil:

- [ ] Implement the parent process to create necessary child processes.
- [ ] Develop the sender process to retrieve and split the message into columns.
- [ ] Implement child processes to encode column messages and place them in the shared memory.
- [ ] Create helper processes to swap messages in the shared memory.
- [ ] Develop spy processes to access and send encoded messages to the master spy process.
- [ ] Implement the master spy process to order and decode received messages.
- [ ] Create the receiver process to access and decode messages from the shared memory.
- [ ] Develop the parent process to evaluate the success or failure of the operation.
- [ ] Handle termination conditions to end the simulation.
- [ ] Write unit tests to ensure the correctness of the implemented functionalities.
- [ ] Update the README file with detailed instructions on how to run the system.
- [ ] Write a comprehensive documentation describing the system's architecture, components, and functionality.

## 🤖 Tech Stack

- Linux OS
- Docker
- VNC client
- C programming language
- OpenGL library

🚧 This project is a work in progress. Further updates and enhancements will be made.

# TODO List

- [ ] Parent Process:
  - [ ] Create shared memory for message exchange between processes.
  - [ ] Create semaphores for synchronization and mutual exclusion.
  - [ ] Create sender, receiver, master spy, helper, and spy processes.
  - [ ] Wait for the receiver process to complete and determine the operation's success.

- [ ] Sender Process:
  - [ ] Read the message from the input file (sender.txt).
  - [ ] Split the message into columns based on the blank character.
  - [ ] Replace empty strings with "alright".
  - [ ] Calculate the number of columns.
  - [ ] Create child processes for each column and pass the column message to them.
  - [ ] Encode the column message based on the specified rules.
  - [ ] Place the encoded message in the shared memory.

- [ ] Child Processes (Column Encoding):
  - [ ] Receive the column message from the sender process.
  - [ ] Encode the message according to the given rules.
  - [ ] Add the encoded message to the shared memory.

- [ ] Helper Processes:
  - [ ] Continuously swap the messages present in the shared memory to confuse spy processes.
  - [ ] Generate random locations in the shared memory to perform swapping.
  - [ ] Use semaphores to ensure mutual exclusion during swapping.

- [ ] Spy Processes:
  - [ ] Continuously access random locations in the shared memory.
  - [ ] Retrieve the encoded messages and send them to the master spy process.
  - [ ] Use semaphores to coordinate access to shared memory.

- [ ] Master Spy Process:
  - [ ] Receive encoded messages from spy processes.
  - [ ] Order the columns in the correct order.
  - [ ] Drop columns that are already received.
  - [ ] Determine when all columns are received.
  - [ ] Attempt to decode the messages in the spy.txt file.
  - [ ] Inform the parent process of the decoding result.

- [ ] Receiver Process:
  - [ ] Continuously access random locations in the shared memory.
  - [ ] Retrieve the encoded messages.
  - [ ] Order the columns in the correct order.
  - [ ] Drop columns that are already received.
  - [ ] Determine when all columns are received.
  - [ ] Attempt to decode the messages in the receiver.txt file.
  - [ ] Inform the parent process of the decoding result.

- [ ] Parent Process (Continued):
  - [ ] Wait for the receiver process to complete and receive the decoding result.
  - [ ] Determine if the receiver successfully decoded the messages before the master spy.
  - [ ] Label the operation as successful or failed based on the result.

- [ ] Termination Condition:
  - [ ] Monitor the number of failed decoding operations by the receiver process.
  - [ ] Monitor the number of successful decoding operations by the receiver process.
  - [ ] Terminate the simulation if either threshold is exceeded.
