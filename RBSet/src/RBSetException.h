/*
 * RBSetException.hpp
 *
 *  Created on: 16.05.2013
 *      Author: user
 */
#pragma once

#include <string>
#include <exception>

/**
 * Базовый класс объектов исключений для множества.
 */
class RBSetException: public std::exception {
public:
  /**
   * Конструктор
   *
   * @param message сообщение
   */
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

/**
 * Класс объектов исключений для итератора множества
 */
class RBSetIteratorException: public RBSetException {
public:
  /**
   * Конструктор
   *
   * @param message сообщение
   */
	RBSetIteratorException(std::string message) : RBSetException(message) {

	}
};

/**
 * Класс объектов исключений для сериализации.
 */
class RBSetParserException: public RBSetException {
public:
  /**
   * Конструктор
   *
   * @param message сообщение
   */
  RBSetParserException(std::string message) : RBSetException(message) {

  }
};

