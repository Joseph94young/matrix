#include <iostream>

#include "concurrent/concurrent_queue.h"

int main(int argc, char** argv)
{
    ConcurrentQueue<int> my_queue(100);
    for (int i = 1; i < argc; i++) {
        try {
            auto item = std::stoi(*++argv);
            my_queue.Push(item);
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << " throw an exception as the " << i << "th agrv \"" << *argv
                      << "\" is not an integer." << std::endl;
        }
    }
    while (!my_queue.Empty()) {
        std::cout << "Queue item: " << my_queue.Pop() << std::endl;
    }
    return 0;
}
