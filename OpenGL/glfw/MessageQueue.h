/**
* <pre>
* Author: taylorcyang@tencent.com
* Date:   2019-09-06
* Time:   10:15
* Life with Passion, Code with Creativity.
* </pre>
*/

#pragma once

#include <cstdint>
#include <list>
#include <mutex>
#include <vector>


template<typename T>
class MemoryPool {
public:
    MemoryPool(std::size_t capacity) :
            mCapacity(capacity),
            mPool(),
            mPoolLock() {
    }

    ~MemoryPool() {
        cleanup();
    }

    T *obtain() {
        std::lock_guard<std::mutex> lk(mPoolLock);
        if (!mPool.empty()) {
            auto *ret = mPool.front();
            mPool.pop_front();
            return ret;
        } else {
            return new T();
        }
    }

    void release(T *item) {
        std::lock_guard<std::mutex> lk(mPoolLock);
        if (mPool.size() < mCapacity) {
            mPool.push_back(item);
        } else {
            delete item;
        }
    }

    void cleanup() {
        std::lock_guard<std::mutex> lk(mPoolLock);
        for (auto *item: mPool) {
            delete item;
        }
        mPool.clear();
    }

private:
    std::size_t mCapacity;
    std::list<T *> mPool;
    std::mutex mPoolLock;
};

class Message {
public:
    typedef void (MessageProc)(const Message &);

    int32_t what = 0;

    int32_t data0 = 0;
    int32_t data1 = 0;
    int32_t data2 = 0;
    int32_t data3 = 0;

    void *ptr0 = nullptr;
    void *ptr1 = nullptr;
    void *ptr2 = nullptr;
    void *ptr3 = nullptr;

private:
    MessageProc *handlerProc;
    MessageProc *cleanupProc;

    Message() : handlerProc(nullptr), cleanupProc(nullptr) {}

public:

    Message(MessageProc *handlerProc, MessageProc *cleanupProc)
            : handlerProc(handlerProc), cleanupProc(cleanupProc) {}

    ~Message() = default;

    MessageProc *getHandlerProc() {
        return handlerProc;
    }

    MessageProc *getCleanupProc() {
        return cleanupProc;
    }

private:

    void performCleanup() {
        if (cleanupProc) {
            cleanupProc(*this);
        }
        what = data0 = data1 = data2 = data3;
        ptr0 = ptr1 = ptr2 = ptr3 = nullptr;
        handlerProc = cleanupProc = nullptr;
    }

    void handle() {
        if (handlerProc) {
            handlerProc(*this);
        }
    }

    std::int64_t dueTime = 0;
    int32_t messageId = 0;

    inline bool due();

    friend class MessageQueue;

    friend class MemoryPool<Message>;
};

template<typename T>
class LoopQueue {
private:
    std::vector<T> mStore;
    std::size_t mBegin = 0;
    std::size_t mEnd = 0;

private:
    bool full() {
        return mBegin == mEnd;
    }

    void reLayout(int32_t reserve) {
        std::vector<T> newStore;
        newStore.reserve(size() + reserve);
        for (auto i = mBegin; i < mStore.size(); i++) {
            newStore.push_back(mStore[i]);
        }
        for (auto i = 0; i < mEnd; i++) {
            newStore.push_back(mStore[i]);
        }
        std::swap(mStore, newStore);
        mBegin = 0;
        mEnd = mStore.size();
    }

public:

    T &operator[](int32_t index) {
        index += mBegin;
        if (index >= mStore.size()) {
            index -= mStore.size();
        }
        return mStore[index];
    }

    void reserve(std::size_t size) {
        if (mEnd <= mBegin) {
            reLayout(size);
        } else {
            mStore.reserve(size);
        }
    }

    void clear() {
        mBegin = mEnd = 0;
        mStore.clear();
    }

    void push_back(const T &value) {
        if (full()) {
            reLayout(1);
        }

        if (mEnd == mStore.size()) {
            mStore.push_back(value);
        } else {
            mStore[mEnd] = value;
        }
        mEnd++;
    }

