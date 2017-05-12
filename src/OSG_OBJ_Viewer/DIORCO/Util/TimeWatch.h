#pragma once

#include <DIORCO/DIORCO_Common.h>

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <ctime>

namespace DIORCO {

	class TimeWatch {
	public:
#ifdef _WIN32
		typedef DWORD time_t;

		TimeWatch();
		~TimeWatch();

		void start();
		void end();

		time_t sec();
		time_t msec();
		time_t usec();
		time_t current();

#else
#if defined(__unix__) || defined(__APPLE__)
		typedef unsigned long int time_t;

		void start();
		void end();

		time_t sec();
		time_t msec();
		time_t usec();
		time_t current();

#else  // C timer
		// using namespace std;
		typedef clock_t time_t;

		void start();
		void end();

		time_t sec();
		time_t msec();
		time_t usec();
		time_t current();

#endif
#endif

	private:
#ifdef _WIN32
		DWORD t_[2];
#else
#if defined(__unix__) || defined(__APPLE__)
		struct timeval tv[2];
		struct timezone tz;
#else
		time_t t_[2];
#endif
#endif
	};

}