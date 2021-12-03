
#include <mutex>
#include <condition_variable>
class NBSemaphore {
public:
    NBSemaphore(int count) : counter{ count } { };
    std::mutex mut;
    std::condition_variable cv;;
    std::atomic<int> counter = 0;

    inline void notify()
    {
        //### Use unique_lock to lock a resource
        std::unique_lock<std::mutex> uLock(mut);
        counter++;
        uLock.unlock();
    }

    inline void wait()
    {
        //### Use unique_lock to lock a resource
        std::unique_lock<std::mutex> uLock(mut);
        cv.wait(uLock, [this] {return counter > 0; });

        counter--;
    }
};
