#include "gtest/gtest.h"

#include "concurrent_queue/concurrent_queue.h"

TEST(testConcurrentQueue, testPush)
{
    ConcurrentQueue<int> cq;
    cq.Push(1);
    EXPECT_EQ(cq.Pop(), 1);
}