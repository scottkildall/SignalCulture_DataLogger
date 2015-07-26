Signal Culture Data Logger
written by Scott Kildall

Red LED will be on while we are not logging
  A momentary switch will begin logging
  Pressing the momentary switch will stop logging
  
  Ideally, we would log to RAM and then flush when the buffer is filled.
  
  We are logging to microseconds, 1000 micros = 1 millis
  
  Other options would be to overclock, which might give us 2X the data-logging rate
  However, this is complicated.
  
  Also, simply running this with a fan on it, too cool it down might make the CPU faster
  
  max # of characters
  - signal value 0-1023 = 4 charcters
  - command (,) = 1 character
  - max unsigned long = 18446744073709551615 = 20
  - newline = 1 chacter
  - termination string = 1 char
  - total = 27 characters, add 1 for safekeeping to make 28
  
  sample time on a is about 280 microseconds
  a faster SD card will have marginal improvements since we are using a RAM buffer and flushing it every 20 lines or so
  
  Sample rate ~3500-3600 H
  i.e. 3500-3600 samples/second
  Pretty good!!
  
  ~ 1.7mb file for 60 seconds
  60 minutes = 102mb
  
  The next issue is that we run out of micros(), so have an overflow problem after about a minute
  We could fix this in code (best solution)
  
  Could also log for a specified amount of time, using a millis() timer