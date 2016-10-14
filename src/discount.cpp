
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
Rcpp::XPtr<QuantLib::YieldTermStructure> discountCurveEngine(Rcpp::List rparams,
                                                             Rcpp::List tslist,
                                                             Rcpp::List legParams) {

    std::vector<std::string> tsNames = tslist.names();

    QuantLib::Date todaysDate(Rcpp::as<QuantLib::Date>(rparams["tradeDate"]));
    QuantLib::Date settlementDate(Rcpp::as<QuantLib::Date>(rparams["settleDate"]));
    RQLContext::instance().settleDate = settlementDate;

    QuantLib::SavedSettings backup;

    QuantLib::Settings::instance().evaluationDate() = todaysDate;
    std::string firstQuoteName = tsNames[0];

    std::string interpWhat, interpHow;
    if (firstQuoteName.compare("flat") != 0) {
        // Get interpolation method (not needed for "flat" case)
        interpWhat = Rcpp::as<std::string>(rparams["interpWhat"]);
        interpHow  = Rcpp::as<std::string>(rparams["interpHow"]);
    }
    // initialise from the singleton instance
    QuantLib::Calendar calendar = RQLContext::instance().calendar;

    QuantLib::DayCounter termStructureDayCounter = QuantLib::Actual365Fixed();
    double tolerance = 1e-12;
    QuantLib::YieldTermStructure* curve;

    if (firstQuoteName.compare("flat") == 0) {            // Create a flat term structure.
        double rateQuote = Rcpp::as<double>(tslist[0]);
        curve = flatRate(settlementDate, rateQuote, QuantLib::Actual365Fixed()).get();

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
        curve = getTermStructure(interpWhat, interpHow, settlementDate,
                                 curveInput, termStructureDayCounter, tolerance);
        curve->enableExtrapolation();
    }
    return Rcpp::XPtr<QuantLib::YieldTermStructure>(curve, true);
}

// [[Rcpp::export]]
std::vector<double> discountFactors(const Rcpp::XPtr<QuantLib::YieldTermStructure>& yts,
                                    const std::vector<QuantLib::Date>& dates) {
    std::vector<double> results;
    for(std::vector<QuantLib::Date>::const_iterator it = dates.begin(); it != dates.end(); ++it) {
        results.push_back(yts->discount(*it));
    }
    return results;
}

// [[Rcpp::export]]
std::vector<double> zeroRates(const Rcpp::XPtr<QuantLib::YieldTermStructure>& yts,
                              const std::vector<QuantLib::Date>& dates) {
    std::vector<double> results;
    for(std::vector<QuantLib::Date>::const_iterator it = dates.begin(); it != dates.end(); ++it) {
        results.push_back(yts->zeroRate(*it, QuantLib::ActualActual(), QuantLib::Continuous));
    }
    return results;
}

// [[Rcpp::export]]
std::vector<double> forwardRates(const Rcpp::XPtr<QuantLib::YieldTermStructure>& yts,
                                 const std::vector<QuantLib::Date>& dates,
                                 std::string period) {
    QuantLib::Period p = QuantLib::PeriodParser::parse(period);
    std::vector<double> results;
    for(std::vector<QuantLib::Date>::const_iterator it = dates.begin(); it != dates.end(); ++it) {
        results.push_back(yts->forwardRate(*it, p, QuantLib::ActualActual(), QuantLib::Continuous));
    }
    return results;
}

// [[Rcpp::export]]
QuantLib::Date referenceDate(const Rcpp::XPtr<QuantLib::YieldTermStructure>& yts) {
    return yts->referenceDate();
}
