## RQuantLib -- R interface to the QuantLib libraries
##
## Copyright (C) 2002 - 2009 Dirk Eddelbuettel
## Copyright (C) 2010        Dirk Eddelbuettel and Khanh Nguyen
##
## $Id$
##
## This file is part of the RQuantLib library for GNU R.
## It is made available under the terms of the GNU General Public
## License, version 2, or at your option, any later version,
## incorporated herein by reference.
##
## This program is distributed in the hope that it will be
## useful, but WITHOUT ANY WARRANTY; without even the implied
## warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
## PURPOSE.  See the GNU General Public License for more
## details.
##
## You should have received a copy of the GNU General Public
## License along with this program; if not, write to the Free
## Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
## MA 02111-1307, USA

Calendar <- R6Class("Calendar",
                    public = list(
                        initialize = function(name) {
                            private$ptr <- getCalendar(name)
                        }
                    ),
                    private = list(
                        ptr = NULL
                    ),
                    active = list(
                        name = function() getCalendarName(private$ptr),
                        isBusinessDay = function(dates) {
                            isBusinessDay(private$ptr, dates)
                        },
                        isHoliday = function(dates) {
                            isHoliday(private$ptr, dates)
                        },
                        isWeekend = function(dates) {
                            isWeekend(private$ptr, dates)
                        },
                        isEndOfMonth = function(dates) {
                            isEndOfMonth(private$ptr, dates)
                        },
                        endOfMonth = function(dates) {
                            EndOfMonth(private$ptr, dates)
                        },
                        adjust = function(dates, bdc = 0) {
                            adjust(private$ptr, dates, bdc)
                        },
                        businessDaysBetween = function(from, to,
                                                       includeFirst = TRUE,
                                                       includeLast = FALSE) {
                            businessDaysBetween(private$ptr, from, to,
                                                includeFirst, includeLast)
                        })
                    )

advance <- function(calendar="TARGET", dates=Sys.Date(),
                    n, timeUnit, # call 1
                    period,      # call 2
                    bdc = 0, emr = FALSE) {
    stopifnot(is.character(calendar))
    stopifnot(class(dates)=="Date")
    call1 <- missing(period) && !missing(n) && !missing(timeUnit)
    call2 <- !missing(period) && missing(n) && missing(timeUnit)
    stopifnot(call1 | call2)
    val <- NULL
    if (call1) {
        val <- advance1(calendar, n, timeUnit, bdc, emr, dates)
    }
    if (call2) {
        val <- advance2(calendar, period, bdc, emr, dates)
    }
    stopifnot( !is.null(val) )
    val
}


holidayList <- function(calendar="TARGET", from=Sys.Date(), to=Sys.Date() + 5, includeWeekends=FALSE) {
    getHolidayList(calendar, from, to, includeWeekends)
}
