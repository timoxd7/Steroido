#ifndef NON_COPYABLE_H
#define NON_COPYABLE_H

template<typename T> // Template for Compatibility reasons
class NonCopyable {
    public:
        NonCopyable() {}

    private:
        /*
            Declare copy constructor as private. Any attempt to copy construct
            a NonCopyable will fail at compile time.
        */
        NonCopyable(const NonCopyable &);

        /*
            Declare copy assignment operator as private. Any attempt to copy assign
            a NonCopyable will fail at compile time.
        */
        NonCopyable &operator=(const NonCopyable &);
};

#endif // NON_COPYABLE_H