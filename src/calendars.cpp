
//  RQuantLib -- R interface to the QuantLib libraries
//
//  Copyright (C) 2002 - 2019  Dirk Eddelbuettel
//  Copyright (C) 2010         Dirk Eddelbuettel and Khanh Nguyen
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

// returns a reference-counted pointer to a matching Calendar object
// [[Rcpp::export]]
Rcpp::XPtr<QuantLib::Calendar> getCalendar(const std::string &calstr) {
    QuantLib::Calendar *pcal;

    if (calstr == "TARGET") { 		// generic calendar
        pcal = new QuantLib::TARGET();

    } else if (calstr == "Argentina") {
        pcal = new QuantLib::Argentina();

    } else if (calstr == "Australia") {
        pcal = new QuantLib::Australia();

    } else if (calstr == "Brazil") {
        pcal = new QuantLib::Brazil();

    } else if (calstr == "Canada" || calstr == "Canada/Settlement") {
        pcal = new QuantLib::Canada(QuantLib::Canada::Settlement);
    } else if (calstr == "Canada/TSX") {
        pcal = new QuantLib::Canada(QuantLib::Canada::TSX);

    } else if (calstr == "China") {
        pcal = new QuantLib::China();

    } else if (calstr == "CzechRepublic") {
        pcal = new QuantLib::CzechRepublic();

    } else if (calstr == "Denmark") {
        pcal = new QuantLib::Denmark();

    } else if (calstr == "Finland") {
        pcal = new QuantLib::Finland();

    } else if (calstr == "Germany" || calstr == "Germany/FrankfurtStockExchange") {
        pcal = new QuantLib::Germany(QuantLib::Germany::FrankfurtStockExchange);
    } else if (calstr == "Germany/Settlement") {
        pcal = new QuantLib::Germany(QuantLib::Germany::Settlement);
    } else if (calstr == "Germany/Xetra") {
        pcal = new QuantLib::Germany(QuantLib::Germany::Xetra);
    } else if (calstr == "Germany/Eurex") {
        pcal = new QuantLib::Germany(QuantLib::Germany::Eurex);

    } else if (calstr == "HongKong") {
        pcal = new QuantLib::HongKong();

    } else if (calstr == "Hungary") {
        pcal = new QuantLib::Hungary();

    } else if (calstr == "Iceland") {
        pcal = new QuantLib::Iceland();

    } else if (calstr == "India") {
        pcal = new QuantLib::India();

    } else if (calstr == "Indonesia") {
        pcal = new QuantLib::Indonesia();

    } else if (calstr == "Italy" || calstr == "Italy/Settlement") {
        pcal = new QuantLib::Italy(QuantLib::Italy::Settlement);
    } else if (calstr == "Italy/Exchange") {
        pcal = new QuantLib::Italy(QuantLib::Italy::Exchange);

    } else if (calstr == "Japan" || calstr == "Japan/Settlement") {
        pcal = new QuantLib::Japan();

    } else if (calstr == "Mexico") {
        pcal = new QuantLib::Mexico();

    } else if (calstr == "NewZealand") {
        pcal = new QuantLib::NewZealand();

    } else if (calstr == "Norway") {
        pcal = new QuantLib::Norway();

    } else if (calstr == "Poland") {
        pcal = new QuantLib::Poland();

    } else if (calstr == "Russia") {
        pcal = new QuantLib::Russia();

    } else if (calstr == "SaudiArabia") {
        pcal = new QuantLib::SaudiArabia();

    } else if (calstr == "Singapore") {
        pcal = new QuantLib::Singapore();

    } else if (calstr == "Slovakia") {
        pcal = new QuantLib::Slovakia();

    } else if (calstr == "SouthAfrica") {
        pcal = new QuantLib::SouthAfrica();

    } else if (calstr == "SouthKorea" || calstr == "SouthKorea/Settlement") {
        pcal = new QuantLib::SouthKorea(QuantLib::SouthKorea::Settlement);
    } else if (calstr == "SouthKorea/KRX") {
        pcal = new QuantLib::SouthKorea(QuantLib::SouthKorea::KRX);

    } else if (calstr == "Sweden") {
        pcal = new QuantLib::Sweden();

    } else if (calstr == "Switzerland") {
        pcal = new QuantLib::Switzerland();

    } else if (calstr == "Taiwan") {
        pcal = new QuantLib::Taiwan();

    } else if (calstr == "Turkey") {
          pcal = new QuantLib::Turkey();

    } else if (calstr == "Ukraine") {
        pcal = new QuantLib::Ukraine();

    } else if (calstr == "UnitedKingdom" || calstr == "UnitedKingdom/Settlement") {
        pcal = new QuantLib::UnitedKingdom(QuantLib::UnitedKingdom::Settlement);
    } else if (calstr == "UnitedKingdom/Exchange") {
        pcal = new QuantLib::UnitedKingdom(QuantLib::UnitedKingdom::Exchange);
    } else if (calstr == "UnitedKingdom/Metals") {
        pcal = new QuantLib::UnitedKingdom(QuantLib::UnitedKingdom::Metals);

    } else if (calstr == "UnitedStates" || calstr == "UnitedStates/Settlement") {
        pcal = new QuantLib::UnitedStates(QuantLib::UnitedStates::Settlement);
    } else if (calstr == "UnitedStates/NYSE") {
        pcal = new QuantLib::UnitedStates(QuantLib::UnitedStates::NYSE);
    } else if (calstr == "UnitedStates/GovernmentBond") {
        pcal = new QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
    } else if (calstr == "UnitedStates/NERC") {
        pcal = new QuantLib::UnitedStates(QuantLib::UnitedStates::NERC);

    } else if (calstr == "WeekendsOnly") {
        pcal = new QuantLib::WeekendsOnly();

    } else if (calstr == "Null" || calstr == "null" || calstr == "NULL") {
        pcal.reset(new QuantLib::NullCalendar());

    } else {
        throw std::invalid_argument("Calendar " + calstr + " not recognised ");
    }

    return Rcpp::XPtr<QuantLib::Calendar>(pcal, true);
}

