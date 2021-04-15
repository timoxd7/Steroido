#ifndef CALLBACK_H
#define CALLBACK_H

typedef uint32_t callback_instance_counter_type_t;

namespace steroido_intern {
    /**
     * @brief Callable Interface
     * 
     * @tparam R Return Value of the assigned callback function
     */
    template<typename R>
    class Callable {
        public:
            virtual ~Callable() {}
            virtual R call() = 0;
    };
};

template<typename R>
class Callback {
    public:
        /**
         * @brief Standard Constructor. Don't use, otherwise your programm will so dermaßend abschmieren, das glauben sie mir nicht
         * 
         */
        Callback() : _callback(nullptr) {
            _instanceCount = new callback_instance_counter_type_t;
            *_instanceCount = 0;
        }

        /**
         * @brief Construct a Callback using a Function (-pointer)
         * 
         * @param func Function to be called on call()
         */
        Callback(R(*func)()) {
            _callback = new FunctionCaller(func);
            _instanceCount = new callback_instance_counter_type_t;
            *_instanceCount = 1;
        }

        /**
         * @brief Construct a Callback using a method of an Instance of an Class
         * 
         * @tparam T 
         * @tparam U 
         * @param obj The Instance of the Object the Method should be called on
         * @param method The Method (-pointer) to the method of the Class which should be called
         */
        template<typename T, typename U>
        Callback(U* obj, R(T::*method)()) {
            _callback = new MethodCaller<T, U>(obj, method);
            _instanceCount = new callback_instance_counter_type_t;
            *_instanceCount = 1;
        }

        /**
         * @brief Destroy the Callback object
         * 
         */
        ~Callback() {
            _destruct();
        }
        
        /**
         * @brief Copy-Construct a Callback object
         * 
         * @param that 
         */
        Callback(const Callback<R> &that) {
            _copy(that);
        }

        /**
         * @brief Copy Assign
         * 
         * @param that 
         * @return Callback<R>& 
         */
        Callback<R>& operator=(const Callback<R> &that) {
            return _copy(that);
        }

        /**
         * @brief Call the Callback
         * 
         * @return R 
         */
        R call() {
            if (*_instanceCount) {
                return _callback->call();
            }
        }
    
    private:
        // Member Variables
        steroido_intern::Callable<R> *_callback;
        callback_instance_counter_type_t *_instanceCount;

        void _destruct() {
            if (!(*_instanceCount)) {
                delete _instanceCount;
            } else if (!(--(*_instanceCount))) {
                delete _callback;
                delete _instanceCount;
            }
        }

        Callback<R>& _copy(const Callback<R> &that) {
            _destruct();

            _callback = that._callback;
            _instanceCount = that._instanceCount;

            ++(*_instanceCount);
            return *this;
        }

        /**
         * @brief Specific caller using the Callable Interface
         * 
         */
        class FunctionCaller : public steroido_intern::Callable<R> {
            public:
                FunctionCaller(R(*func)()) : _func(func) {}

                R call() {
                    return (*_func)();
                }
            
            private:
                FunctionCaller() {}
                R (*_func)();
        };

        template<typename T, typename U>
        class MethodCaller : public steroido_intern::Callable<R> {
            public:
                MethodCaller(U *obj, R (T::*method)()) : _obj(obj), _method(method) {}

                R call() {
                    return (*_obj.*_method)();
                }
            
            private:
                MethodCaller() {}
                U *_obj;
                R (T::*_method)();
        };
};


// -------------- Functions for easier and faster access to a Callback

/**
 * @brief Callback for a single function
 * 
 * @tparam R 
 * @param func 
 * @return Callback<R> 
 */
template<typename R>
Callback<R> callback(R(*func)()) {
    return Callback<R>(func);
}

/**
 * @brief Callback for an Object's function
 * 
 * @tparam T 
 * @tparam U 
 * @tparam R 
 * @param obj 
 * @param method 
 * @return Callback<R> 
 */
template<typename T, typename U, typename R>
Callback<R> callback(U* obj, R(T::*method)()) {
    return Callback<R>(obj, method);
}

/**
 * @brief Callback for an Object's function
 * 
 * @tparam T 
 * @tparam U 
 * @tparam R 
 * @param obj 
 * @param method 
 * @return Callback<R> 
 */
template<typename T, typename U, typename R>
Callback<R> callback(U &obj, R(T::*method)()) {
    return Callback<R>(&obj, method);
}

#endif // CALLBACK_H