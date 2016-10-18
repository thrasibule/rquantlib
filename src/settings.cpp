#include "rquantlib_internal.h"

// [[Rcpp::export]]
bool setEvaluationDate(QuantLib::Date evalDate) {
    QuantLib::Settings::instance().evaluationDate() = evalDate;
    return true;
}

// [[Rcpp::export]]
QuantLib::Date EvaluationDate() {
    return QuantLib::Settings::instance().evaluationDate();
}
