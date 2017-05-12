#include "stdafx.h"
#include "TimeWatch.h"

namespace DIORCO {

#ifdef _WIN32
		TimeWatch::TimeWatch() { ::timeBeginPeriod(1); }
		TimeWatch::~TimeWatch() { ::timeEndPeriod(1); }

		void TimeWatch::start() { t_[0] = ::timeGetTime(); }
		void TimeWatch::end() { t_[1] = ::timeGetTime(); }

		TimeWatch::time_t TimeWatch::sec() { return (time_t)((t_[1] - t_[0]) / 1000); }
		TimeWatch::time_t TimeWatch::msec() { return (time_t)((t_[1] - t_[0])); }
		TimeWatch::time_t TimeWatch::usec() { return (time_t)((t_[1] - t_[0]) * 1000); }
		TimeWatch::time_t TimeWatch::current() { return ::timeGetTime(); }

#else
#if defined(__unix__) || defined(__APPLE__)
		void TimeWatch::start() { gettimeofday(tv + 0, &tz); }
		void TimeWatch::end() { gettimeofday(tv + 1, &tz); }

		TimeWatch::time_t TimeWatch::sec() { return (time_t)(tv[1].tv_sec - tv[0].tv_sec); }
		TimeWatch::time_t TimeWatch::msec() {
			return this->sec() * 1000 +
				(time_t)((tv[1].tv_usec - tv[0].tv_usec) / 1000);
		}
		TimeWatch::time_t TimeWatch::usec() {
			return this->sec() * 1000000 + (time_t)(tv[1].tv_usec - tv[0].tv_usec);
		}
		TimeWatch::time_t TimeWatch::current() {
			struct timeval t;
			gettimeofday(&t, NULL);
			return (time_t)(t.tv_sec * 1000 + t.tv_usec);
		}

#else  // C timer
		void TimeWatch::start() { t_[0] = clock(); }
		void TimeWatch::end() { t_[1] = clock(); }

		TimeWatch::time_t TimeWatch::sec() { return (time_t)((t_[1] - t_[0]) / CLOCKS_PER_SEC); }
		TimeWatch::time_t TimeWatch::msec() { return (time_t)((t_[1] - t_[0]) * 1000 / CLOCKS_PER_SEC); }
		TimeWatch::time_t TimeWatch::usec() { return (time_t)((t_[1] - t_[0]) * 1000000 / CLOCKS_PER_SEC); }
		TimeWatch::time_t TimeWatch::current() { return (time_t)clock(); }
#endif
#endif
}