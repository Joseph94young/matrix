#ifdef __clang__

#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wkeyword-macro"
#    define private public     // NOLINT(cppcoreguidelines-macro-usage)
#    define protected public   // NOLINT(cppcoreguidelines-macro-usage)
#    pragma clang diagnostic pop

#elif defined(__GNUC__) || defined(__GNUG__)

#    define private public     // NOLINT(cppcoreguidelines-macro-usage)
#    define protected public   // NOLINT(cppcoreguidelines-macro-usage)

#endif

#include "concurrent/concurrent_queue.h"

#include "gtest/gtest.h"

TEST(testConcurrentQueue, testPush)
{
    ConcurrentQueue<int> cq(100);
    cq.Push(1);
    EXPECT_EQ(cq.queue_.size(), 1);
    EXPECT_EQ(cq.queue_.front(), 1);
}

TEST(testConcurrentQueue, testPop)
{
    ConcurrentQueue<int> cq(100);
    cq.Push(1);
    EXPECT_EQ(cq.Pop(), 1);
    EXPECT_EQ(cq.queue_.empty(), true);
}