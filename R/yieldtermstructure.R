YieldTermStructure <- R6Class("YieldTermStructure",
                              public = list(
                                  initialize =
                                  function(trait = c("discount", "forward", "zero"),
                                           interpolator = c("linear", "loglinear", "spline"),
                                           settlementDays = 0L, calendar = Calendar$new("WeekendsOnly"),
                                           dayCounter = DayCounter$new("Actual365Fixed"),
                                           tslist, legParams, tol = 1e-12) {
                                      stopifnot(is.character(trait))
                                      stopifnot(is.character(interpolator))
                                      stopifnot(is.numeric(settlementDays))
                                      stopifnot(inherits(calendar, "Calendar"))
                                      stopifnot(inherits(dayCounter, "DayCounter"))
                                      trait <- match.arg(trait)
                                      interpolator <- match.arg(interpolator)
                                      legParams$floatFreq <- matchFloatFrequency(legParams$floatFreq)
                                      legParams$fixFreq <- matchFrequency(legParams$fixFreq)
                                      legParams$dayCounter <- matchDayCounter(legParams$dayCounter)
                                      private$ptr <-
                                          PiecewiseYieldCurve(trait,
                                                              interpolator,
                                                              settlementDays,
                                                              calendar$.__enclos_env__$private$ptr,
                                                              dayCounter$.__enclos_env__$private$ptr,
                                                              tslist, legParams, tol)
                                  },
                                  discount = function(dates) {
                                      stopifnot(inherits(dates, "Date"))
                                      return( discount(private$ptr, dates) )
                                  },
                                  zeroRate = function(dates,
                                      dayCounter = DayCounter$new("Actual365Fixed")) {
                                      stopifnot(inherits(dates, "Date"))
                                      stopifnot(inherits(dayCounter, "DayCounter"))
                                      return( zeroRate(private$ptr,
                                                       dates,
                                                       dayCounter$.__enclos_env__$private$ptr))
                                  },
                                  forwardRate = function(dates, period = "3m",
                                      dayCounter = DayCounter$new("Actual365Fixed")) {
                                      stopifnot(inherits(dates, "Date"))
                                      stopifnot(inherits(dayCounter, "DayCounter"))
                                      stopifnot(is.character(period))
                                      return( forwardRate(private$ptr, dates, period,
                                                          dayCounter$.__enclos_env__$private$ptr) )
                                  }),
                              private = list(
                                  ptr = NULL),
                              active = list(
                                  extrapolation = function(enabled) {
                                      if (missing(enabled)) {
                                          return( allowsExtrapolation(private$ptr) )
                                      } else {
                                          setExtrapolation(private$ptr, enabled)
                                      }
                                  },
                                  referenceDate = function() referenceDate(private$ptr),
                                  calendar = function() {
                                      cal <- Calendar$new()
                                      cal$.__enclos_env__$private$ptr <- calendar(private$ptr)
                                      return( cal )
                                  },
                                  dayCounter = function() {
                                      dc <- DayCounter$new()
                                      dc$.__enclos_env__$private$ptr <- dayCounter(private$ptr)
                                      return( dc )
                                  }
                                  )
                              )
