#ifndef REPVICORE_INTERNAL_LOGGING_IMPL_HPP
#define REPVICORE_INTERNAL_LOGGING_IMPL_HPP

#include <cstdarg>
#include <string>

namespace repvicore_internal {

    class LogManager {
        public:

        void registerSink(); // put paramter here
        void log(const char *message);

        private:

        static constexpr const int MAX_LOG_MESSAGE_LENGTH = 1024;
    };
};


#endif // REPVICORE_INTERNAL_LOGGING_IMPL_HPP