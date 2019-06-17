
// RQuantLib function prototypes and macros
//
// Copyright 2002 - 2019  Dirk Eddelbuettel <edd@debian.org>
// Copyright 2005 - 2006  Dominick Samperi
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with RcppArmadillo.  If not, see <http://www.gnu.org/licenses/>.

#ifndef rquantlib_h
#define rquantlib_h

#include <ql/quantlib.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <ql/shared_ptr.hpp>

//using namespace QuantLib;

#include <RcppCommon.h>

namespace Rcpp {

    // non-intrusive extension via template specialisation
    template <> QuantLib::Date as(SEXP dtsexp);

    // non-intrusive extension via template specialisation
    template <> SEXP wrap(const QuantLib::Date &d);

    // non-intrusive extension via template specialisation
    template <> std::vector<QuantLib::Date> as(SEXP dtvecsexp);

    // non-intrusive extension via template specialisation
    template <> SEXP wrap(const std::vector<QuantLib::Date> &dvec);
}

#include <Rcpp.h>


// Used to maintain context while in an R function.
class RQLContext : public QuantLib::Singleton<RQLContext> {
public:
    RQLContext() : calendar(new QuantLib::TARGET(), true) {
        settleDate = QuantLib::Date::todaysDate() + fixingDays;
    }
    // The tradeDate (evaluation date) is maintained by Settings,
    // (which is a singleton structure provided by QuantLib)
    // and used to translate between dates and real-valued times.
    QuantLib::Date settleDate;
    Rcpp::XPtr<QuantLib::Calendar> calendar;
    QuantLib::Integer fixingDays = 2;
};

// Instrument types used to construct the yield curve.
enum RQLObservableType { RQLDeposit, RQLSwap, RQLFuture, RQLFRA };

// Used to identify the specifics of a particular contract.
class RQLObservable {
public:
    RQLObservable(RQLObservableType type,
                  int n1, int n2,
                  QuantLib::TimeUnit units)
        : type_(type), n1_(n1), n2_(n2), units_(units) {}
    RQLObservableType getType() { return type_; }
    int getN1() { return n1_; }
    int getN2() { return n2_; }
    QuantLib::TimeUnit getUnits() { return units_; }
private:
    RQLObservableType type_;
    int n1_, n2_;    // n2 used for FRA's
    QuantLib::TimeUnit units_; // not used for futures and FRA's
};

typedef std::map<std::string, RQLObservable*> RQLMap;
typedef std::map<std::string, RQLObservable*>::const_iterator RQLMapIterator;

// Database used to maintain curve construction instrument details.
class ObservableDB : public QuantLib::Singleton<ObservableDB> {
public:
    ObservableDB();
    QuantLib::ext::shared_ptr<QuantLib::RateHelper> getRateHelper(std::string& ticker,
                                                          QuantLib::Rate r,
                                                          int fixDayCount = 6, //Thirty360
                                                          int fixFreq = 1, //Annual
                                                          int floatFreq = 6 ); //6 Months
private:
    RQLMap db_;
};

QuantLib::YieldTermStructure*
getTermStructure(const std::string& interpWhat, const std::string& interpHow,
                 const QuantLib::Date& settleDate,
                 const std::vector<QuantLib::ext::shared_ptr<QuantLib::RateHelper> >& curveInput,
                 QuantLib::DayCounter& dayCounter, QuantLib::Real tolerance);

QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure>
flatRate(const QuantLib::Date& today,
         double forward,
         const QuantLib::DayCounter& dc);

QuantLib::ext::shared_ptr<QuantLib::BlackVolTermStructure>
flatVol(const QuantLib::Date& today,
        const QuantLib::ext::shared_ptr<QuantLib::Quote>& vol,
        const QuantLib::DayCounter& dc);

enum EngineType {Analytic,
                 JR, CRR, EQP, TGEO, TIAN, LR, JOSHI,
                 FiniteDifferences, Integral,
                 PseudoMonteCarlo, QuasiMonteCarlo };

enum optionType { European = 0, American };

QuantLib::ext::shared_ptr<QuantLib::VanillaOption>
makeOption(const QuantLib::ext::shared_ptr<QuantLib::StrikedTypePayoff>& payoff,
           const QuantLib::ext::shared_ptr<QuantLib::Exercise>& exercise,
           const QuantLib::ext::shared_ptr<QuantLib::Quote>& u,
           const QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure>& q,
           const QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure>& r,
           const QuantLib::ext::shared_ptr<QuantLib::BlackVolTermStructure>& vol,
           EngineType engineType = Analytic,
           QuantLib::Size binomialSteps=128,
           QuantLib::Size samples=100);

QuantLib::ext::shared_ptr<QuantLib::GeneralizedBlackScholesProcess>
makeProcess(const QuantLib::ext::shared_ptr<QuantLib::Quote>& u,
            const QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure>& q,
            const QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure>& r,
            const QuantLib::ext::shared_ptr<QuantLib::BlackVolTermStructure>& vol);

int dateFromR(const Rcpp::Date &d); // using 'new' API's Rcpp::Date

//utility functions for parameters of fixed-income instrument function
QuantLib::IborIndex getIborIndex(std::string type);
QuantLib::DateGeneration::Rule getDateGenerationRule(const double n);
QuantLib::Frequency getFrequency(const int n);
QuantLib::TimeUnit getTimeUnit(const int n);
QuantLib::Compounding getCompounding(const int n);
QuantLib::BusinessDayConvention getBusinessDayConvention(const int n);
QuantLib::DayCounter getDayCounter(const int n);
QuantLib::DateGeneration::Rule getDateGenerationRule(const int n);
QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure> buildTermStructure(Rcpp::List params, Rcpp::List);
QuantLib::Schedule getSchedule(Rcpp::List rparam);
QuantLib::ext::shared_ptr<QuantLib::FixedRateBond> getFixedRateBond(Rcpp::List bondparam, std::vector<double> ratesVec, Rcpp::List scheduleparam);
QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure> getFlatCurve(Rcpp::List flatcurve);
QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure> rebuildCurveFromZeroRates(std::vector<QuantLib::Date> dates, std::vector<double> zeros);

QuantLib::ext::shared_ptr<QuantLib::IborIndex>
buildIborIndex(std::string type,
               const QuantLib::Handle<QuantLib::YieldTermStructure>& iborStrc);
Rcpp::XPtr<QuantLib::Calendar> getCalendar(const std::string &calstr);
QuantLib::Period periodByTimeUnit(int length, std::string unit);

// simple option type creator based on string
QuantLib::Option::Type getOptionType(const std::string &t);

// create a data.frame with dates and amounts
Rcpp::DataFrame getCashFlowDataFrame(const QuantLib::Leg &bondCashFlow);

// fill QL data structures based on data.frames
QuantLib::DividendSchedule getDividendSchedule(Rcpp::DataFrame dividendScheduleFrame);
QuantLib::CallabilitySchedule getCallabilitySchedule(Rcpp::DataFrame callabilityScheduleFrame);
QuantLib::Duration::Type getDurationType(const int n);

// dates.cpp
QuantLib::Date advanceDate(QuantLib::Date issueDate, int days);


#endif