// [[Rcpp::export]]
const std::string getCalendarName(const Rcpp::XPtr<QuantLib::Calendar>& cal) {
    return cal->name();
}

// [[Rcpp::export]]
bool setCalendarContext(std::string calendar, int fixingDays, QuantLib::Date settleDate) {

    // Rcpp Attribute cannot reflect complicated default arguments
    if (settleDate.serialNumber() == 0) {
        calendar = "TARGET";
        fixingDays = 2;
        settleDate = QuantLib::Date::todaysDate() + 2;
    }
    // set fixingDays and settleDate
    RQLContext::instance().fixingDays = fixingDays;
    RQLContext::instance().settleDate = settleDate;
    RQLContext::instance().calendar = getCalendar(calendar); // set calendar in global singleton

    return true;
}

// [[Rcpp::export]]
std::vector<bool> isBusinessDay(const Rcpp::XPtr<QuantLib::Calendar>& calendar,
                                const std::vector<QuantLib::Date>& dates) {
    std::vector<bool> bizdays;
    for (std::vector<QuantLib::Date>::const_iterator it = dates.begin();
         it != dates.end(); ++it) {
        bizdays.push_back(calendar->isBusinessDay(*it));
    }
    return bizdays;
}

// [[Rcpp::export]]
    std::vector<bool> isHoliday(const Rcpp::XPtr<QuantLib::Calendar>& calendar,
                                const std::vector<QuantLib::Date>& dates) {
    std::vector<bool> hdays;
    for (std::vector<QuantLib::Date>::const_iterator it = dates.begin();
         it != dates.end(); ++it) {
        hdays.push_back(calendar->isHoliday(*it));
    }
    return hdays;
}

// [[Rcpp::export]]
std::vector<bool> isWeekend(const Rcpp::XPtr<QuantLib::Calendar>& calendar,
                            const std::vector<QuantLib::Date>& dates) {

    std::vector<bool> weekends;
    for (std::vector<QuantLib::Date>::const_iterator it = dates.begin();
         it != dates.end(); ++it) {
        weekends.push_back(calendar->isWeekend(it->weekday()));
    }
    return weekends;
}

// [[Rcpp::export]]
std::vector<bool> isEndOfMonth(const Rcpp::XPtr<QuantLib::Calendar>& calendar,
                               const std::vector<QuantLib::Date>& dates) {

    std::vector<bool> eom;
    for (std::vector<QuantLib::Date>::const_iterator it = dates.begin();
         it != dates.end(); ++it) {
        eom.push_back(calendar->isEndOfMonth(*it));
    }
    return eom;
}

