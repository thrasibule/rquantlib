
//  RQuantLib -- R interface to the QuantLib libraries
//
//  Copyright (C) 2002 - 2019  Dirk Eddelbuettel <edd@debian.org>
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

// [[Rcpp::interfaces(r, cpp)]]

// [[Rcpp::export]]
double europeanOptionImpliedVolatilityEngine(std::string type,
                                             double value,
                                             double underlying,
                                             double strike,
                                             double dividendYield,
                                             double riskFreeRate,
                                             double maturity,
                                             double volatility) {

    const QuantLib::Size maxEvaluations = 100;
    const double tolerance = 1.0e-6;

#ifdef QL_HIGH_RESOLUTION_DATE
    // in minutes
    boost::posix_time::time_duration length = boost::posix_time::minutes(boost::uint64_t(maturity * 360 * 24 * 60));
#else
    int length = int(maturity*360 + 0.5); // FIXME: this could be better
#endif

    QuantLib::Option::Type optionType = getOptionType(type);

    QuantLib::Date today = QuantLib::Settings::instance().evaluationDate();

    // new framework as per QuantLib 0.3.5
    // updated for 0.3.7
    QuantLib::DayCounter dc = QuantLib::Actual360();

    QuantLib::ext::shared_ptr<QuantLib::SimpleQuote> spot = QuantLib::ext::make_shared<QuantLib::SimpleQuote>(underlying);
    QuantLib::ext::shared_ptr<QuantLib::SimpleQuote> vol = QuantLib::ext::make_shared<QuantLib::SimpleQuote>(volatility);
    QuantLib::ext::shared_ptr<QuantLib::BlackVolTermStructure> volTS = flatVol(today, vol, dc);
    QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure> qTS = flatRate(today, dividendYield, dc);
    QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure> rTS = flatRate(today, riskFreeRate, dc);
#ifdef QL_HIGH_RESOLUTION_DATE
    QuantLib::Date exDate(today.dateTime() + length);
#else
    QuantLib::Date exDate = today + length;
#endif
    QuantLib::ext::shared_ptr<QuantLib::Exercise> exercise = QuantLib::ext::make_shared<QuantLib::EuropeanExercise>(exDate);
    QuantLib::ext::shared_ptr<QuantLib::StrikedTypePayoff> payoff = QuantLib::ext::make_shared<QuantLib::PlainVanillaPayoff>(optionType, strike);
    QuantLib::ext::shared_ptr<QuantLib::VanillaOption>
        option = makeOption(payoff, exercise, spot, qTS, rTS,
                            volTS, Analytic,
                            QuantLib::Null<QuantLib::Size>(),
                            QuantLib::Null<QuantLib::Size>());

    QuantLib::ext::shared_ptr<QuantLib::GeneralizedBlackScholesProcess>
        process = makeProcess(spot, qTS, rTS,volTS);

    double volguess = volatility;
    vol->setValue(volguess);

    return option->impliedVolatility(value, process, tolerance, maxEvaluations);
}

// [[Rcpp::export]]
double americanOptionImpliedVolatilityEngine(std::string type,
                                             double value,
                                             double underlying,
                                             double strike,
                                             double dividendYield,
                                             double riskFreeRate,
                                             double maturity,
                                             double volguess,
                                             int timesteps,
                                             int gridpoints) {

    const QuantLib::Size maxEvaluations = 100;
    const double tolerance = 1.0e-6;

#ifdef QL_HIGH_RESOLUTION_DATE
    // in minutes
    boost::posix_time::time_duration length = boost::posix_time::minutes(boost::uint64_t(maturity * 360 * 24 * 60));
#else
    int length = int(maturity*360 + 0.5); // FIXME: this could be better
#endif

    QuantLib::Option::Type optionType = getOptionType(type);

    QuantLib::Date today = QuantLib::Settings::instance().evaluationDate();

    // new framework as per QuantLib 0.3.5
    QuantLib::DayCounter dc = QuantLib::Actual360();
    QuantLib::ext::shared_ptr<QuantLib::SimpleQuote> spot = QuantLib::ext::make_shared<QuantLib::SimpleQuote>(underlying);
    QuantLib::ext::shared_ptr<QuantLib::SimpleQuote> vol = QuantLib::ext::make_shared<QuantLib::SimpleQuote>(volguess);
    QuantLib::ext::shared_ptr<QuantLib::BlackVolTermStructure> volTS = flatVol(today, vol,dc);
    QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure> qTS = flatRate(today, dividendYield, dc);
    QuantLib::ext::shared_ptr<QuantLib::YieldTermStructure> rTS = flatRate(today, riskFreeRate, dc);

#ifdef QL_HIGH_RESOLUTION_DATE
    QuantLib::Date exDate(today.dateTime() + length);
#else
    QuantLib::Date exDate = today + length;
#endif
    QuantLib::Settings::instance().evaluationDate() = today;

    QuantLib::ext::shared_ptr<QuantLib::Exercise> exercise = QuantLib::ext::make_shared<QuantLib::AmericanExercise>(today, exDate);
    QuantLib::ext::shared_ptr<QuantLib::StrikedTypePayoff> payoff = QuantLib::ext::make_shared<QuantLib::PlainVanillaPayoff>(optionType, strike);
    QuantLib::ext::shared_ptr<QuantLib::VanillaOption> option = makeOption(payoff, exercise, spot, qTS, rTS, volTS, JR);

    QuantLib::ext::shared_ptr<QuantLib::GeneralizedBlackScholesProcess> process = makeProcess(spot, qTS, rTS,volTS);

    return option->impliedVolatility(value, process, tolerance, maxEvaluations);
}
