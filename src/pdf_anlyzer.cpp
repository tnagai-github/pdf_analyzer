/* 
 * Copyright (c) 2021, Tetsuro Nagai
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <cassert>
#include <string>
#include <boost/format.hpp>

constexpr int BUF_MAX=100000;

constexpr int DIM_DATA=2 ;

using std::string;
using std::vector;
using std::array;
using std::cout;
using std::cerr;
using std::endl;


int main(int argc, char** argv)
{

  if(argc != 1){
        cerr << "ERROR: no arguments allowed"  <<endl;
        cerr << "Data should be provided via standard input"  <<endl;
        cerr << "Use, for example, by cat data.txt | ./pdf_analyzer \n" <<endl;
        cerr << "First column must be coordinate, and the second must be probability."  <<endl;
        cerr << "The probability may not be normalized."  <<endl;
        return -1;
  }

  vector<double> pdf;
  vector<double> x;

  string buf;
  while (std::getline(std::cin, buf))
  {
    char tmp1[BUF_MAX];
    char tmp2[BUF_MAX];
    auto bsscanf = std::sscanf(buf.c_str(), "%s %s" , tmp1,tmp2);
    if(bsscanf != DIM_DATA){
        cerr << "ERROR: not enough columns\n" << "Exit!!" <<endl;
        return -1;
    }

    char *err;
    double x_tmp=std::strtod(tmp1, &err);
    if(*err!='\0'){
      cerr << "ERROR: error to convert: " << tmp1 << "\nExit!!" <<endl;
      return -1;
    }
    double pdf_tmp =std::strtod(tmp2, &err);
    if(*err!='\0'){
      cerr << "ERROR: error to convert: " << tmp2 << "\nExit!!" <<endl;
      return -1;
    }

    if(pdf_tmp < 0){
      cerr << "ERROR: pdf should be non-negative\nExit!!" <<endl;
      return -1;
    }
      
    if(!x.empty() and x.back() > x_tmp){
      cerr << "ERROR: x should be in ascending (increasing) order\nExit!!" <<endl;
      return -1;
    }
      
    x.push_back(x_tmp);
    pdf.push_back(pdf_tmp);
  }
  if(x.size() == 0){
      cerr << "ERROR: no data provided\nExit!!" <<endl;
      return -1;
  }


  auto sum_pdf = std::accumulate(pdf.begin(), pdf.end(), 0.0);
  std::transform(pdf.begin(), pdf.end(), pdf.begin(), [&sum_pdf](double x){return x / sum_pdf;});
  auto ave_x = std::inner_product(pdf.begin(), pdf.end(), x.begin(), 0.0);

  vector<double> residual_sq(pdf.size(), 0);
  std::transform(x.begin(), x.end(), residual_sq.begin(), [&ave_x](double x){return (x-ave_x)*(x-ave_x);});

  double  variance = std::inner_product(residual_sq.begin(), residual_sq.end(), pdf.begin(), 0.0) ;
  double  stddev = sqrt(variance);

  vector<double> residual_3th(pdf.size(), 0);
  std::transform(x.begin(), x.end(), residual_3th.begin(), [&ave_x](double x){return (x-ave_x)*(x-ave_x)*(x-ave_x);});

  auto skew = std::inner_product(residual_3th.begin(), residual_3th.end(), pdf.begin(), 0.0) / pow(stddev, 3) ;

  vector<double> residual_4th(pdf.size(), 0);
  std::transform(x.begin(), x.end(), residual_4th.begin(), [&ave_x](double x){return (x-ave_x)*(x-ave_x)*(x-ave_x)*(x-ave_x);});

  auto kurtosis = std::inner_product(residual_4th.begin(), residual_4th.end(), pdf.begin(), 0.0) / pow(variance, 2) -3.0 ;

  auto  at_max = std::max_element(pdf.begin(), pdf.end());
  auto  mode = x[std::distance(pdf.begin(), at_max)];

  //auto  median  = x[std::distance(pdf.begin(), at_max)];
  vector<double> cdf(pdf.size(), 0);
  std::partial_sum(pdf.begin(), pdf.end(), cdf.begin());

  auto at_Q1 = find_if(cdf.begin(), cdf.end(), [](double x){return x>0.25? true : false;});
  auto at_Q2 = find_if(cdf.begin(), cdf.end(), [](double x){return x>0.50? true : false;});
  auto at_Q3 = find_if(cdf.begin(), cdf.end(), [](double x){return x>0.75? true : false;});

  auto Q1 = x[std::distance(cdf.begin(), at_Q1)];
  auto Q2 = x[std::distance(cdf.begin(), at_Q2)];
  auto Q3 = x[std::distance(cdf.begin(), at_Q3)];


  std::cout  << boost::format("#%12s %12s %12s %12s %12s") % "ave_x"%"stddev" % "variance"  % "skew" % "kurtosis" << std::endl;
  std::cout  << boost::format(" %12.8g %12.8g %12.8g %12.8g %12.8g\n") % ave_x % stddev % variance % skew % kurtosis;

  std::cout  << boost::format("#%12s %12s %12s") % "1Q"%"2Q(median)" % "3Q"  << std::endl;
  std::cout  << boost::format(" %12.8g %12.8g %12.8g \n") % Q1  % Q2 % Q3 ;

  std::cout  << boost::format("#%12s %12s %12s") % "mode "%"normalization" % ""  << std::endl;
  std::cout  << boost::format(" %12.8g %12.8g\n") % mode  % sum_pdf ;

  return EXIT_SUCCESS ;
}
