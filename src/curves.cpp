
//  RQuantLib -- R interface to the QuantLib libraries
//
//  Copyright (C) 2005 - 2007  Dominick Samperi
//  Copyright (C) 2007 - 2019  Dirk Eddelbuettel
//
//  This file is part of RQuantLib.
//
//  RQuantLib is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 2 of the License, or
//  (at your option) any later version.
//
//  RQuantLib is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with RQuantLib.  If not, see <http://www.gnu.org/licenses/>.

//  [[Rcpp::interfaces(r, cpp)]]

#include <rquantlib_internal.h>

// Database of interest rate instrument contract details.
ObservableDB::ObservableDB() {
    db_["d1w"] = new RQLObservable(RQLDeposit, 1, 0, QuantLib::Weeks);
    db_["d1m"] = new RQLObservable(RQLDeposit, 1, 0, QuantLib::Months);
    db_["d2m"] = new RQLObservable(RQLDeposit, 2, 0, QuantLib::Months);
    db_["d3m"] = new RQLObservable(RQLDeposit, 3, 0, QuantLib::Months);
    db_["d6m"] = new RQLObservable(RQLDeposit, 6, 0, QuantLib::Months);
    db_["d9m"] = new RQLObservable(RQLDeposit, 9, 0, QuantLib::Months);
    db_["d1y"] = new RQLObservable(RQLDeposit, 1, 0, QuantLib::Years);
    db_["s2y"] = new RQLObservable(RQLSwap, 2, 0, QuantLib::Years);
    db_["s3y"] = new RQLObservable(RQLSwap, 3, 0, QuantLib::Years);
    db_["s4y"] = new RQLObservable(RQLSwap, 4, 0, QuantLib::Years);
    db_["s5y"] = new RQLObservable(RQLSwap, 5, 0, QuantLib::Years);
    db_["s6y"] = new RQLObservable(RQLSwap, 6, 0, QuantLib::Years);
    db_["s7y"] = new RQLObservable(RQLSwap, 7, 0, QuantLib::Years);
    db_["s8y"] = new RQLObservable(RQLSwap, 8, 0, QuantLib::Years);
    db_["s9y"] = new RQLObservable(RQLSwap, 9, 0, QuantLib::Years);
    db_["s10y"] = new RQLObservable(RQLSwap, 10, 0, QuantLib::Years);
    db_["s12y"] = new RQLObservable(RQLSwap, 12, 0, QuantLib::Years);
    db_["s15y"] = new RQLObservable(RQLSwap, 15, 0, QuantLib::Years);
    db_["s20y"] = new RQLObservable(RQLSwap, 20, 0, QuantLib::Years);
    db_["s25y"] = new RQLObservable(RQLSwap, 25, 0, QuantLib::Years);
    db_["s30y"] = new RQLObservable(RQLSwap, 30, 0, QuantLib::Years);
    db_["s40y"] = new RQLObservable(RQLSwap, 40, 0, QuantLib::Years);
    db_["s50y"] = new RQLObservable(RQLSwap, 50, 0, QuantLib::Years);
    db_["s60y"] = new RQLObservable(RQLSwap, 60, 0, QuantLib::Years);
    db_["s70y"] = new RQLObservable(RQLSwap, 70, 0, QuantLib::Years);
    db_["s80y"] = new RQLObservable(RQLSwap, 80, 0, QuantLib::Years);
    db_["s90y"] = new RQLObservable(RQLSwap, 90, 0, QuantLib::Years);
    db_["s100y"] = new RQLObservable(RQLSwap, 100, 0, QuantLib::Years);
    db_["fut1"] = new RQLObservable(RQLFuture, 1, 0, QuantLib::Months);
    db_["fut2"] = new RQLObservable(RQLFuture, 2, 0, QuantLib::Months);
    db_["fut3"] = new RQLObservable(RQLFuture, 3, 0, QuantLib::Months);
    db_["fut4"] = new RQLObservable(RQLFuture, 4, 0, QuantLib::Months);
    db_["fut5"] = new RQLObservable(RQLFuture, 5, 0, QuantLib::Months);
    db_["fut6"] = new RQLObservable(RQLFuture, 6, 0, QuantLib::Months);
    db_["fut7"] = new RQLObservable(RQLFuture, 7, 0, QuantLib::Months);
    db_["fut8"] = new RQLObservable(RQLFuture, 8, 0, QuantLib::Months);
    db_["fra3x6"] = new RQLObservable(RQLFRA, 3, 6, QuantLib::Months);
    db_["fra6x9"] = new RQLObservable(RQLFRA, 6, 9, QuantLib::Months);
    db_["fra6x12"] = new RQLObservable(RQLFRA, 6, 12, QuantLib::Months);
}


