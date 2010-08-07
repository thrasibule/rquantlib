// -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- 
//
// RQuantLib -- R interface to the QuantLib libraries
//
// Copyright (C) 2002 - 2010 Dirk Eddelbuettel <edd@debian.org>
//
// $Id$
//
// This file is part of the RQuantLib library for GNU R.
// It is made available under the terms of the GNU General Public
// License, version 2, or at your option, any later version,
// incorporated herein by reference.
//
// This program is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public
// License along with this program; if not, write to the Free
// Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA

#include <rquantlib.h>

RcppExport SEXP EuropeanOption(SEXP optionParameters) {

    try {

        Rcpp::List rparam(optionParameters);    	// Parameters via R list object

        std::string type     = Rcpp::as<std::string>(rparam["type"]);
        double underlying    = Rcpp::as<double>(rparam["underlying"]);
        double strike        = Rcpp::as<double>(rparam["strike"]);
        Spread dividendYield = Rcpp::as<double>(rparam["dividendYield"]);
        Rate riskFreeRate    = Rcpp::as<double>(rparam["riskFreeRate"]);
        Time maturity        = Rcpp::as<double>(rparam["maturity"]);
        int length           = int(maturity*360 + 0.5); // FIXME: this could be better
        double volatility    = Rcpp::as<double>(rparam["volatility"]);
    
        Option::Type optionType = getOptionType(type);

        Date today = Date::todaysDate();
        Settings::instance().evaluationDate() = today;

        // new framework as per QuantLib 0.3.5
        DayCounter dc = Actual360();
        boost::shared_ptr<SimpleQuote> spot(new SimpleQuote( underlying ));
        boost::shared_ptr<SimpleQuote> vol(new SimpleQuote( volatility ));
        boost::shared_ptr<BlackVolTermStructure> volTS = flatVol(today, vol, dc);
        boost::shared_ptr<SimpleQuote> qRate(new SimpleQuote( dividendYield ));
        boost::shared_ptr<YieldTermStructure> qTS = flatRate(today, qRate, dc);
        boost::shared_ptr<SimpleQuote> rRate(new SimpleQuote( riskFreeRate ));
        boost::shared_ptr<YieldTermStructure> rTS = flatRate(today, rRate, dc);

        Date exDate = today + length;
        boost::shared_ptr<Exercise> exercise(new EuropeanExercise(exDate));
	
        boost::shared_ptr<StrikedTypePayoff> payoff(new PlainVanillaPayoff(optionType, strike));
        boost::shared_ptr<VanillaOption> option = makeOption(payoff, exercise, spot, qTS, rTS, volTS);

        Rcpp::List rl = Rcpp::List::create(Rcpp::Named("value") = option->NPV(),
                                           Rcpp::Named("delta") = option->delta(),
                                           Rcpp::Named("gamma") = option->gamma(),
                                           Rcpp::Named("vega") = option->vega(),
                                           Rcpp::Named("theta") = option->theta(),
                                           Rcpp::Named("rho") = option->rho(),
                                           Rcpp::Named("divRho") = option->dividendRho(),
                                           Rcpp::Named("parameters") = optionParameters);
        return rl;

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}

RcppExport SEXP AmericanOption(SEXP optionParameters) {

    try {
        Rcpp::List rparam(optionParameters);

        std::string type = Rcpp::as<std::string>(rparam["type"]);
        double underlying = Rcpp::as<double>(rparam["underlying"]);
        double strike = Rcpp::as<double>(rparam["strike"]);
        Spread dividendYield = Rcpp::as<double>(rparam["dividendYield"]);
        Rate riskFreeRate = Rcpp::as<double>(rparam["riskFreeRate"]);
        Time maturity = Rcpp::as<double>(rparam["maturity"]);
        int length = int(maturity*360 + 0.5); // FIXME: this could be better
        double volatility = Rcpp::as<double>(rparam["volatility"]);
        
        Option::Type optionType = getOptionType(type);

        // new framework as per QuantLib 0.3.5, updated for 0.3.7
        // updated again for 0.9.0, see eg test-suite/americanoption.cpp
        Date today = Date::todaysDate();
        Settings::instance().evaluationDate() = today;
        DayCounter dc = Actual360();
        boost::shared_ptr<SimpleQuote> spot(new SimpleQuote(underlying));
        boost::shared_ptr<SimpleQuote> qRate(new SimpleQuote(dividendYield));
        boost::shared_ptr<YieldTermStructure> qTS = flatRate(today,qRate,dc);
        boost::shared_ptr<SimpleQuote> rRate(new SimpleQuote(riskFreeRate));
        boost::shared_ptr<YieldTermStructure> rTS = flatRate(today,rRate,dc);
        boost::shared_ptr<SimpleQuote> vol(new SimpleQuote(volatility));
        boost::shared_ptr<BlackVolTermStructure> volTS = flatVol(today, vol, dc);

        boost::shared_ptr<StrikedTypePayoff> payoff(new PlainVanillaPayoff(optionType, strike));

        Date exDate = today + length;
        boost::shared_ptr<Exercise> exercise(new AmericanExercise(today, exDate));

        boost::shared_ptr<BlackScholesMertonProcess> 
            stochProcess(new BlackScholesMertonProcess(Handle<Quote>(spot),
                                                       Handle<YieldTermStructure>(qTS),
                                                       Handle<YieldTermStructure>(rTS),
                                                       Handle<BlackVolTermStructure>(volTS)));
        // new from 0.3.7 BaroneAdesiWhaley
        boost::shared_ptr<PricingEngine> engine(new BaroneAdesiWhaleyApproximationEngine(stochProcess));

        VanillaOption option(payoff, exercise);
        option.setPricingEngine(engine);
                                                
        Rcpp::List rl = Rcpp::List::create(Rcpp::Named("value") = option.NPV(),
                                           Rcpp::Named("delta") = R_NaN,
                                           Rcpp::Named("gamma") = R_NaN,
                                           Rcpp::Named("vega") = R_NaN,
                                           Rcpp::Named("theta") = R_NaN,
                                           Rcpp::Named("rho") = R_NaN,
                                           Rcpp::Named("divRho") = R_NaN,
                                           Rcpp::Named("parameters") = optionParameters);
        return rl;

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}

RcppExport SEXP EuropeanOptionArrays(SEXP typesexp, SEXP parsexp) {

    try {
        Option::Type optionType = getOptionType( Rcpp::as<std::string>(typesexp) );
        Rcpp::NumericMatrix par(parsexp); // matrix of parameters as per expand.grid() in R
        int n = par.nrow();
        Rcpp::NumericVector value(n), delta(n), gamma(n), vega(n), theta(n), rho(n), divrho(n);

        Date today = Date::todaysDate();
        Settings::instance().evaluationDate() = today;

        DayCounter dc = Actual360();

        for (int i=0; i<n; i++) {

            // pars <- expand.grid(underlying, strike, dividendYield, riskFreeRate, maturity, volatility)

            double underlying    = par(i, 0);    // first column
            double strike        = par(i, 1);    // second column
            Spread dividendYield = par(i, 2);    // third column
            Rate riskFreeRate    = par(i, 3);    // fourth column
            Time maturity        = par(i, 4);    // fifth column
            int length           = int(maturity*360 + 0.5); // FIXME: this could be better
            double volatility    = par(i, 5);    // sixth column
    
            boost::shared_ptr<SimpleQuote> spot(new SimpleQuote( underlying ));
            boost::shared_ptr<SimpleQuote> vol(new SimpleQuote( volatility ));
            boost::shared_ptr<BlackVolTermStructure> volTS = flatVol(today, vol, dc);
            boost::shared_ptr<SimpleQuote> qRate(new SimpleQuote( dividendYield ));
            boost::shared_ptr<YieldTermStructure> qTS = flatRate(today, qRate, dc);
            boost::shared_ptr<SimpleQuote> rRate(new SimpleQuote( riskFreeRate ));
            boost::shared_ptr<YieldTermStructure> rTS = flatRate(today, rRate, dc);

            Date exDate = today + length;
            boost::shared_ptr<Exercise> exercise(new EuropeanExercise(exDate));
	
            boost::shared_ptr<StrikedTypePayoff> payoff(new PlainVanillaPayoff(optionType, strike));
            boost::shared_ptr<VanillaOption> option = makeOption(payoff, exercise, spot, qTS, rTS, volTS);

            value[i]  = option->NPV();
            delta[i]  = option->delta();
            gamma[i]  = option->gamma();
            vega[i]   = option->vega();
            theta[i]  = option->theta();
            rho[i]    = option->rho();
            divrho[i] = option->dividendRho();
        }
        return Rcpp::List::create(Rcpp::Named("value")  = value,
                                  Rcpp::Named("delta")  = delta,
                                  Rcpp::Named("gamma")  = gamma,
                                  Rcpp::Named("vega")   = vega,
                                  Rcpp::Named("theta")  = theta,
                                  Rcpp::Named("rho")    = rho,
                                  Rcpp::Named("divRho") = divrho);

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}
