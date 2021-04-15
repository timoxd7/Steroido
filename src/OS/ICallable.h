#ifndef ICALLABLE_H
#define ICALLABLE_H

/**
 * @brief Interface for a Callable
 * 
 */
class ICallable : private NonCopyable<ICallable> {
    public:
        virtual void call() = 0;
};

#endif