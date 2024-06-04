# ETF Cost Basis Analyzer

## Overview
The ETF Cost Basis Analyzer is a robust tool designed to automate the calculation and reporting of adjusted cost bases for ETF investments, a critical component for accurate tax reporting. This functionality supports the services provided by robo-advisors to generate necessary tax documents efficiently.

## Files Description
- **main.cpp**: Entry point of the application, orchestrating the flow of data processing.
- **Transaction.hpp**: Defines the `Transaction` class and its associated functions, handling individual transaction records.
- **History.hpp**: Defines the `History` class, managing a collection of transactions and their historical data processing.
- **FileReader.hpp**: Header file with declarations for functions responsible for reading and parsing input data.
- **FileReader.cpp**: Implements functions declared in `FileReader.hpp`, ensuring accurate data input.
- **transaction_history.txt**: Sample input file containing a list of transaction data for processing.
- **History_Transaction_definitions.cpp**: Implementation of member functions for both `History` and `Transaction` classes, detailing operations on historical data.
