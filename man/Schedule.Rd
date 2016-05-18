% $Id$
\name{Schedule}
\alias{Schedule}
\alias{Schedule.default}
\alias{Schedule.list}
\title{Schedule generation}
\description{
  The \code{Schedule} function generates a schedule of dates conformant to
  a given convention in a given calendar.
}
\usage{
\method{Schedule}{default}(effectiveDate,
         maturityDate,
         period="Semiannual",
         calendar="TARGET",
         businessDayConvention= "Following",
         terminationDateConvention= "Following",
         dateGeneration= "Backward",
         endOfMonth=F)
\method{Schedule}{list}(params)
}
\arguments{
\item{effectiveDate}{a Date, when the schedule becomes effective.}
\item{maturityDate}{a Date, when the schedule matures.}
\item{period}{(Optional) a number or string, the frequency of the
  schedule. Default value is 'Semiannual'.}
\item{calendar}{(Optional) a string, the calendar name. Defaults to 'TARGET'}
\item{businessDayConvention}{(Optional) a number or string, the day
  convention to use. Defaults to 'Following'.}
\item{terminationDateConvention}{(Optional) a number or string, the
  day convention to use for the terminal date. Defaults to 'Following'.}
\item{dateGeneration}{(Optional) a number or string, the date
  generation rule. Defaults to 'Backward'.}
\item{endOfMonth}{(Optional) 1 or 0, use End of Month rule for
  schedule dates. Defaults to 0 (FALSE).}

Can also be called with

\item{params}{a named list, QuantLib's parameters of the schedule,
  taking the aforementioned arguments.}

See example below.
}

\value{
  The \code{Schedule} function returns an object of class
  \code{Schedule}. It contains the list of dates in the schedule.
}
\details{
  Please see any decent Finance textbook for background reading, and the
  \code{QuantLib} documentation for details on the \code{QuantLib}
  implementation.
}
\references{\url{http://quantlib.org} for details on \code{QuantLib}.}
\author{
  Michele Salvadore \email{michele.salvadore@gmail.com} for the inplementation;
  Dirk Eddelbuettel \email{edd@debian.org} for the \R interface;
  the QuantLib Group for \code{QuantLib}
}

\seealso{
  \code{\link{FixedRateBond}}
}

\examples{

params <- list(effectiveDate=as.Date("2004-11-30"),
               maturityDate=as.Date("2008-11-30"),
               period='Semiannual',
               calendar='UnitedStates/GovernmentBond',
               businessDayConvention='Unadjusted',
               terminationDateConvention='Unadjusted',
               dateGeneration='Forward',
               endOfMonth=1)
Schedule(params)
}
\keyword{misc}