// Get RateHelper used to build the yield curve corresponding to a
// database key ('ticker') and observed rate/price.
QuantLib::ext::shared_ptr<QuantLib::RateHelper> 
ObservableDB::getRateHelper(std::string& ticker, QuantLib::Rate r, int fixDayCount,
                            int fixFreq, int floatFreq) {
    RQLMapIterator iter = db_.find(ticker);
    if (iter == db_.end()) {
        std::string errortxt = "Unknown curve construction instrument: " + ticker;
        Rcpp::stop(errortxt);
    }
    RQLObservable *p = iter->second;
    RQLObservableType type = p->getType();
    int n1 = p->getN1(), n2 = p->getN2();
    QuantLib::TimeUnit units = p->getUnits();

    QuantLib::Date settlementDate = RQLContext::instance().settleDate;
    QuantLib::Calendar calendar = *RQLContext::instance().calendar;
    QuantLib::Integer fixingDays = RQLContext::instance().fixingDays;
    QuantLib::DayCounter depositDayCounter = QuantLib::Actual360();
    bool endOfMonth = false;

    QuantLib::ext::shared_ptr<QuantLib::RateHelper> rh;
    if (type == RQLDeposit) {
        rh = QuantLib::ext::make_shared<QuantLib::DepositRateHelper>(
            r, n1*units, fixingDays,
            calendar, QuantLib::ModifiedFollowing,
            endOfMonth, // false
            depositDayCounter);
    } else if (type == RQLSwap) {
        QuantLib::Frequency swFixedLegFrequency = getFrequency(fixFreq);
        QuantLib::BusinessDayConvention swFixedLegConvention = QuantLib::Unadjusted;
        QuantLib::DayCounter swFixedLegDayCounter = getDayCounter(fixDayCount);
        QuantLib::ext::shared_ptr<QuantLib::IborIndex>
            swFloatingLegIndex = QuantLib::ext::make_shared<QuantLib::IborIndex>(
                "WOIbor", floatFreq * QuantLib::Months,
                fixingDays, // settlement days
                QuantLib::EURCurrency(),
                calendar,
                QuantLib::ModifiedFollowing,
                endOfMonth, // false
                QuantLib::Actual360());
        rh = QuantLib::ext::make_shared<QuantLib::SwapRateHelper>(
            r, n1*QuantLib::Years,
            calendar, swFixedLegFrequency,
            swFixedLegConvention,
            swFixedLegDayCounter,
            swFloatingLegIndex);
    } else if (type == RQLFuture) {
        QuantLib::Integer futMonths = 3;
        QuantLib::Date imm = QuantLib::IMM::nextDate(settlementDate);
        for (int i = 1; i < n1; i++)
            imm = QuantLib::IMM::nextDate(imm+1);
        //Rcpp::Rcout << "Curves: IMM Date is " << imm << std::endl;
        rh  = QuantLib::ext::make_shared<QuantLib::FuturesRateHelper>(
            r, imm, futMonths, calendar,
            QuantLib::ModifiedFollowing,
            endOfMonth, // false
            depositDayCounter);
    } else if (type == RQLFRA) {
        rh = QuantLib::ext::make_shared<QuantLib::FraRateHelper>(r, n1, n2, fixingDays, calendar,
                                                                 QuantLib::ModifiedFollowing,
                                                                 endOfMonth, // false
                                                                 depositDayCounter);
    } else {
        Rcpp::stop("Bad type in curve construction");
    }
    return rh;
}

