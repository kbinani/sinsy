/* ----------------------------------------------------------------- */
/*           The HMM-Based Singing Voice Synthesis System "Sinsy"    */
/*           developed by Sinsy Working Group                        */
/*           http://sinsy.sourceforge.net/                           */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2009-2013  Nagoya Institute of Technology          */
/*                           Department of Computer Science          */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the Sinsy working group nor the names of    */
/*   its contributors may be used to endorse or promote products     */
/*   derived from this software without specific prior written       */
/*   permission.                                                     */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

#include "util_log.h"
#include "LabelData.h"

using namespace sinsy;

namespace
{
const size_t NUM_P = 16;
const size_t NUM_A = 5;
const size_t NUM_B = 5;
const size_t NUM_C = 5;
const size_t NUM_D = 9;
const size_t NUM_E = 60;
const size_t NUM_F = 9;
const size_t NUM_G = 2;
const size_t NUM_H = 2;
const size_t NUM_I = 2;
const size_t NUM_J = 3;
const char* SEPARATOR_P[NUM_P] = {"", "@", "^", "-", "+", "=", "_", "%", "^", "_", "~", "-", "!", "[", "$", "]"};
const char* SEPARATOR_A[NUM_A] = {"/A:", "-", "-", "@", "~"};
const char* SEPARATOR_B[NUM_B] = {"/B:", "_", "_", "@", "|"};
const char* SEPARATOR_C[NUM_C] = {"/C:", "+", "+", "@", "&"};
const char* SEPARATOR_D[NUM_D] = {"/D:", "!", "#", "$", "%", "|", "&", ";", "-"};
const char* SEPARATOR_E[NUM_E] = {
   "/E:", "]", "^", "=", "~", "!", "@", "#", "+", "]",
   "$", "|", "[", "&", "]", "=", "^", "~", "#", "_",
   ";", "$", "&", "%", "[", "|", "]", "-", "^", "+",
   "~", "=", "@", "$", "!", "%", "#", "|", "|", "-",
   "&", "&", "+", "[", ";", "]", ";", "~", "~", "^",
   "^", "@", "[", "#", "=", "!", "~", "+", "!", "^"
};
const char* SEPARATOR_F[NUM_F] = {"/F:", "#", "#", "-", "$", "$", "+", "%", ";"};
const char* SEPARATOR_G[NUM_G] = {"/G:", "_"};
const char* SEPARATOR_H[NUM_H] = {"/H:", "_"};
const char* SEPARATOR_I[NUM_I] = {"/I:", "_"};
const char* SEPARATOR_J[NUM_J] = {"/J:", "~", "@"};
};

/*!
 constructor
 */
LabelData::LabelData() : monophoneFlag(false), outputTimeFlag(true), beginTime(-1), endTime(-1)
{
   List *p(NULL), *a(NULL), *b(NULL), *c(NULL), *d(NULL), *e(NULL), *f(NULL), *g(NULL), *h(NULL), *i(NULL), *j(NULL);

   try {
      p = new List(NUM_P, "xx");
      a = new List(NUM_A, "xx");
      b = new List(NUM_B, "xx");
      c = new List(NUM_C, "xx");
      d = new List(NUM_D, "xx");
      e = new List(NUM_E, "xx");
      f = new List(NUM_F, "xx");
      g = new List(NUM_G, "xx");
      h = new List(NUM_H, "xx");
      i = new List(NUM_I, "xx");
      j = new List(NUM_J, "xx");
   } catch (const std::bad_alloc&) {
      delete p;
      delete a;
      delete b;
      delete c;
      delete d;
      delete e;
      delete f;
      delete g;
      delete h;
      delete i;
      delete j;
      ERR_MSG("Cannot allocate memory");
   }

   data.insert(std::make_pair('p', p));
   data.insert(std::make_pair('a', a));
   data.insert(std::make_pair('b', b));
   data.insert(std::make_pair('c', c));
   data.insert(std::make_pair('d', d));
   data.insert(std::make_pair('e', e));
   data.insert(std::make_pair('f', f));
   data.insert(std::make_pair('g', g));
   data.insert(std::make_pair('h', h));
   data.insert(std::make_pair('i', i));
   data.insert(std::make_pair('j', j));
}

/*!
 destructor
 */
