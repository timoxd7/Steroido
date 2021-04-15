#ifndef NON_COPYABLE_H
#define NON_COPYABLE_H

/**
 * @brief Template for Compatibility reasons
 * 
 * @tparam T 
 */
template<typename T>
class NonCopyable {
    public:
        NonCopyable() {}

    private:
        /**
         * @brief Declare copy constructor as private. Any attempt to copy construct a NonCopyable will fail at compile time.
         * 
         */
        NonCopyable(const NonCopyable &);

        /**
         * @brief Declare copy assignment operator as private. Any attempt to copy assign a NonCopyable will fail at compile time.
         * 
         * @return NonCopyable& 
         */
        NonCopyable &operator=(const NonCopyable &);
};

#endif // NON_COPYABLE_H