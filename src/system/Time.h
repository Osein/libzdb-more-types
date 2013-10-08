/*
 * Copyright (C) Tildeslash Ltd. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 *
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.
 */


#ifndef TIME_INCLUDED
#define TIME_INCLUDED
#include "SQLDateTime.h"


/**
 * <b>Time</b> is an abstraction of date and time. Time is stored internally 
 * as the number of seconds and microseconds since the epoch, <i>January 1, 
 * 1970 00:00 UTC</i>. 
 *
 * @file
 */


/** @name Class methods */
//@{


/**
 * Returns a Unix timestamp representation of the parsed string in the 
 * local timezone.
 * @return A local time representation of <code>t</code> or 0 if 
 * <code>t</code> is NULL
 * @exception SQLException if the parameter value cannot be converted
 * to a valid timestamp
 * @see SQLException.h
 */
time_t Time_toTimestamp(const char *t);


/**
 * Returns a Date representation of the parsed string in the
 * local timezone.
 * @return A sqldate_t structure representing the date of <code>t</code>
 * in the local timezone.
 * @exception SQLException if the parameter value cannot be converted
 * to a valid Date, for instance if <code>t</code> is NULL
 * @see SQLException.h
 */
sqldate_t Time_toDate(const char *t);


/**
 * Returns a Time representation of the parsed string in the
 * local timezone.
 * @return A sqltime_t structure representing the time of <code>t</code>
 * in the local timezone.
 * @exception SQLException if the parameter value cannot be converted
 * to a valid Time, for instance if <code>t</code> is NULL
 * @see SQLException.h
 */
sqltime_t Time_toTime(const char *t);


/**
 * Returns a DateTime representation of the parsed string in the
 * local timezone.
 * @return A sqldatetime_t structure representing the datetime of 
 * <code>t</code> in the local timezone.
 * @exception SQLException if the parameter value cannot be converted
 * to a valid DateTime, for instance if <code>t</code> is NULL
 * @see SQLException.h
 */
sqldatetime_t Time_toDateTime(const char *t);


/**
 * Returns the time since the Epoch (00:00:00 UTC, January 1, 1970),
 * measured in seconds. 
 * @return A time_t representing the current local time since the epoch
 * @exception AssertException If time could not be obtained
 */
time_t Time_now(void);


/**
 * Returns the time since the Epoch (00:00:00 UTC, January 1, 1970),
 * measured in milliseconds. 
 * @return A 64 bits long representing the current local time since 
 * the epoch in milliseconds
 * @exception AssertException If time could not be obtained
 */
long long int Time_milli(void);


/**
 * This method suspend the calling process or Thread for
 * <code>u</code> micro seconds.
 * @param u Micro seconds to sleep
 * @return true
 */
int Time_usleep(long u);

//@}

#undef T
#endif
