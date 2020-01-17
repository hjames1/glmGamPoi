// #include <Rcpp.h>
#include <RcppArmadillo.h>

using namespace Rcpp;

// [[Rcpp::depends(RcppArmadillo)]]

// [[Rcpp::export]]
IntegerVector makeCumSumLookupVector(IntegerVector y){
  double max_y = max(y);
  IntegerVector lookupTable(max_y+1);
  IntegerVector cumsumLookupTable(max_y);
  for(int i = 0; i < y.size(); i++){
    lookupTable[y[i]]++;
  }
  int lastelem = 0;
  for(int  i = lookupTable.size()-1; i >= 1; i--){
    cumsumLookupTable[i-1] = lookupTable[i] + lastelem;
    lastelem = cumsumLookupTable[i-1];
  }
  return cumsumLookupTable;
}

// [[Rcpp::export]]
double score_function_bandara_fast(IntegerVector y, IntegerVector cumsumLookupTable, NumericVector mu,
                                   double r, arma::mat model_matrix, bool do_cr_adj){
  double cr_term = 0.0;
  if(do_cr_adj){
    arma::vec w_diag = 1.0/(1.0/mu + 1/r);
    arma::vec dw_diag = pow(mu / (mu + r), 2);
    arma::mat b = model_matrix.t() * (model_matrix.each_col() % w_diag);
    arma::mat db = model_matrix.t() * (model_matrix.each_col() % dw_diag);
    cr_term = 0.5 * trace(b.i() * db);
  }

  double digammaSummand = 0.0;
  for(int v = 0; v < cumsumLookupTable.size(); v++){
    digammaSummand += cumsumLookupTable[v] / (r + v + 1 - 1);
  }

  double otherSummand = 0.0;
  for(int i = 0; i < y.size(); ++i){
    otherSummand += log(1 + mu[i] / r) + (y[i] - mu[i]) / (mu[i] + r);
  }
  return digammaSummand - otherSummand + cr_term;
}


// [[Rcpp::export]]
double score_deriv_function_bandara_fast(IntegerVector y, IntegerVector cumsumLookupTable,NumericVector mu,
                                         double r, arma::mat model_matrix, bool do_cr_adj){
  double cr_term = 0.0;
  if(do_cr_adj){
    arma::vec w_diag = 1/(1/mu + 1/r);
    arma::vec dw_diag = pow(mu / (mu + r), 2);
    arma::vec d2w_diag = -2 * pow(mu, 2) / pow(mu + r, 3);

    arma::mat b = model_matrix.t() * (model_matrix.each_col() % w_diag);
    arma::mat db = model_matrix.t() * (model_matrix.each_col() % dw_diag);
    arma::mat d2b = model_matrix.t() * (model_matrix.each_col() % d2w_diag);
    arma::mat b_i = b.i();
    double ddetb = ( det(b) * trace(b.i() * db) );
    double d2detb = ( det(b) * (pow(trace(b_i * db), 2) - trace(b_i * db * b_i * db) + trace(b_i * d2b)) );
    cr_term = 0.5 * pow(ddetb/det(b), 2) - 0.5 * d2detb / det(b);
  }

  double digammaSummand = 0.0;
  for(int v = 0; v < cumsumLookupTable.size(); v++){
    digammaSummand += cumsumLookupTable[v] / pow(r + v + 1 - 1, 2);
  }
  double otherSummand = 0.0;
  for(int i = 0; i < y.size(); ++i){
    otherSummand += mu[i] / (mu[i] * r + pow(r, 2)) + (y[i] - mu[i]) / pow(mu[i] + r, 2);
  }

  return -digammaSummand + otherSummand - cr_term;
}



