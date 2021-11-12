# pdf_analyzer
simple command tool get descriptive statistics
including average, standard deviation, mode, 25 percentile, median, 75 percentile. 


## Install

- make directory like build,
- in the directory, execute cmake path/to/src
- then execute make


## Usage
- data is suppled from standard input. The first column must be coordinate and the second must be the pdf
- the pdf does not have to be normalized, i.e., you can use an unnormalized histogram as input. 
- results will be printed in the standard output.
- In particualr, for example, 
```
cat pdf_data.dat | path/to/pdf_analyzer 
```
will show statistics. 