LabelData::~LabelData()
{
   Data::iterator itr(data.begin());
   Data::iterator itrEnd(data.end());
   for (; itr != itrEnd; ++itr) {
      delete itr->second;
   }
}

/*!
 set monophone flag
 */
void LabelData::setMonophoneFlag(bool b)
{
   monophoneFlag = b;
}

/*!
 set output flag
 */
void LabelData::setOutputTimeFlag(bool b)
{
   outputTimeFlag = b;
}

/*!
 set begin time
 */
void LabelData::setBeginTime(double d)
{
   beginTime = static_cast<INT64>(d * 1.0e+7);
}

/*!
 set end time
 */
void LabelData::setEndTime(double d)
{
   endTime = static_cast<INT64>(d * 1.0e+7);
}

/*!
 get data
 */
const std::string& LabelData::get(char category, size_t number) const
{
   Data::const_iterator itr(data.find(category));
   if (data.end() == itr) {
      throw std::runtime_error("LabelData::get() unknown category");
   }
   const List* list(itr->second);
   if ((0 == number) || (list->size() < number)) {
      throw std::runtime_error("LabelData::get() number is out of range");
   }
   return list->at(number - 1);
}

/*!
 set data (std::string)
 */
template<>
void LabelData::set<std::string>(char category, size_t number, const std::string& value)
{
   Data::iterator itr(data.find(category));
   if (data.end() == itr) {
      throw std::runtime_error("LabelData::set() unknown category");
   }
   List* list(itr->second);
   if ((0 == number) || (list->size() < number)) {
      throw std::runtime_error("LabelData::set() number is out of range");
   }
   list->at(number - 1) = value;
}

/*!
 set data (bool)
 */
template<>
void LabelData::set<bool>(char category, size_t number, const bool& value)
{
   set(category, number, std::string(value ? "1" : "0"));
}

/*!
 to stream
 */
std::ostream& sinsy::operator<<(std::ostream& os, const LabelData& obj)
{
   if (obj.outputTimeFlag) {
      os << obj.beginTime << " " << obj.endTime << " ";
   }
   LabelData::List& p = *(obj.data.find('p')->second);
   LabelData::List& a = *(obj.data.find('a')->second);
   LabelData::List& b = *(obj.data.find('b')->second);
   LabelData::List& c = *(obj.data.find('c')->second);
   LabelData::List& d = *(obj.data.find('d')->second);
   LabelData::List& e = *(obj.data.find('e')->second);
   LabelData::List& f = *(obj.data.find('f')->second);
   LabelData::List& g = *(obj.data.find('g')->second);
   LabelData::List& h = *(obj.data.find('h')->second);
   LabelData::List& i = *(obj.data.find('i')->second);
   LabelData::List& j = *(obj.data.find('j')->second);

   if (obj.monophoneFlag) {
      os << p[3];
      return os;
   }

   // p
   for (size_t idx(0); idx < NUM_P; ++idx) {
      os << SEPARATOR_P[idx] << p[idx];
   }

   // a
   for (size_t idx(0); idx < NUM_A; ++idx) {
      os << SEPARATOR_A[idx] << a[idx];
   }

   // b
   for (size_t idx(0); idx < NUM_B; ++idx) {
      os << SEPARATOR_B[idx] << b[idx];
   }

   // c
   for (size_t idx(0); idx < NUM_C; ++idx) {
      os << SEPARATOR_C[idx] << c[idx];
   }

   // d
   for (size_t idx(0); idx < NUM_D; ++idx) {
      os << SEPARATOR_D[idx] << d[idx];
   }

   // e
   for (size_t idx(0); idx < NUM_E; ++idx) {
      os << SEPARATOR_E[idx] << e[idx];
   }

   // f
   for (size_t idx(0); idx < NUM_F; ++idx) {
      os << SEPARATOR_F[idx] << f[idx];
   }

   // g
   for (size_t idx(0); idx < NUM_G; ++idx) {
      os << SEPARATOR_G[idx] << g[idx];
   }

   // h
   for (size_t idx(0); idx < NUM_H; ++idx) {
      os << SEPARATOR_H[idx] << h[idx];
   }

   // i
   for (size_t idx(0); idx < NUM_I; ++idx) {
      os << SEPARATOR_I[idx] << i[idx];
   }

   // j
   for (size_t idx(0); idx < NUM_J; ++idx) {
      os << SEPARATOR_J[idx] << j[idx];
   }

   return os;
}
