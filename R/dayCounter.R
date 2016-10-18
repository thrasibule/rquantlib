DayCounter <- R6Class("DayCounter",
                      public = list(
                          initialize = function(name) {
                              private$ptr <- getDaycounter(matchDayCounter(name))
                          },
                          dayCount = function(startDates, endDates) {
                              dayCount(private$ptr, startDates, endDates)
                          },
                          yearFraction = function(startDates, endDates) {
                              yearFraction(private$ptr, startDates, endDates)
                          }
                          ),
                      private = list(
                          ptr = NULL
                          ),
                      active = list(
                          name = function() getDayCounterName(private$ptr)
                          )
                      )
