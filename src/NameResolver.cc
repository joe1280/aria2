/* <!-- copyright */
/*
 * aria2 - The high speed download utility
 *
 * Copyright (C) 2006 Tatsuhiro Tsujikawa
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 */
/* copyright --> */
#include "NameResolver.h"
#include "DlAbortEx.h"
#include "message.h"
#include "StringFormat.h"
#include "Util.h"
#include <cstring>

namespace aria2 {

NameResolver::NameResolver():_socktype(0) {}

std::deque<std::string> NameResolver::resolve(const std::string& hostname)
{
  struct addrinfo hints;
  struct addrinfo* res;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = _socktype;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;
  int s;
  s = getaddrinfo(hostname.c_str(), "0", &hints, &res);
  if(s) {
    throw DlAbortEx(StringFormat(EX_RESOLVE_HOSTNAME,
				 hostname.c_str(), gai_strerror(s)).str());
  }
  std::deque<std::string> addrs;
  struct addrinfo* rp;
  for(rp = res; rp; rp = rp->ai_next) {
    std::pair<std::string, uint16_t> addressPort
      = Util::getNumericNameInfo(rp->ai_addr, rp->ai_addrlen);
    addrs.push_back(addressPort.first);
  }
  freeaddrinfo(res);

  return addrs;
}

void NameResolver::setSocktype(int socktype)
{
  _socktype = socktype;
}

} // namespace aria2
