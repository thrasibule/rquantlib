
// RQuantLib wrapper implmenentation
//
// Copyright 2014  Dirk Eddelbuettel <edd@debian.org>
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

#ifndef _rquantlib_impl_h_
#define _rquantlib_impl_h_

#include "rquantlib_internal.h"

// define template specialisations for as and wrap
namespace Rcpp {

    static const unsigned int QLtoJan1970Offset = 25569;  	// Offset to R / Unix epoch

    template <> QuantLib::Date as(SEXP dtsexp) {
        switch(TYPEOF(dtsexp)) {
        case INTSXP:
        {
            int dt = INTEGER(dtsexp)[0];
            return dt == NA_INTEGER ? QuantLib::Date() :
                QuantLib::Date(dt + QLtoJan1970Offset);
            break;
        }
        case REALSXP:
        {
            double dt = REAL(dtsexp)[0];
            return dt == NA_REAL ? QuantLib::Date() :
                QuantLib::Date(static_cast<int>(dt) + QLtoJan1970Offset);
            break;
        }
        default:
            stop("types needs to be Real or Integer");
        }
    }

    template <> SEXP wrap(const QuantLib::Date &d) {
        IntegerVector r = IntegerVector::create(static_cast<int>(d.serialNumber())
            - QLtoJan1970Offset);
        r.attr("class") = CharacterVector::create("Date");
        return wrap(r);
    }

    // non-intrusive extension via template specialisation
    template <> std::vector<QuantLib::Date> as(SEXP dtvecsexp) {
        int n = Rf_length(dtvecsexp);
        std::vector<QuantLib::Date> dates(n);
        switch(TYPEOF(dtvecsexp)) {
        case INTSXP:
        {
            int dt;
            for (int i = 0; i < n; i++) {
                dt = INTEGER(dtvecsexp)[i];
                dates[i] = dt == NA_INTEGER ? QuantLib::Date() :
                    QuantLib::Date(dt + QLtoJan1970Offset);
            }
            break;
        }
        case REALSXP:
        {
            double dt;
            for (int i = 0; i < n; i++) {
                dt = REAL(dtvecsexp)[i];
                dates[i] = dt == NA_REAL ? QuantLib::Date() :
                    QuantLib::Date(static_cast<int>(dt) + QLtoJan1970Offset);
            }
            break;
        }
        default:
            stop("type needs to be Real or Integer");
        }
        return dates;
    }

    // non-intrusive extension via template specialisation
    template <> SEXP wrap(const std::vector<QuantLib::Date> &dvec) {
        int n = dvec.size();
        NumericVector dtvec(n);
        for (int i = 0; i < n; i++) {
            dtvec[i] = static_cast<double>(dvec[i].serialNumber() - QLtoJan1970Offset);
        }
        dtvec.attr("class") = CharacterVector::create("Date");
        return wrap(dtvec);
    }

}

#endif