// [[Rcpp::export]]
std::vector<QuantLib::Date> endOfMonth(const Rcpp::XPtr<QuantLib::Calendar>& calendar,
                                       const std::vector<QuantLib::Date>& dates) {
    std::vector<QuantLib::Date> ndates;
    for (std::vector<QuantLib::Date>::const_iterator it = dates.begin();
         it != dates.end(); ++it) {
        ndates.push_back(calendar->endOfMonth(*it));
    }
    return ndates;
}

// [[Rcpp::export]]
std::vector<QuantLib::Date> adjust(const Rcpp::XPtr<QuantLib::Calendar>& calendar,
                                   const std::vector<QuantLib::Date>& dates,
                                   int bdc = 0) {
    QuantLib::BusinessDayConvention bdcval = getBusinessDayConvention(bdc);
    std::vector<QuantLib::Date> adjusted;
    for (std::vector<QuantLib::Date>::const_iterator it = dates.begin();
         it != dates.end(); ++it) {
        adjusted.push_back(calendar->adjust(*it, bdcval));
    }
    return adjusted;
}

// [[Rcpp::export]]
std::vector<QuantLib::Date> advance1(const Rcpp::XPtr<QuantLib::Calendar>& calendar,
                                     int amount, int unit, int bdcVal,
                                     bool emr,
                                     const std::vector<QuantLib::Date>& dates) {

    QuantLib::BusinessDayConvention bdc = getBusinessDayConvention(bdcVal);
    std::vector<QuantLib::Date> advance;
    for (std::vector<QuantLib::Date>::const_iterator it = dates.begin();
         it != dates.end(); ++it) {
        advance.push_back(calendar->advance(*it, amount, getTimeUnit(unit), bdc, emr));
    }
    return advance;
}

// [[Rcpp::export]]
std::vector<QuantLib::Date> advance2(const Rcpp::XPtr<QuantLib::Calendar>& calendar,
                                     int period, int bdcVal, bool emr,
                                     const std::vector<QuantLib::Date>& dates) {

    QuantLib::BusinessDayConvention bdc = getBusinessDayConvention(bdcVal);
    std::vector<QuantLib::Date> advance;
    for (std::vector<QuantLib::Date>::const_iterator it = dates.begin();
         it != dates.end(); ++it) {
        advance.push_back(calendar->advance(*it, QuantLib::Period(getFrequency(period)),
                                            bdc, emr));
    }
    return advance;
}

// [[Rcpp::export]]
std::vector<int> businessDaysBetween(const Rcpp::XPtr<QuantLib::Calendar>& calendar,
                                     const std::vector<QuantLib::Date>& from,
                                     const std::vector<QuantLib::Date>& to,
                                     bool includeFirst,
                                     bool includeLast) {
    int n = from.size();
    std::vector<int> between(n);
    for (int i = 0; i < n; i++) {
        between[i] = calendar->businessDaysBetween(from[i], to[i],
                                                   includeFirst, includeLast);
    }
    return between;
}

// [[Rcpp::export]]
std::vector<QuantLib::Date> getHolidayList(const Rcpp::XPtr<QuantLib::Calendar>& calendar,
                                           QuantLib::Date from, QuantLib::Date to,
                                           bool includeWeekends=false) {

    return QuantLib::Calendar::holidayList(*calendar, from, to, includeWeekends);
}

// [[Rcpp::export]]
void addHolidays(std::string calendar, std::vector<QuantLib::Date> dates) {
    Rcpp::XPtr<QuantLib::Calendar> pcal = getCalendar(calendar);
    int n = dates.size();
    for (int i=0; i<n; i++) {
        pcal->addHoliday(dates[i]);
    }
}

// [[Rcpp::export]]
void removeHolidays(std::string calendar, std::vector<QuantLib::Date> dates) {
    Rcpp::XPtr<QuantLib::Calendar> pcal = getCalendar(calendar);
    int n = dates.size();
    for (int i=0; i<n; i++) {
        pcal->removeHoliday(dates[i]);
    }
}

// [[Rcpp::export]]
QuantLib::Date nextIMMDate(QuantLib::Date day) {
    return QuantLib::IMM::nextDate(day);
}
