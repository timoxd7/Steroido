#ifndef ICALLABLE_H
#define ICALLABLE_H

class ICallable : private NonCopyable<ICallable> {
    public:
        virtual void call() = 0;
};

#endif