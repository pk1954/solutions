// ThreadTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <chrono>
#include <iostream>
#include <thread>

using std::cout;
using std::thread;
using std::chrono::seconds;
using std::this_thread::sleep_for;

void independentThread() 
{
    cout << "Starting concurrent thread.\n";
    sleep_for(seconds(2));
    cout << "Exiting concurrent thread.\n";
}
 
void threadCaller() 
{
    cout << "Starting thread caller.\n";
    thread t(independentThread);
    t.detach();
    sleep_for(std::chrono::seconds(1));
  //  t.join();
    cout << "Exiting thread caller.\n";
}
 
int main() 
{
    cout << "Hello World!\n";
    threadCaller();
    sleep_for(seconds(5));
    cout << "Good bye World!\n";
}
