#include<condition_variable>
#include<functional>
#include<future>
#include<memory>
#include<thread>
#include<mutex>
#include<queue>
#include<type_traits>
#include<utility>

// Mainly refer to: https://github.com/bshoshany/thread-pool

class ThreadPool
{
private:
    using concurrency_t =
        std::invoke_result_t<decltype(std::thread::hardware_concurrency)>;
    static_assert(std::is_same<unsigned int, concurrency_t>::value);

    std::unique_ptr<std::thread[]> _threads = nullptr;
    concurrency_t _threadCount;
    std::condition_variable taskAvailableCv;
    std::condition_variable taskDoneCv;

    std::queue<std::function<void()>> taskQueue;
    std::mutex taskQueueMutex;
    size_t taskRunning = 0;
    bool waiting = false;
    bool workers_running = false;

    concurrency_t detetmineThreadCount(concurrency_t threadCount) {
        if (threadCount > 0) return threadCount;
        threadCount = std::thread::hardware_concurrency() * 2;
        if (threadCount > 0) return threadCount;
        return 1;
    }

    void createThreads() {
        {
            std::scoped_lock _(taskQueueMutex);
            workers_running = true;
        }
        for (concurrency_t i = 0; i < _threadCount; ++i) {
            _threads[i] = std::thread(&ThreadPool::worker, this);
        }
    }

    void destoryThreads() {
        {
            std::scoped_lock _(taskQueueMutex);
            workers_running = false;
        }
        taskAvailableCv.notify_all();
        for (concurrency_t i = 0; i < _threadCount; ++i) {
            _threads[i].join();
        }
    }

    void worker() {
        std::function<void()> task;
        while (true) {
            std::unique_lock taskLock(taskQueueMutex);
            taskAvailableCv.wait(taskLock, [this] {
                return !taskQueue.empty() || !workers_running;
            });

            if (!workers_running) break;

            task = std::move(taskQueue.front());
            taskQueue.pop();
            ++taskRunning;
            taskLock.unlock();

            task();

            taskLock.lock();
            --taskRunning;
            if (waiting && taskRunning == 0 && taskQueue.empty()) {
                taskDoneCv.notify_all();
            }
        }
    }

public:
    ThreadPool(const concurrency_t threadCount = 0): 
        _threadCount(detetmineThreadCount(threadCount)),
        _threads(std::make_unique<std::thread[]>(detetmineThreadCount(threadCount))) {
            createThreads();
        };
    
    ~ThreadPool() {
        waitForAllTasks();
        destoryThreads();
    }

    template<typename F, typename... A>
    void push_task(F&& task, A&&... args) {
        {
            std::scoped_lock _(taskQueueMutex);
            // create a function without args
            taskQueue.push(
                std::bind(std::forward<F>(task), std::forward<A>(args)...)
            );
        }
        taskAvailableCv.notify_one();
    }
    
    template <typename F, typename... A, typename R = std::invoke_result_t<std::decay_t<F>, std::decay_t<A>...>>
    std::future<R> submit(F&& task, A&&... args) {
        std::shared_ptr<std::promise<R>> taskPromise = std::make_shared<std::promise<R>>();
        push_task(
            [taskFuntion = std::bind(std::forward<F>(task), std::forward<A>(args)...), taskPromise] {
                if constexpr (std::is_void_v<R>) {
                    std::invoke(taskFuntion);
                    taskPromise->set_value();
                } else {
                    taskPromise->set_value(std::invoke(taskFuntion));
                }
            }
        );
        return taskPromise->get_future();
    }

    void waitForAllTasks() {
        std::unique_lock taskLock(taskQueueMutex);
        waiting = true;
        taskDoneCv.wait(taskLock, [this] 
            { return taskRunning == 0 && taskQueue.empty(); });
        waiting = false;
    }
};  