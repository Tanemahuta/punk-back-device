#include "Logging.h"


void Logging::Init(int level, long baud) {
  _level = constrain(level, LOG_LEVEL_NOOUTPUT, LOG_LEVEL_VERBOSE);
  _baud = baud;
  Serial.begin(_baud);
}

void Logging::Error(const char* file, int line, String msg, ...) {
  if (LOG_LEVEL_ERRORS <= _level) {
    va_list args;
    va_start(args, msg);
    print(file, line, "ERROR: "+msg, args);
  }
}

void Logging::Info(const char* file, int line, String msg, ...) {
  if (LOG_LEVEL_INFOS <= _level) {
    va_list args;
    va_start(args, msg);
    print(file, line, "INFO: "+msg, args);
  }
}

void Logging::Debug(const char* file, int line, String msg, ...) {
  if (LOG_LEVEL_DEBUG <= _level) {
    va_list args;
    va_start(args, msg);
    print(file, line, "DEBUG: "+msg, args);
  }
}


void Logging::Verbose(const char* file, int line, String msg, ...) {
  if (LOG_LEVEL_VERBOSE <= _level) {
    va_list args;
    va_start(args, msg);
    print(file, line, "VERBOSE: "+msg, args);
  }
}

void Logging::print(const char* file, int line, String format, va_list args) {
  char formatArray[format.length()+1];
  format.toCharArray(formatArray, format.length()+1);
  print(file, line, formatArray, args);
}

void Logging::print(const char* file, int line, const char *format, va_list args) {
  Serial.print(file);
  Serial.print(":");
  Serial.print(line);
  Serial.print(" ");
  // loop through format string
  for (; *format != 0; ++format) {
    if (*format == '%') {
      ++format;
      if (*format == '\0') break;
      if (*format == '%') {
        Serial.print(*format);
        continue;
      }
      if ( *format == 's' ) {
        register char *s = (char *)va_arg( args, int );
        Serial.print(s);
        continue;
      }
      if ( *format == 'd' || *format == 'i') {
        Serial.print(va_arg( args, int ), DEC);
        continue;
      }
      if ( *format == 'x' ) {
        Serial.print(va_arg( args, int ), HEX);
        continue;
      }
      if ( *format == 'X' ) {
        Serial.print("0x");
        Serial.print(va_arg( args, int ), HEX);
        continue;
      }
      if ( *format == 'b' ) {
        Serial.print(va_arg( args, int ), BIN);
        continue;
      }
      if ( *format == 'B' ) {
        Serial.print("0b");
        Serial.print(va_arg( args, int ), BIN);
        continue;
      }
      if ( *format == 'l' ) {
        Serial.print(va_arg( args, long ), DEC);
        continue;
      }

      if ( *format == 'c' ) {
        Serial.print(va_arg( args, int ));
        continue;
      }
      if ( *format == 't' ) {
        if (va_arg( args, int ) == 1) {
          Serial.print("T");
        }
        else {
          Serial.print("F");
        }
        continue;
      }
      if ( *format == 'T' ) {
        if (va_arg( args, int ) == 1) {
          Serial.print("true");
        }
        else {
          Serial.print("false");
        }
        continue;
      }

    }
    Serial.print(*format);
  }
  Serial.println();
}

Logging Log = Logging();








