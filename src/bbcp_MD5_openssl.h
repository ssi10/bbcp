#ifndef __BBCP_MD5_openssl_H__
#define __BBCP_MD5_openssl_H__
/******************************************************************************/
/*                                                                            */
/*                            b b c p _ M D 5 . h                             */
/*                                                                            */
/*(c) 2010-17 by the Board of Trustees of the Leland Stanford, Jr., University*/
/*      All Rights Reserved. See bbcp_Version.C for complete License Terms    */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC02-76-SFO0515 with the Department of Energy              */
/*                                                                            */
/* bbcp is free software: you can redistribute it and/or modify it under      */
/* the terms of the GNU Lesser General Public License as published by the     */
/* Free Software Foundation, either version 3 of the License, or (at your     */
/* option) any later version.                                                 */
/*                                                                            */
/* bbcp is distributed in the hope that it will be useful, but WITHOUT        */
/* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      */
/* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public       */
/* License for more details.                                                  */
/*                                                                            */
/* You should have received a copy of the GNU Lesser General Public License   */
/* along with bbcp in a file called COPYING.LESSER (LGPL license) and file    */
/* COPYING (GPL license).  If not, see <http://www.gnu.org/licenses/>.        */
/*                                                                            */
/* The copyright holder's institutional names and contributor's names may not */
/* be used to endorse or promote products derived from this software without  */
/* specific prior written permission of the institution or contributor.       */
/******************************************************************************/

#include <string.h>

typedef unsigned int uint32;

// OpenSSL 3.0 이상에서 MD5 함수 사용을 위한 매크로
#define OPENSSL_API_COMPAT 0x10100000L
#define OPENSSL_SUPPRESS_DEPRECATED

/******************************************************************************/
/*                  M D 5 C o n t e x t   S t r u c t u r e                   */
/******************************************************************************/
  

/******************************************************************************/
/*                             M D 5   C l a s s                              */
/******************************************************************************/

#include <stdio.h>

#include "bbcp_Headers.h"
#include "bbcp_ChkSum.h"
#ifdef MACOS
#include "CommonCrypto/CommonDigest.h"
#undef MD5_CTX
#define MD5_CTX CC_MD5_CTX
#else
#include "openssl/md5.h"
#endif
  
class bbcp_MD5_openssl : public bbcp_ChkSum
{
public:

int   Chk_MD5(char *buff, int  blen, char MD5Val[16])
             {MD5Init(&MyContext);
              MD5Update(&MyContext, (unsigned char *)buff, (unsigned)blen);
              MD5Final(MyDigest, &MyContext);
              return memcmp((const void *)MD5Val, (const void *)MyDigest,
                            (size_t)sizeof(MyDigest));
             }

void  Set_MD5(char *buff, int  blen, char MD5Val[16])
             {MD5Init(&MyContext);
              MD5Update(&MyContext, (unsigned char *)buff, (unsigned)blen);
              MD5Final((unsigned char *)MD5Val, &MyContext);
             }

char *csCurr(char **Text=0)
             {MD5_CTX currCTX = MyContext;
              MD5Final(MyDigest, &currCTX);
              if (Text) *Text = x2a((char *)MyDigest);
              return (char *)MyDigest;
             }

int   csSize() {return sizeof(MyDigest);}

char *csVal();

void  Init() {MD5Init(&MyContext);}

void  Update(const char *Buff, int BLen)
            {MD5Update(&MyContext, (unsigned char *)Buff, (unsigned)BLen);}

char *Final(char **Text=0)
            {MD5Final(MyDigest, &MyContext);
             if (Text) *Text = x2a((char *)MyDigest);
             return (char *)MyDigest;
            }

const char *Type() {return "md5sl";}

      bbcp_MD5_openssl() {MD5Init(&MyContext);}
     ~bbcp_MD5_openssl() {}

private:

MD5_CTX       MyContext;
unsigned char MyDigest[16];

void byteReverse(unsigned char *buf, unsigned longs);
void MD5Init(MD5_CTX *context);
void MD5Update(MD5_CTX *context, unsigned char const *buf,
               unsigned len);
void MD5Final(unsigned char digest[16], MD5_CTX *context);
void MD5Transform(uint32 buf[4], uint32 const in[16]);
};
#endif
