#include <time.h>
#include <stdio.h>

#include "ttracker.h"

void ttracker_init(ttracker_t* tracker, ttracker_event_t* events,
    unsigned int event_count)
{
    tracker->events = events;
    tracker->event_count = event_count;
    tracker->status = TTRACKER_SUCCESS;
}

void ttracker_start(ttracker_t* tracker, unsigned int tid)
{
    tracker->status |=
        clock_gettime(CLOCK_MONOTONIC_RAW, &tracker->events[tid].ts_start);
}

void ttracker_stop(ttracker_t* tracker, unsigned int tid)
{
    tracker->status |=
        clock_gettime(CLOCK_MONOTONIC_RAW, &tracker->events[tid].ts_stop);
}

void ttracker_print_sec(const ttracker_t* tracker)
{
    for (unsigned int i = 0; i < tracker->event_count; ++i)
    {
        const ttracker_event_t* event = &tracker->events[i];
        long sec = (long) (event->ts_stop.tv_sec - event->ts_start.tv_sec);
        long nsec = (long) (event->ts_stop.tv_nsec - event->ts_start.tv_nsec);

        /* Adjust time on overflow  */
        if (nsec < 0)
        {
            --sec;
            nsec += 1E9;
        }

        printf("%ld.%09ld", sec, nsec);

        if (i != tracker->event_count - 1)
        {
            printf(",");
        }
    }

    printf("\n");
}
