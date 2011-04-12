
/*
 *  Copyright (c) Mercury Federal Systems, Inc., Arlington VA., 2009-2011
 *
 *    Mercury Federal Systems, Incorporated
 *    1901 South Bell Street
 *    Suite 402
 *    Arlington, Virginia 22202
 *    United States of America
 *    Telephone 703-413-0781
 *    FAX 703-413-0784
 *
 *  This file is part of OpenCPI (www.opencpi.org).
 *     ____                   __________   ____
 *    / __ \____  ___  ____  / ____/ __ \ /  _/ ____  _________ _
 *   / / / / __ \/ _ \/ __ \/ /   / /_/ / / /  / __ \/ ___/ __ `/
 *  / /_/ / /_/ /  __/ / / / /___/ ____/_/ / _/ /_/ / /  / /_/ /
 *  \____/ .___/\___/_/ /_/\____/_/    /___/(_)____/_/   \__, /
 *      /_/                                             /____/
 *
 *  OpenCPI is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  OpenCPI is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenCPI.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "OcpiOsTimer.h"
#include "OcpiOsMisc.h"
#include "OcpiOsDebug.h"

#include "gtest/gtest.h"

#include <iostream>

std::ostream& operator<< ( std::ostream& s, OCPI::OS::Timer::ElapsedTime e )
{
  s << e.seconds << " seconds " << e.nanoseconds << " nanoseconds " << '\n';

  return s;
}

namespace
{
  class TestOcpiOsTimer : public ::testing::Test
  {
    protected:
      std::string d_argv0;
  };

  // Test 1: Check that the timer functionally works.
  TEST( TestOcpiOsTimer, test_1 )
  {
    OCPI::OS::Timer t;
    OCPI::OS::Timer::ElapsedTime e;
    t.start ( );
    t.stop( );
    t.getValue ( e );
    EXPECT_EQ ( true, true );
  }


  // Test 2: Sleep for a while and see that the timer isn't too far off.
  TEST( TestOcpiOsTimer, test_2 )
  {
    OCPI::OS::Timer::ElapsedTime e;
    OCPI::OS::Timer t ( true );
    OCPI::OS::sleep ( 3000 );
    t.stop ( );
    t.getValue ( e );
    unsigned int msecs = e.seconds * 1000 + e.nanoseconds / 1000000;
    // Allow for a 15% skew
    EXPECT_GE( msecs, 2550 );
    EXPECT_LE( msecs, 3450 );
  }


  // Test 3: Test that the timer can be re-started.
  TEST( TestOcpiOsTimer, test_3 )
  {
    OCPI::OS::Timer::ElapsedTime e;
    OCPI::OS::Timer t;

    for ( unsigned int i = 0; i < 10; i++ )
    {
      t.start ( );
      OCPI::OS::sleep ( 300 );
      t.stop ( );
      OCPI::OS::sleep ( 300 );
    }

    t.getValue ( e );
    unsigned int msecs = e.seconds * 1000 + e.nanoseconds / 1000000;
    // Allow for a 1% skew
    EXPECT_GE( msecs, 2970 );
    EXPECT_LE( msecs, 3030 );
  }


  // Test 4: Timer reset
  TEST( TestOcpiOsTimer, test_4 )
  {
    OCPI::OS::Timer::ElapsedTime e;
    OCPI::OS::Timer t;

    for ( unsigned int i = 0; i < 10; i++ )
    {
      t.start ( );
      OCPI::OS::sleep ( 300 );
      t.stop ( );
    }

    t.reset ( );

    for ( unsigned int i = 0; i < 10; i++ )
    {
      t.start ( );
      OCPI::OS::sleep ( 300 );
      t.stop ( );
    }

    t.getValue ( e );
    unsigned int msecs = e.seconds * 1000 + e.nanoseconds / 1000000;
    // Allow for a 1% skew
    EXPECT_GE( msecs, 2970 );
    EXPECT_LE( msecs, 3030 );
  }


  // Test 5: Timer precision: Expect better than 100ms
  TEST( TestOcpiOsTimer, test_5 )
  {
    OCPI::OS::Timer::ElapsedTime e;
    OCPI::OS::Timer::getPrecision ( e );
    EXPECT_EQ( e.seconds, 0 );
    EXPECT_NE( e.nanoseconds, 0 );
    EXPECT_LE( e.nanoseconds, 100000000 );
  }


  //  Test 6: Elapsed time comparators
  TEST( TestOcpiOsTimer, test_6 )
  {
    OCPI::OS::Timer::ElapsedTime e1, e2;

    e1.seconds = 100; e1.nanoseconds = 100;
    e2.seconds = 100; e2.nanoseconds = 100;

    EXPECT_EQ( e1, e2 );

    e1.seconds = 101; e1.nanoseconds = 100;
    e2.seconds = 100; e2.nanoseconds = 100;

    EXPECT_NE( e1, e2 );
    EXPECT_GT( e1, e2 );

    e1.seconds = 100; e1.nanoseconds = 101;
    e2.seconds = 100; e2.nanoseconds = 100;

    EXPECT_NE( e1, e2 );
    EXPECT_GT( e1, e2 );

    e1.seconds = 100; e1.nanoseconds = 99;
    e2.seconds = 100; e2.nanoseconds = 100;

    EXPECT_NE( e1, e2 );
    EXPECT_LT( e1, e2 );

    e1.seconds = 99;  e1.nanoseconds = 100;
    e2.seconds = 100; e2.nanoseconds = 100;

    EXPECT_NE( e1, e2 );
    EXPECT_LT( e1, e2 );
  }


  // Test 7: Elapsed time arithmetic
  TEST( TestOcpiOsTimer, test_7 )
  {
    OCPI::OS::Timer::ElapsedTime e1, e2, e3;

    e1.seconds = 100; e1.nanoseconds = 300;
    e2.seconds = 200; e2.nanoseconds = 400;
    e3 = e1 + e2;

    EXPECT_EQ( e3.seconds, 300 );
    EXPECT_EQ( e3.nanoseconds, 700 );

    e1.seconds = 100; e1.nanoseconds = 600000000;
    e2.seconds = 100; e2.nanoseconds = 600000000;
    e3 = e1 + e2;

    EXPECT_EQ( e3.seconds, 201 );
    EXPECT_EQ( e3.nanoseconds, 200000000 );

    e1.seconds = 300; e1.nanoseconds = 400;
    e2.seconds = 100; e2.nanoseconds = 100;
    e3 = e1 - e2;

    EXPECT_EQ( e3.seconds, 200 );
    EXPECT_EQ( e3.nanoseconds, 300 );

    e1.seconds = 300; e1.nanoseconds = 400;
    e2.seconds = 100; e2.nanoseconds = 500;
    e3 = e1 - e2;

    EXPECT_EQ( e3.seconds, 199 );
    EXPECT_EQ( e3.nanoseconds, 999999900 );
  }

} // End: namespace<unnamed>
