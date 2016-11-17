// nullmailer -- a simple relay-only MTA
// Copyright (C) 2016  Bruce Guenter <bruce@untroubled.org>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// You can contact me at <bruce@untroubled.org>.  There is also a mailing list
// available to discuss this package.  To subscribe, send an email to
// <nullmailer-subscribe@lists.untroubled.org>.

#include "config.h"
#include "defines.h"
#include <unistd.h>
#include "ac/time.h"
#include "itoa.h"
#include "mystring/mystring.h"

mystring make_date(time_t t)
{
  char buf[256];
  if (t == 0)
    t = time(0);
  struct tm* l = localtime(&t);
  strftime(buf, 256, "%a, %d %b %Y %H:%M:%S ", l);
#ifdef TM_HAS_GMTOFF
  long tznum = l->TM_HAS_GMTOFF/60;
#else
  long tznum = -timezone/60;
#ifdef TM_HAS_ISDST
  int daylight = l->TM_HAS_ISDST;
#endif // TM_HAS_ISDST
  if(daylight)
    tznum += 60;
#endif // TM_HAS_GMTOFF
  char tz[6];
  tz[0] = '+';
  if(tznum < 0) {
    tznum = -tznum;
    tz[0] = '-';
  }
  long tzhours = tznum / 60;
  tz[1] = (tzhours/10)%10 + '0';
  tz[2] = tzhours%10 + '0';
  long tzmins = tznum % 60;
  tz[3] = (tzmins/10)%10 + '0';
  tz[4] = tzmins%10 + '0';
  tz[5] = 0;
  return mystringjoin(buf) + tz;
}

// Message ID strings have the form SECONDS.USEC.PID.nullmailer@HOST
mystring make_messageid(const mystring& idhost)
{
  struct timeval tv;
  gettimeofday(&tv, 0);
  mystring tmp = "<";
  tmp += itoa(tv.tv_sec);
  tmp += '.';
  tmp += itoa(tv.tv_usec, 6);
  tmp += '.';
  tmp += itoa(getpid());
  tmp += ".nullmailer@";
  tmp += idhost;
  tmp += '>';
  return tmp;
}

mystring make_boundary()
{
  struct timeval tv;
  gettimeofday(&tv, 0);
  mystring tmp = itoa(tv.tv_sec);
  tmp += '.';
  tmp += itoa(tv.tv_usec, 6);
  tmp += '.';
  tmp += itoa(getpid());
  return tmp;
}
