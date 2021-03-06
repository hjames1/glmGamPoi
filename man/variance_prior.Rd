% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/quasi_gamma_poisson_shrinkage.R
\name{variance_prior}
\alias{variance_prior}
\title{Estimate the scale and df for a Inverse Chisquare distribution that generate the true gene variances}
\usage{
variance_prior(s2, df, covariate = NULL, abundance_trend = NULL)
}
\arguments{
\item{s2}{vector of observed variances. Must not contain \code{0}'s.}

\item{df}{vector or single number with the degrees of freedom}

\item{covariate}{a vector with the same length as s2. \code{covariate} is used to regress
out the trend in \code{s2}. If \code{covariate = NULL}, it is ignored.}

\item{abundance_trend}{logical that decides if the additional abundance trend is fit
to the data. If \code{NULL} the abundance trend is fitted if there are more than 10 observations
and the \code{covariate} is not \code{NULL}. Default: \code{NULL}}
}
\value{
a list with three elements:
\describe{
\item{variance0}{estimate of the scale of the inverse Chisquared distribution. If
covariate is \code{NULL} a single number, otherwise a vector of \code{length(covariate)}}
\item{df0}{estimate of the degrees of freedom of the inverse Chisquared distribution}
\item{var_pos}{the shrunken variance estimates: a combination of \code{s2} and \code{variance0}}
}
}
\description{
This function implements Smyth's 2004 variance shrinkage. It also supports covariates that are
fitted to log(s2) with natural splines. This is based on the 2012 Lund et al. quasi-likelihood
paper.
}
\seealso{
\code{limma::squeezeVar()}
}
\keyword{internal}