    void push_front(const T &value) {
        if (full()) {
            reLayout(1);
        }
        auto index = mBegin - 1;
        if (index < 0) {
            index += mStore.size();
        }
        if (index == mStore.size()) {
            mStore.push_back(value);
        } else {
            mStore[index] = value;
        }
        mBegin = index;
    }

    T &back() {
        auto i = mEnd - 1;
        if (i < 0) {
            i += mStore.size();
        }
        return mStore[i];
    }

    T &front() {
        return mStore[mBegin];
    }

    T pop_back() {
        assert(!empty());
        mEnd--;
        if (mEnd < 0) {
            mEnd += mStore.size();
        }
        return mStore[mEnd];
    }

    T pop_front() {
        assert(!empty());
        T value = mStore[mBegin];
        mBegin++;
        if (mBegin == mStore.size()) {
            mBegin = 0;
        }
        return value;
    }

    void insert(int32_t index, const T &value) {
        if (index == size()) {
            push_back(value);
        } else {
            if (mBegin < mEnd) {
                mStore.insert(mStore.begin() + mBegin + index, value);
                mEnd++;
            } else {
                int pos = mBegin + index;
                if (pos < mStore.size()) {
                    mStore.insert(mStore.begin() + pos, value);
                } else {
                    pos -= mStore.size();
                    mStore.insert(mStore.begin() + pos, value);
                    mBegin++;
                    mEnd++;
                }
            }
        }
    }

    void remove(int32_t index) {
        if (index == 0) {
            pop_front();
        } else if (index == size() - 1) {
            pop_back();
        } else {
            if (mBegin < mEnd) {
                mStore.erase(mStore.begin() + mBegin + index);
                mEnd--;
            } else {
                int pos = mBegin + index;
                if (pos < mStore.size()) {
                    mStore.erase(mStore.begin() + pos);
                } else {
                    pos -= mStore.size();
                    mStore.erase(mStore.begin() + pos);
                    mBegin--;
                    mEnd--;
                }
            }
        }
    }

    int32_t size() {
        auto size = mEnd - mBegin;
        if (size < 0) {
            size += mStore.size();
        }
        return size;
    }

    bool empty() {
        return mStore.size() == size();
    }
};

class MessageQueue {
public:
    class Supervisor {
    protected:
        virtual void beforeMessage(const Message &message) = 0;

        virtual void afterMessage(const Message &message) = 0;

    private:
        friend MessageQueue;
    };

private:
    MemoryPool<Message> mMessagePool;
    std::atomic_bool mShutdown;
    std::atomic_bool mProcessingMessage;

    std::mutex mQueueMutex;
    std::condition_variable mQueueNotEmptyCondition;
    LoopQueue<Message *> mQueue;
    std::atomic_int32_t mMessageIdCounter;

    std::shared_ptr<Supervisor> mSupervisor;

    static constexpr std::size_t kDefaultPoolSize = 1024;

    friend class Message;

private:
    static int64_t timestamp();

    bool hasDueMessageLocked();

    int32_t findInsertPositionLocked(int64_t dueTime);

    inline void assertNotShutDown();

    /**
     * obtain a new message, to be latter posted.
     */
    Message *obtainMessage();

    void releaseMessage(Message *message);

    void beforeMessage(const Message &message);

    void afterMessage(const Message &message);

public:
    MessageQueue();

    ~MessageQueue();

    void shutDown();

    void setSupervisor(const std::shared_ptr<Supervisor> &supervisor);

    /**
     * post a message to queue
     * @param message the ownership transfers to this queue
     * @return messageId used to removeMessage
     */
    int32_t postMessage(const Message &message);

    int32_t postMessageDelayed(const Message &message, int64_t delayMillis);

    int32_t postMessageAtFrontQueue(const Message &message);

    bool removeMessage(int32_t messageId);

    /**
     * @param what Message::what
     * @return removed or not
     */
    bool removeMessageByWhat(int32_t what);

    /**
     *
     * @param proc
     * @return removed or not
     */
    bool removeMessageByHandlerProc(Message::MessageProc *proc);

    void loopQueue();
};



