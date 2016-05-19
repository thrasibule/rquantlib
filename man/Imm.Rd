\name{IMM}
\alias{IMMcode}
\alias{IMMdate}
\alias{nextIMMdate}
\alias{isIMMdate}
\alias{isIMMcode}
\alias{nextIMMcode}
\title{IMM functions from QuantLib}
\description{
  pomme
}
\usage{
IMMcode(date)
IMMdate(code, referenceDate = NULL)
nextIMMdate(date, mainCycle = TRUE)
isIMMdate(date, mainCycle = TRUE)
isIMMcode(code, mainCycle = TRUE)
nextIMMcode(date, mainCycle = TRUE)
}
\arguments{
  \item{date}{A \code{Date}}
  \item{code}{A string which represents a valid IMM code}
  \item{mainCycle}{A boolean}
}
\value{
  \code{isIMMdate}, and \code{isIMMcode} return a boolean.
  \code{IMMdate} returns a date or throws an error.
  \code{nextIMMdate} returns a date.
  \code{IMMcode} returns an IMM code or throws an error.
  \code{nextIMMcode} returns an IMM code.
}
\details{
}
\references{\url{http://quantlib.org} for details on \code{QuantLib}.}
\author{Guillaume Horel \email{guillaume.horel@gmail.com} for the \R interface;
  the QuantLib Group for \code{QuantLib}}
\note{The interface might change in future release as \code{QuantLib}
  stabilises its own API.}
\examples{
  IMMdate("H3") ## 2013-03-20
  IMMcode(IMMdate("H3")) ## H3

  IMMcode(nextIMMdate(Sys.Date()))
  nextIMMcode(Sys.Date()) ## same as previous

  isIMMcode("G5") ## TRUE
}
\keyword{misc}
