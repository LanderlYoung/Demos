/**
* <pre>
* Author: taylorcyang@tencent.com
* Date:   2019-09-06
* Time:   10:15
* Life with Passion, Code with Creativity.
* </pre>
*/

#include "MessageQueue.h"

bool Message::due() {
    return dueTime <= MessageQueue::timestamp();
}

MessageQueue::MessageQueue() :
        mMessagePool(kDefaultPoolSize),
        mShutdown(false),
        mProcessingMessage(false),
        mQueueMutex(),
        mQueueNotEmptyCondition(),
        mQueue(),
        mMessageIdCounter(0),
        mSupervisor() {
    mQueue.reserve(kDefaultPoolSize);
}

MessageQueue::~MessageQueue() {
    shutDown();
}

Message *MessageQueue::obtainMessage() {
    return mMessagePool.obtain();
}

void MessageQueue::releaseMessage(Message *message) {
    message->performCleanup();
    mMessagePool.release(message);
}

void MessageQueue::beforeMessage(const Message &message) {
    if (auto supervisor = mSupervisor) {
        supervisor->beforeMessage(message);
    }
}

void MessageQueue::afterMessage(const Message &message) {
    if (auto supervisor = mSupervisor) {
        supervisor->afterMessage(message);
    }
}

void MessageQueue::setSupervisor(const std::shared_ptr<MessageQueue::Supervisor> &supervisor) {
    mSupervisor = supervisor;
}

void MessageQueue::shutDown() {
    mShutdown = true;
    {
        std::lock_guard<std::mutex> lk(mQueueMutex);
        for (auto i = 0; i < mQueue.size(); i++) {
            mQueue[i]->performCleanup();
        }
        mQueue.clear();

        // wake up looper
        mQueueNotEmptyCondition.notify_all();
    }

    while (mProcessingMessage.load()) {
        // spin
    }

    mMessagePool.cleanup();
}

int32_t MessageQueue::postMessage(const Message &message) {
    return postMessageDelayed(message, 0);
}

int32_t MessageQueue::postMessageDelayed(const Message &message, int64_t delayMillis) {
    assertNotShutDown();

    auto id = mMessageIdCounter++;
    auto msg = obtainMessage();
    *msg = message;
    msg->dueTime = timestamp() + delayMillis;
    msg->messageId = id;

    {
        std::lock_guard<std::mutex> lk(mQueueMutex);
        auto pos = findInsertPositionLocked(msg->dueTime);
        mQueue.insert(pos, msg);
    }
    mQueueNotEmptyCondition.notify_all();

    return id;
}

int32_t MessageQueue::postMessageAtFrontQueue(const Message &message) {
    assertNotShutDown();

    auto id = mMessageIdCounter++;
    auto msg = obtainMessage();
    *msg = message;
    msg->dueTime = timestamp();
    msg->messageId = id;

    {
        std::lock_guard<std::mutex> lk(mQueueMutex);
        mQueue.push_front(msg);
    }
    mQueueNotEmptyCondition.notify_all();

    return id;
}

int32_t MessageQueue::findInsertPositionLocked(int64_t dueTime) {
    int32_t begin = 0;
    int32_t end = mQueue.size() - 1;
    while (begin <= end) {
        int32_t mid = (begin + end) / 2;
        auto diff = mQueue[mid]->dueTime - dueTime;
        if (diff < 0) {
            begin = mid + 1;
        } else if (diff > 0) {
            end = mid - 1;
        } else {
            // insert at last position of same due time messages.
            auto i = mid + 1;
            for (; i <= end; ++i) {
                if (mQueue[i]->dueTime != dueTime) {
                    break;
                }
            }
            return i;
        }
    }
    return begin;
}

/*inline*/
void MessageQueue::assertNotShutDown() {
    assert(!mShutdown);
}

bool MessageQueue::removeMessage(int32_t messageId) {
    std::lock_guard<std::mutex> lk(mQueueMutex);
    bool removed = false;
    for (auto i = 0; i < mQueue.size();) {
        auto msg = mQueue[i];
        if (msg->messageId == messageId) {
            releaseMessage(msg);
            mQueue.remove(i);
            removed = true;
        } else {
            i++;
        }
    }
    return removed;
}

bool MessageQueue::removeMessageByWhat(int32_t what) {
    std::lock_guard<std::mutex> lk(mQueueMutex);
    bool removed = false;
    for (auto i = 0; i < mQueue.size();) {
        auto msg = mQueue[i];
        if (msg->what == what) {
            releaseMessage(msg);
            mQueue.remove(i);
            removed = true;
        } else {
            i++;
        }
    }
    return removed;
}

bool MessageQueue::removeMessageByHandlerProc(Message::MessageProc *proc) {
    std::lock_guard<std::mutex> lk(mQueueMutex);
    bool removed = false;
    for (auto i = 0; i < mQueue.size();) {
        auto msg = mQueue[i];
        if (msg->handlerProc == proc) {
            releaseMessage(msg);
            mQueue.remove(i);
            removed = true;
        } else {
            i++;
        }
    }
    return removed;
}

bool MessageQueue::hasDueMessageLocked() {
    return !mQueue.empty() && mQueue.front()->due();
}

void MessageQueue::loopQueue() {
    while (!mShutdown.load()) {
        Message *message;
        {
            std::unique_lock<std::mutex> lk(mQueueMutex);
            if (!hasDueMessageLocked()) {
                if (mQueue.empty()) {
                    mQueueNotEmptyCondition.wait(lk);
                } else {
                    auto timeToWait = mQueue.front()->dueTime - timestamp();
                    if (timeToWait > 0) {
                        mQueueNotEmptyCondition.wait_for(
                                lk, std::chrono::milliseconds(timeToWait));
                    }
                }
                continue;
            }
            message = mQueue.pop_front();
        }

        // process message
        mProcessingMessage = true;
        beforeMessage(*message);

        message->handle();

        afterMessage(*message);
        releaseMessage(message);
        mProcessingMessage = false;
    }
}

/*static*/
int64_t MessageQueue::timestamp() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}