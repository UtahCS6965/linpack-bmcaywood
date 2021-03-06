/*
 * Copyright 2010:
 *  - David Rohr (drohr@jwdt.org)
 *  - Matthias Bach (bach@compeng.uni-frankfurt.de)
 *  - Matthias Kretz (kretz@compeng.uni-frankfurt.de)
 *
 * This file is part of HPL-GPU.
 *
 * HPL-GPU is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * HPL-GPU is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HPL-GPU.  If not, see <http://www.gnu.org/licenses/>.
 *
 * In addition to the rules layed out by the GNU General Public License
 * the following exception is granted:
 *
 * Use with the Original BSD License.
 *
 * Notwithstanding any other provision of the GNU General Public License
 * Version 3, you have permission to link or combine any covered work with
 * a work licensed under the 4-clause BSD license into a single combined
 * work, and to convey the resulting work.  The terms of this License will
 * continue to apply to the part which is the covered work, but the special
 * requirements of the 4-clause BSD license, clause 3, concerning the
 * requirement of acknowledgement in advertising materials will apply to
 * the combination as such.
 */

#ifndef TSC_H
#define TSC_H

#ifdef _MSC_VER
#include <intrin.h>
#pragma intrinsic(__rdtsc)
#endif

class TimeStampCounter
{
    public:
        void start();
        void stop();
        unsigned long long cycles() const;

    private:
        union Data {
            unsigned long long a;
            unsigned int b[2];
        } m_start, m_end;
};

inline void TimeStampCounter::start()
{
#ifdef _MSC_VER
    m_start.a = __rdtscp();
#else
    asm volatile("rdtscp" : "=a"(m_start.b[0]), "=d"(m_start.b[1]) :: "ecx" );
#endif
}

inline void TimeStampCounter::stop()
{
#ifdef _MSC_VER
    m_end.a = __rdtscp();
#else
    asm volatile("rdtscp" : "=a"(m_end.b[0]), "=d"(m_end.b[1]) :: "ecx" );
#endif
}

inline unsigned long long TimeStampCounter::cycles() const
{
    return m_end.a - m_start.a;
}

#endif // TSC_H
