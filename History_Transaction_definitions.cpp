#include <cassert>
#include <iomanip>
#include <iostream>

#include "FileReader.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
//
Transaction::Transaction( std::string ticker_symbol,  unsigned int day_date,  
        unsigned int month_date,  unsigned year_date, 
        bool buy_sell_trans,  unsigned int number_shares,  
        double trans_amount ){

  this->symbol = ticker_symbol;
  this->day = day_date;
  this->month = month_date;
  this->year = year_date;
  this->shares = number_shares;
  this->amount = trans_amount;
  this->trans_id = assigned_trans_id;
  this->acb = 0.0;
  this->acb_per_share = 0.0;
  this->share_balance = 0;
  this->cgl = 0.0;
  this->p_next = nullptr;
  if (buy_sell_trans){
    this->trans_type = "Buy";
  } else {
    this->trans_type = "Sell";
  }
  ++assigned_trans_id;
}
    
// Destructor
Transaction::~Transaction(){
}

// Overloaded < operator.
//

bool Transaction::operator<(Transaction const &other) {
  if (year > other.year) {
    return false;
  } else if (year < other.year){
    return true;
  }

  if (month > other.month) {
    return false;
  } else if (month < other.month){
    return true;
  }

  if (day > other.day) {
    return false;
  } else if (day < other.day){
    return true;
  }

  if (trans_id > other.trans_id) {
    return true;
  } else {
    return false;
  }
}

// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
//
History::History(){
  p_head = nullptr;
}

// Destructor
//
History::~History(){

  while (p_head != nullptr) {
    Transaction *p_temp{p_head};
    p_head = p_head -> get_next();
    delete p_temp;
    p_temp = nullptr;
  }
}

// read_history(...): Read the transaction history from file.
//
void History::read_history(){
  FinanceTracker::open_file();

  while (FinanceTracker::next_trans_entry()){
    std::string symbol = FinanceTracker::get_trans_symbol();
    unsigned int day = FinanceTracker::get_trans_day();
    unsigned int month = FinanceTracker::get_trans_month();
    unsigned int year = FinanceTracker::get_trans_year();
    unsigned int shares = FinanceTracker::get_trans_shares();
    double amount = FinanceTracker::get_trans_amount();
    bool buysell = FinanceTracker::get_trans_type();

    Transaction *new_trans = new Transaction(symbol, day, month, year, buysell, shares, amount);
    insert(new_trans);
  }
  
  FinanceTracker::close_file();
}

// insert(...): Insert transaction into linked list.
void History::insert (Transaction *p_new_trans){
  if (p_head == nullptr){
    p_head = p_new_trans;
    return;
  }

  Transaction *p_temp = p_head;
  while(p_temp->get_next() != nullptr){
    p_temp = p_temp->get_next();
  }
  
  p_temp->set_next(p_new_trans);
}

// sort_by_date(): Sort the linked list by trade date.
//
void History::sort_by_date() {
  if (p_head == nullptr || p_head->get_next() == nullptr) {
    return;
  }

  Transaction *sortedList = nullptr;

  while (p_head != nullptr)
  {
    // pi is the first element in the original list, remove it from the orignal list and 
    // will insert into the sorted list
    Transaction *pi = p_head;

    // Move the head of the original list to the next
    p_head = p_head->get_next();

    // Break pi from the original list
    pi->set_next(nullptr);

    // If sorted list is empty, assign pi to the sorted list
    if (sortedList == nullptr) {
      sortedList = pi;
    }
    else {
      Transaction *pj = sortedList;
      Transaction *prev = nullptr;
      
      //
      while (pj != nullptr) {
        if (*pi < *pj) {
          if (prev == nullptr) {
            // Insert pi to the head of the sorted list
            pi->set_next(sortedList);
            sortedList = pi;
          }
          else {
            // Insert pi to the right position of the sorted list
            prev->set_next(pi);
            pi->set_next(pj);
          }

          break;
        }
        else {
          // Reachs the end of the sorted list and insert pi to the trail of the sorted list
          if (pj->get_next() == nullptr) {
            pj->set_next(pi);
            break;
          }
          else {
            // Move to the next of the sorted list
            prev = pj;
            pj = pj->get_next();
          }
        }
      }
    }
  }

  // Assign the sorted list back to p_head
  p_head = sortedList;
}

// update_acb_cgl(): Updates the ACB and CGL values.
//
void History::update_acb_cgl() {

  Transaction *curr = p_head;
  double acb{ 0.0 };
  int share_balance{ 0 };
  double acb_per_share{ 0.0 };
  double cgl{ 0.0 };

  while (curr != nullptr) {
    // buy transaction
    if (curr->get_trans_type() == true) {
      acb += curr->get_amount();
      share_balance += curr->get_shares();
    }
    else {
      acb -= acb_per_share * curr->get_shares();
      share_balance -= curr->get_shares();
      cgl = curr->get_amount() - acb_per_share * curr->get_shares();
      curr->set_cgl(cgl);
    }

    acb_per_share = acb / share_balance;

    curr->set_acb(acb);
    curr->set_share_balance(share_balance);
    curr->set_acb_per_share(acb_per_share);

    curr = curr->get_next();
  }
}

// compute_cgl(): )Compute the ACB, and CGL.
double History::compute_cgl(unsigned int year) {
  double total_cgl{ 0.0 };
  Transaction *curr = p_head;
  bool found_year{ false };
  while (curr != nullptr) {

    if (curr->get_year() == year) {
      if (!found_year) {
        found_year = true;
      }
      total_cgl += curr->get_cgl();
    }
    else if ((curr->get_year() != year) && found_year) {
      break;
    }

    curr = curr->get_next();
  }

  return total_cgl;
}

// print() Print the transaction history.
//
void History::print(){
  std::cout << "========== BEGIN TRANSACTION HISTORY ============" << std::endl;

  Transaction *p_temp = p_head;
  while(p_temp != nullptr){
    p_temp->print();
    p_temp = p_temp->get_next();
  }

  std::cout << "========== END TRANSACTION HISTORY ============" << std::endl;
}

// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }
