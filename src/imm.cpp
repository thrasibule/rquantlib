#include <rquantlib.h>


// [[Rcpp::export]]
std::string IMMcode(const QuantLib::Date& date) {
    return QuantLib::IMM::code(date);
}

// [[Rcpp::export]]
QuantLib::Date IMMdate(const std::string& code,
                       const Rcpp::Nullable<QuantLib::Date> referenceDate = R_NilValue) {
    if(referenceDate.isNotNull()) {
        return QuantLib::IMM::date(code, Rcpp::as<QuantLib::Date>(referenceDate));
    } else {
        return QuantLib::IMM::date(code);
    }
}

// [[Rcpp::export]]
QuantLib::Date nextIMMdate(const QuantLib::Date& date, bool mainCycle = true) {
    return QuantLib::IMM::nextDate(date, mainCycle);
}

// [[Rcpp::export]]
bool isIMMdate(const QuantLib::Date& date, bool mainCycle = true) {
    return QuantLib::IMM::isIMMdate(date, mainCycle);
}

// [[Rcpp::export]]
bool isIMMcode(const std::string& code, bool mainCycle = true) {
    return QuantLib::IMM::isIMMcode(code, mainCycle);
}

// [[Rcpp::export]]
std::string nextIMMcode(const QuantLib::Date& date, bool mainCycle = true) {
    return QuantLib::IMM::nextCode(date, mainCycle);
}
