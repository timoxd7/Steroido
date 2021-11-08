#ifndef FLOAT_FOLLOWER_H
#define FLOAT_FOLLOWER_H

/**
 * @brief An float Follower that will follow an set value in a linear way.
 * Good to filter analog values, giving it a predefined rise and fall time.
 * 
 */
class FloatFollower : private NonCopyable<FloatFollower> {
    public:
        FloatFollower(float presetValue = 0.0f, float riseAmount = 0.0f, float fallAmount = 0.0f)
            : _currentValue(presetValue),
              _lastSetValue(presetValue),
              _riseAmount(riseAmount),
              _fallAmount(fallAmount) {
            _elapsedTime.reset();
            _elapsedTime.start();
        }

        float set(float newSetValue) {
            _lastSetValue = newSetValue;

            float deltaSeconds = _elapsedTime.read();
            _elapsedTime.reset();
            _elapsedTime.start();


            if (newSetValue > _currentValue) {
                // -> Rising
                float difference = newSetValue - _currentValue;
                float possibleChange = _riseAmount * deltaSeconds;

                // Get the actual rise amount
                if (possibleChange >= difference) {
                    _currentValue = newSetValue;
                } else {
                    _currentValue += possibleChange;
                }
            } else if (newSetValue < _currentValue) {
                // -> Falling
                float difference = _currentValue - newSetValue;
                float possibleChange = _fallAmount * deltaSeconds;

                if (possibleChange >= difference) {
                    _currentValue = newSetValue;
                } else {
                    _currentValue -= possibleChange;
                }
            }

            return _currentValue;
        }

        float get() {
            return set(_lastSetValue);
        }

        /**
         * @brief Set the amount the Value should be able to rise in one second
         * 
         * @param riseAmount 
         */
        void setRiseAmount(float& riseAmount) {
            _riseAmount = riseAmount;
        }

        /**
         * @brief Set the amount the Value should be able to fall in one second
         * 
         * @param fallAmount 
         */
        void setFallAmount(float& fallAmount) {
            _fallAmount = fallAmount;
        }
    
    private:
        float _currentValue;
        float _lastSetValue;

        float _riseAmount;
        float _fallAmount;

        Timer _elapsedTime;
};

#endif // FLOAT_FOLLOWER_H