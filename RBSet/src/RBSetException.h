/*
 * RBSetException.hpp
 *
 *  Created on: 16.05.2013
 *      Author: user
 */
#pragma once

#include <string>
#include <exception>

class RBSetException: public std::exception {
public:
  RBSetException(std::string message) : message(message) {

  }

  virtual ~RBSetException() throw() {

  }

private:
  const std::string message;

  virtual const char* what() const throw() {
    return message.c_str();
  }
};
