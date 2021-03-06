/**
 * Copyright (c) 2016 DeepCortex GmbH <legal@eventql.io>
 * Authors:
 *   - Paul Asmuth <paul@eventql.io>
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License ("the license") as
 * published by the Free Software Foundation, either version 3 of the License,
 * or any later version.
 *
 * In accordance with Section 7(e) of the license, the licensing of the Program
 * under the license does not imply a trademark license. Therefore any rights,
 * title and interest in our trademarks remain entirely with us.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the license for more details.
 *
 * You can be released from the requirements of the license by purchasing a
 * commercial license. Buying such a license is mandatory as soon as you develop
 * commercial activities involving this program without disclosing the source
 * code of your own applications
 */
#pragma once
#include "eventql/util/io/outputstream.h"

enum class TerminalStyle : uint8_t {
  BRIGHT      = 1,
  DIM         = 2,
  UNDERSCORE  = 4,
  BLINK       = 5,
  REVERSE     = 7,
  HIDDEN      = 8,
  BLACK       = 30,
  RED         = 31,
  GREEN       = 32,
  YELLOW      = 33,
  BLUE        = 34,
  MAGENTA     = 35,
  CYAN        = 36,
  WHITE       = 37,
  BG_BLACK    = 40,
  BG_RED      = 41,
  BG_GREEN    = 42,
  BG_YELLOW   = 43,
  BG_BLUE     = 44,
  BG_MAGENTA  = 45,
  BG_CYAN     = 46,
  BG_WHITE    = 47
};


/**
 * A terminal output stream allows to conditionally use common ANSI terminal
 * escape codes, to e.g. color the output or control the cursor.
 *
 * A TerminalOutputStream detects if the underlying output stream is a TTY and
 * if not, doesn't omit any escape sequences.
 */
class TerminalOutputStream : public OutputStream {
public:

  /**
   * Create a new TerminalOutputStream instance from the provided output stream
   *
   * @param stream the stream
   */
  static ScopedPtr<TerminalOutputStream> fromStream(
      ScopedPtr<OutputStream> stream);

  /**
   * Create a new TerminalOutputStream instance from the provided output stream
   *
   * @param stream the stream
   */
  TerminalOutputStream(
      ScopedPtr<OutputStream> stream);

  /**
   * Write the provided string to the stream with the specified style(s).
   * This method may raise an exception.
   *
   * @param str a string to be written/printed
   * @param style one or more TerminalStyles
   */
  void print(const String& str, Vector<TerminalStyle> style = {});

  /**
   * Write the provided string to the stream with in red color
   *
   * @param str a string to be written/printed
   */
  void printRed(const String& str);

  /**
   * Write the provided string to the stream with in green color
   *
   * @param str a string to be written/printed
   */
  void printGreen(const String& str);

  /**
   * Write the provided string to the stream with in yellow color
   *
   * @param str a string to be written/printed
   */
  void printYellow(const String& str);

  /**
   * Write the provided string to the stream with in blue color
   *
   * @param str a string to be written/printed
   */
  void printBlue(const String& str);

  /**
   * Write the provided string to the stream with in magenta color
   *
   * @param str a string to be written/printed
   */
  void printMagenta(const String& str);

  /**
   * Write the provided string to the stream with in cyan color
   *
   * @param str a string to be written/printed
   */
  void printCyan(const String& str);

  /**
   * Erases from the current cursor position to the end of the current line.
   */
  void eraseEndOfLine();

  /**
   * Erases from the current cursor position to the start of the current line.
   */
  void eraseStartOfLine();

  /**
   * Erases the entire current line.
   */
  void eraseLine();

  /**
   * Erases the screen from the current line down to the bottom of the screen.
   */
  void eraseDown();

  /**
   * Erases the screen from the current line up to the top of the screen.
   */
  void eraseUp();

  /**
   * Erases the screen with the background colour and moves the cursor to home.
   */
  void eraseScreen();

  /**
   * Text wraps to next line if longer than the length of the display area.
   */
  void enableLineWrap();

  /**
   * Disables line wrapping.
   */
  void disableLineWrap();

  /**
   * Set the title of this terminal if supported
   */
  void setTitle(const String& title);

  /**
   * Write the next n bytes to the stream. This may raise an exception.
   * Returns the number of bytes that have been written.
   *
   * @param data a pointer to the data to be written
   * @param size then number of bytes to be written
   */
  size_t write(const char* data, size_t size) override;

  /**
   * Returns true if this file descriptor is connected to a TTY/terminal and
   * false otherwise
   */
  bool isTTY() const override;

protected:
  ScopedPtr<OutputStream> os_;
  bool is_tty_;
};
