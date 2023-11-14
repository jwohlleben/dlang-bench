import core.stdc.time;

extern (C):

/* Defines for return codes */
enum TTRACKER_SUCCESS = 0x0; ///< Success

/**
 * Must be defined for dlang
 */
struct timespec {
    time_t tv_sec; ///< Seconds
    long tv_nsec; ///< Nanoseconds
}

/**
 * Represents a time interval from ts_start to ts_stop
 */
struct _ttracker_event_t
{
    timespec ts_start; ///< Start of interval
    timespec ts_stop; ///< Stop of interval
}

alias ttracker_event_t = _ttracker_event_t;

/**
 * Represents a tracker for time events
 */
struct _ttracker_t
{
    int status; ///< If not TTRACKER_SUCCESS, the measurement(s) failed
    ttracker_event_t* events; ///< All time events
    uint event_count; ///< Event count
}

alias ttracker_t = _ttracker_t;

/**
 * Initializes the time tracker
 *
 * @param tracker The tracker to initialize
 * @param events The events array
 * @pararm event_count Number of events to monitor
 */
void ttracker_init(ttracker_t* tracker, ttracker_event_t* events,
    uint event_count);

/**
 * Tracks a start event
 *
 * @param tracker The tracker to use
 * @pararm tid The event index. Must be 0 <= tid < event_count
 */
void ttracker_start(ttracker_t* tracker, uint tid);

/**
 * Tracks a stop event
 *
 * @param tracker The tracker to use
 * @pararm tid The event index. Must be 0 <= tid < event_count
 */
void ttracker_stop(ttracker_t* tracker, uint tid);

/**
 * Prints all tracked events in SECONDS.NANOSECONDS format separated by commas
 *
 * @param tracker The tracker to print
 */
void ttracker_print_sec(const(ttracker_t)* tracker);
