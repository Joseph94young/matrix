#include "concurrent_queue/concurrent_queue.h"

#include "gtest/gtest.h"

TEST(testConcurrentQueue, testPush)
{
    ConcurrentQueue<int> cq;
    cq.Push(1);
    EXPECT_EQ(cq.Pop(), 1);
}