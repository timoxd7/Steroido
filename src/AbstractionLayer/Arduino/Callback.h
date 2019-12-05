#ifndef CALLBACK_H
#define CALLBACK_H

typedef uint32_t callback_instance_counter_type_t;

template<typename R>
class Callback {
    public:
        /*
            Construct a Callback using a Function (-pointer)

            @param func Function to be called on call()
        */
        Callback(R(*func)()) {
            _callback = new FunctionCaller(func);
            _instanceCount = new callback_instance_counter_type_t;
            *_instanceCount = 1;
        }

        /*
            Construct a Callback using a method of an Instance of an Class

            @param obj    The Instance of the Object the Method should be called on
            @param method The Method (-pointer) to the method of the Class which should be called
        */
        template<typename T, typename U>
        Callback(U* obj, R(T::*method)()) {
            _callback = new MethodCaller<T, U>(obj, method);
            _instanceCount = new callback_instance_counter_type_t;
            *_instanceCount = 1;
        }

        ~Callback() {
            if (!(--(*_instanceCount))) {
                delete _callback;
                delete _instanceCount;
            }
        }

        Callback(const Callback &that) {
            ++(*_instanceCount);
        }

        R call() {
            return _callback->call();
        }
    
    private:
        Callback() {}

        // Callable Interface
        class Callable {
            virtual R call() = 0;
        };

        // Member Variables
        Callable *_callback;
        callback_instance_counter_type_t *_instanceCount;

        // Specific caller using the Callable Interface
        class FunctionCaller : public Callable {
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
        class MethodCaller : public Callable {
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


// Functions for easyer and faster access to a Callback

// Callback for a single function
template<typename R>
Callback<R> callback(R(*func)()) {
    return Callback<R>(func);
}

// Callback for an Object
template<typename T, typename U, typename R>
Callback<R> callback(U* obj, R(T::*method)()) {
    return Callback<R>(obj, method);
}

template<typename T, typename U, typename R>
Callback<R> callback(U &obj, R(T::*method)()) {
    return Callback<R>(&obj, method);
}

#endif // CALLBACK_H