// Return the term structure built using a set of RateHelpers (curveInput)
// employing the specified interpolation method and day counter.
QuantLib::YieldTermStructure*
getTermStructure (const std::string& interpWhat, const std::string& interpHow,
                  const QuantLib::Natural settlementDays,
                  const QuantLib::Calendar& calendar,
                  const std::vector<QuantLib::ext::shared_ptr<QuantLib::RateHelper> >& curveInput,
                  QuantLib::DayCounter& dayCounter, QuantLib::Real tolerance) {

    QuantLib::YieldTermStructure* ts;
    if (interpWhat == "discount"  && interpHow == "linear" ) {
        ts = new QuantLib::PiecewiseYieldCurve<QuantLib::Discount,
                                               QuantLib::Linear>(
                                                   settlementDays,
                                                   calendar,
                                                   curveInput,
                                                   dayCounter,
                                                   tolerance);
    } else if (interpWhat == "discount"  && interpHow == "loglinear" ) {
        ts = new QuantLib::PiecewiseYieldCurve<QuantLib::Discount,
                                               QuantLib::LogLinear>(
                                                   settlementDays,
                                                   calendar,
                                                   curveInput,
                                                   dayCounter,
                                                   tolerance);
    } else if (interpWhat == "discount" && interpHow == "spline") {
        ts = new QuantLib::PiecewiseYieldCurve<QuantLib::Discount,
                                               QuantLib::Cubic>(
                                                   settlementDays,
                                                   calendar,
                                                   curveInput,
                                                   dayCounter,
                                                   tolerance);
    } else if (interpWhat == "forward"  && interpHow == "linear" ) {
        ts = new QuantLib::PiecewiseYieldCurve<QuantLib::ForwardRate,
                                               QuantLib::Linear>(
                                                   settlementDays,
                                                   calendar,
                                                   curveInput,
                                                   dayCounter,
                                                   tolerance);
    } else if (interpWhat == "forward"  && interpHow == "loglinear" ) {
        ts = new QuantLib::PiecewiseYieldCurve<QuantLib::ForwardRate,
                                               QuantLib::LogLinear>(
                                                   settlementDays,
                                                   calendar,
                                                   curveInput,
                                                   dayCounter,
                                                   tolerance);
    } else if (interpWhat == "forward"  && interpHow == "spline" ) {
            ts = new QuantLib::PiecewiseYieldCurve<QuantLib::ForwardRate,
                                                   QuantLib::Cubic>(
                                                       settlementDays,
                                                       calendar,
                                                       curveInput,
                                                       dayCounter,
                                                       tolerance);
    } else if (interpWhat == "zero" && interpHow == "linear" ) {
        ts = new QuantLib::PiecewiseYieldCurve<QuantLib::ZeroYield,
                                              QuantLib::Linear>(
                                                  settlementDays,
                                                  calendar,
                                                  curveInput,
                                                  dayCounter,
                                                  tolerance);
    } else if (interpWhat == "zero" && interpHow == "loglinear" ) {
        ts = new QuantLib::PiecewiseYieldCurve<QuantLib::ZeroYield,
                                               QuantLib::LogLinear>(
                                                   settlementDays,
                                                   calendar,
                                                   curveInput,
                                                   dayCounter,
                                                   tolerance);
    } else if (interpWhat == "zero" && interpHow == "spline" ) {
        ts = new QuantLib::PiecewiseYieldCurve<QuantLib::ZeroYield,
                                               QuantLib::Cubic>(
                                                   settlementDays,
                                                   calendar,
                                                   curveInput,
                                                   dayCounter,
                                                   tolerance);
    } else {
        Rcpp::Rcout << "interpWhat = " << interpWhat << std::endl;
        Rcpp::Rcout << "interpHow  = " << interpHow << std::endl;
        Rcpp::stop("What/How term structure options not recognized");
    }

    return ts;
}
