Settings <- R6Class("Settings",
                    active = list(
                        EvaluationDate = function(date) {
                            if (missing(date)) {
                                return (EvaluationDate())
                            } else {
                                setEvaluationDate(date)
                            }
                        })
                    )
