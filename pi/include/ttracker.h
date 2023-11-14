#ifndef TTRACKER_H
#define TTRACKER_H

#include <time.h>

/* Defines for return codes */
#define TTRACKER_SUCCESS 0x0 ///< Success

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Represents a time interval from ts_start to ts_stop
 */
typedef struct _ttracker_event_t
{
    struct timespec ts_start; ///< Start of interval
    struct timespec ts_stop;  ///< Stop of interval
} ttracker_event_t;

/**
 * Represents a tracker for time events
 */
typedef struct _ttracker_t
{
    int status; ///< If not TTRACKER_SUCCESS, the measurement(s) failed
    ttracker_event_t* events; ///< All time events
    unsigned int event_count; ///< Event count
} ttracker_t;

/**
 * Initializes the time tracker
 *
 * @param tracker The tracker to initialize
 * @param events The events array
 * @pararm event_count Number of events to monitor
 */
void ttracker_init(ttracker_t* tracker, ttracker_event_t* events,
    unsigned int event_count);

/**
 * Tracks a start event
 *
 * @param tracker The tracker to use
 * @pararm tid The event index. Must be 0 <= tid < event_count
 */
void ttracker_start(ttracker_t* tracker, unsigned int tid);

/**
 * Tracks a stop event
 *
 * @param tracker The tracker to use
 * @pararm tid The event index. Must be 0 <= tid < event_count
 */
void ttracker_stop(ttracker_t* tracker, unsigned int tid);

/**
 * Prints all tracked events in SECONDS.NANOSECONDS format separated by commas
 *
 * @param tracker The tracker to print
 */
void ttracker_print_sec(const ttracker_t* tracker);

#ifdef __cplusplus
}
#endif

#endif
