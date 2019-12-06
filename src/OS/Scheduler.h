#ifndef SCHEDULER_H
#define SCHEDULER_H

class Scheduler {
    public:
        static void run() {
            // Call single callables first (e.g. with not sleeptime)
            for (auto &element : callableSchedule) {
                element.call();
            }

            // Call each scheduled element if the sleeptime is over
            for (auto &element : scheduledSchedule) {
                if (element.callable->getSleepingSince() > element.callable->getSleepTime()) {
                    element.callable->resetSleepTimer();
                    element.call();
                }
            }
        }

        static void add(ScheduledCallable &callable) {
            _add<ScheduledCallable>(callable, scheduledSchedule);
        }

        static void add(ICallable &callable) {
            _add<ICallable>(callable, callableSchedule);
        }
        
        static void remove(ScheduledCallable &callable) {
            _remove<ScheduledCallable>(callable, scheduledSchedule);
        }

        static void remove(ICallable &callable) {
            _remove<ICallable>(callable, callableSchedule);
        }
    
    private:
        // Template Class for SchedulerElements. C has to be a ICallable or ScheduledCallable
        template<class C>
        class SchedulerElement {
            public:
                SchedulerElement(C &_callable) {
                    callable = &_callable;
                }

                void call() {
                    callable->call();
                }

                C *callable;
        };

        static std::vector<SchedulerElement<ICallable>> callableSchedule;
        static std::vector<SchedulerElement<ScheduledCallable>> scheduledSchedule;

        template<class C>
        static void _add(C &callable, std::vector<SchedulerElement<C>> &schedule) {
            // First check if already added
            for (auto &element : schedule) {
                if (element.callable == &callable) return; // -> element already added
            }

            #ifdef VECTOR_EMPLACE_BACK_ENABLED
                schedule.emplace_back((C)callable);
            #else
                SchedulerElement<C> element(callable);
                schedule.push_back(element);
            #endif
        }

        template<class C>
        static void _remove(C &callable, std::vector<SchedulerElement<C>> &schedule) {
            for (auto it = schedule.begin(); it != schedule.end(); ++it) {
                if (it->callable == &callable) {
                    schedule.erase(it);
                    return;
                }
            }
        }
};

#endif