
//  RQuantLib function DiscountCurve
//
//  Copyright (C) 2005 - 2007  Dominick Samperi
//  Copyright (C) 2007 - 2019  Dirk Eddelbuettel
//  Copyright (C) 2009 - 2011  Dirk Eddelbuettel and Khanh Nguyen
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

#include <rquantlib_internal.h>

// [[Rcpp::export]]
Rcpp::XPtr<QuantLib::YieldTermStructure> PiecewiseYieldCurve(
    const std::string& trait, const std::string& interpolator,
    int settlementDays,
    const Rcpp::XPtr<QuantLib::Calendar>& calendar,
    const Rcpp::XPtr<QuantLib::DayCounter>& termStructureDayCounter,
    Rcpp::List tslist,
    Rcpp::List legParams,
    double tolerance) {

    std::vector<std::string> tsNames = tslist.names();

    std::string firstQuoteName = tsNames[0];
    QuantLib::YieldTermStructure* curve;

    if (firstQuoteName == "flat") {            // Create a flat term structure.
        double rateQuote = Rcpp::as<double>(tslist[0]);
        curve = new QuantLib::FlatForward(settlementDays, *calendar, rateQuote,
                                          *termStructureDayCounter);
    } else {             // Build curve based on a set of observed rates and/or prices.
        std::vector<QuantLib::ext::shared_ptr<QuantLib::RateHelper> > curveInput;

        // For general swap inputs, not elegant but necessary to pass to getRateHelper()
        int fixDayCount = Rcpp::as<int>(legParams["dayCounter"]);
        int fixFreq   = Rcpp::as<int>(legParams["fixFreq"]) ;
        int floatFreq = Rcpp::as<int>(legParams["floatFreq"]);

        for(int i = 0; i < tslist.size(); i++) {
            std::string name = tsNames[i];
            double val = Rcpp::as<double>(tslist[i]);
            QuantLib::ext::shared_ptr<QuantLib::RateHelper> rh =
                ObservableDB::instance().getRateHelper(name, val,
                                                       fixDayCount,
                                                       fixFreq, floatFreq);
            if (rh.get() == NULL)
                throw std::range_error("Unknown rate in getRateHelper");
            curveInput.push_back(rh);
        }
        curve = getTermStructure(trait, interpolator, settlementDays,
                                 *calendar,
                                 curveInput,
                                 *termStructureDayCounter, tolerance);
        curve->enableExtrapolation();
    }
    return Rcpp::XPtr<QuantLib::YieldTermStructure>(curve, true);
}

// [[Rcpp::export]]
std::vector<double> discount(const Rcpp::XPtr<QuantLib::YieldTermStructure>& yts,
                             const std::vector<QuantLib::Date>& dates) {
    std::vector<double> results;
    for(std::vector<QuantLib::Date>::const_iterator it = dates.begin(); it != dates.end(); ++it) {
        results.push_back(yts->discount(*it));
    }
    return results;
}

// [[Rcpp::export]]
std::vector<double> zeroRate(const Rcpp::XPtr<QuantLib::YieldTermStructure>& yts,
                             const std::vector<QuantLib::Date>& dates,
                             const Rcpp::XPtr<QuantLib::DayCounter>& dc,
                             int comp = 2) {
    std::vector<double> results;
    for(std::vector<QuantLib::Date>::const_iterator it = dates.begin(); it != dates.end(); ++it) {
        results.push_back(yts->zeroRate(*it, *dc, static_cast<QuantLib::Compounding>(comp)));
    }
    return results;
}

// [[Rcpp::export]]
std::vector<double> forwardRate(const Rcpp::XPtr<QuantLib::YieldTermStructure>& yts,
                                const std::vector<QuantLib::Date>& dates,
                                const std::string& period,
                                const Rcpp::XPtr<QuantLib::DayCounter>& dc,
                                int comp = 2) {
    QuantLib::Period p = QuantLib::PeriodParser::parse(period);
    std::vector<double> results;
    for(std::vector<QuantLib::Date>::const_iterator it = dates.begin(); it != dates.end(); ++it) {
        results.push_back(yts->forwardRate(*it, p, *dc, static_cast<QuantLib::Compounding>(comp)));
    }
    return results;
}

// [[Rcpp::export]]
QuantLib::Date referenceDate(const Rcpp::XPtr<QuantLib::YieldTermStructure>& yts) {
    return yts->referenceDate();
}

// [[Rcpp::export]]
QuantLib::Date maxDate(const Rcpp::XPtr<QuantLib::YieldTermStructure>& yts) {
    return yts->maxDate();
}

// [[Rcpp::export]]
void setExtrapolation(const Rcpp::XPtr<QuantLib::YieldTermStructure>& yts, bool enabled) {
    if(enabled) {
        yts->enableExtrapolation();
    } else {
        yts->disableExtrapolation();
    }
}

// [[Rcpp::export]]
bool allowsExtrapolation(const Rcpp::XPtr<QuantLib::YieldTermStructure>& yts) {
    return yts->allowsExtrapolation();
}

// [[Rcpp::export]]
Rcpp::XPtr<QuantLib::Calendar> calendar(const Rcpp::XPtr<QuantLib::YieldTermStructure>& yts) {
    QuantLib::Calendar* cal = new QuantLib::Calendar(yts->calendar());
    return Rcpp::XPtr<QuantLib::Calendar>(cal, true);
}

// [[Rcpp::export]]
Rcpp::XPtr<QuantLib::DayCounter> dayCounter(const Rcpp::XPtr<QuantLib::YieldTermStructure>& yts) {
    QuantLib::DayCounter* dc = new QuantLib::DayCounter(yts->dayCounter());
    return Rcpp::XPtr<QuantLib::DayCounter>(dc, true);
}
