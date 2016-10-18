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
                            if(!missing(name)) {
                                private$ptr <- getCalendar(name)
                            }
                        },
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

                        businessDaysBetween = function(from, to,
                            includeFirst = TRUE,
                            includeLast = FALSE) {
                            businessDaysBetween(private$ptr, from, to,
                                                includeFirst, includeLast)
                        },
                        adjust = function(dates, bdc = "Following") {
                            stopifnot(class(dates) == "Date")
                            adjust(private$ptr, dates, matchBDC(bdc))
                        },
                        advance = function(dates, n, timeUnit, period,
                            bdc = "Following", emr = FALSE) {
                            stopifnot(class(dates) == "Date")
                            call1 <- missing(period) && !missing(n) && !missing(timeUnit)
                            call2 <- !missing(period) && missing(n) && missing(timeUnit)
                            stopifnot(call1 | call2)
                            val <- NULL
                            if (call1) {
                                val <- advance1(private$ptr, n, timeUnit, matchBDC(bdc), emr, dates)
                            }
                            if (call2) {
                                stopifnot(is.character(period))
                                val <- advance2(private$ptr, period, matchBDC(bdc), emr, dates)
                            }
                            stopifnot( !is.null(val) )
                            val
                        },
                        getHolidayList = function(from = Sys.Date(), to=Sys.Date() + 5,
                            includeWeekends=FALSE) {
                            getHolidayList(private$ptr, from, to, includeWeekends)
                        }
                    ),
                    private = list(
                        ptr = NULL
                        ),
                    active = list(
                        name = function() getCalendarName(private$ptr)
                        )
                    )
