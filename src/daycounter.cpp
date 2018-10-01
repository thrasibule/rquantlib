
//  RQuantLib -- R interface to the QuantLib libraries
//
//  Copyright (C) 2009 - 2011  Dirk Eddelbuettel and Khanh Nguyen
//  Copyright (C) 2012 - 2018  Dirk Eddelbuettel
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
Rcpp::XPtr<QuantLib::DayCounter> getDaycounter(const int n) {
    QuantLib::DayCounter* dc;
    if (n==0)
        dc = new QuantLib::Actual360();
    else if (n==1)
        dc = new QuantLib::Actual365Fixed();
    else if (n==2)
        dc = new QuantLib::ActualActual();
    else if (n==3)
        dc = new QuantLib::Business252();
    else if (n==4)
        dc = new QuantLib::OneDayCounter();
    else if (n==5)
        dc = new QuantLib::SimpleDayCounter();
    else if (n==6)
        dc = new QuantLib::Thirty360();
    else if (n==7)
        dc = new QuantLib::Actual365Fixed(QuantLib::Actual365Fixed::NoLeap);
    else if (n==8)
        dc = new QuantLib::ActualActual(QuantLib::ActualActual::ISMA);
    else if (n==9)
        dc = new QuantLib::ActualActual(QuantLib::ActualActual::Bond);
    else if (n==10)
        dc = new QuantLib::ActualActual(QuantLib::ActualActual::ISDA);
    else if (n==11)
        dc = new QuantLib::ActualActual(QuantLib::ActualActual::Historical);
    else if (n==12)
        dc = new QuantLib::ActualActual(QuantLib::ActualActual::AFB);
    else // if (n==13)
        dc = new QuantLib::ActualActual(QuantLib::ActualActual::Euro);
    return Rcpp::XPtr<QuantLib::DayCounter>(dc, true);
}

// [[Rcpp::export]]
std::string getDayCounterName(const Rcpp::XPtr<QuantLib::DayCounter>& dc) {
    return dc->name();
}

// [[Rcpp::export]]
std::vector<int> dayCount(const Rcpp::XPtr<QuantLib::DayCounter>& dc,
                          const std::vector<QuantLib::Date>& startDates,
                          const std::vector<QuantLib::Date>& endDates) {

    int n = startDates.size();
    std::vector<int> result(n);
    for (int i=0; i< n; i++){
        result[i] = dc->dayCount(startDates[i], endDates[i]);
    }
    return result;
}


// [[Rcpp::export]]
std::vector<double> yearFraction(const Rcpp::XPtr<QuantLib::DayCounter>& dc,
                                 const std::vector<QuantLib::Date>& startDates,
                                 const std::vector<QuantLib::Date>& endDates) {

    int n = startDates.size();
    std::vector<double> result(n);
    for (int i=0; i < n; i++){
        result[i] = dc->yearFraction(startDates[i], endDates[i]);
    }
    return result;
}
