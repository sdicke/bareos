/*
   BAREOS® - Backup Archiving REcovery Open Sourced

   Copyright (C) 2019-2022 Bareos GmbH & Co. KG

   This program is Free Software; you can redistribute it and/or
   modify it under the terms of version three of the GNU Affero General Public
   License as published by the Free Software Foundation and included
   in the file LICENSE.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
   Affero General Public License for more details.

   You should have received a copy of the GNU Affero General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.
*/
#if defined(HAVE_MINGW)
#  include "include/bareos.h"
#  include "gtest/gtest.h"
#else
#  include "gtest/gtest.h"
#  include "include/bareos.h"
#endif

#include "lib/parse_conf.h"
#include "dird/dird_globals.h"
#include "dird/dird_conf.h"

namespace directordaemon {

struct JobControlRecord {
  JobControlRecord(int32_t JobPrio) { JobPriority = JobPrio; }
  int32_t JobPriority{0};
};


struct JcrCompare {
  bool operator()(const JobControlRecord left,
                  const JobControlRecord right) const
  {
    return left.JobPriority < right.JobPriority;
  }
};

std::multiset<JobControlRecord, JcrCompare> running_jobs;

TEST(test_jobq, JobQ)
{
  JobControlRecord jcr3(30);
  JobControlRecord jcr2(20);
  JobControlRecord jcr1(10);

  running_jobs.insert(jcr1);
  running_jobs.insert(jcr2);
  running_jobs.insert(jcr3);
  EXPECT_EQ(3, running_jobs.size());
  EXPECT_FALSE(running_jobs.empty());

  for (auto j : running_jobs) { std::cout << j.JobPriority << ":"; }
  std::cout << std::endl;

  auto pos = running_jobs.find(jcr2);
  if (pos != running_jobs.end()) {
    std::cout << "jcr2 found, removing" << std::endl;
    running_jobs.erase(pos);
  }

  for (auto j : running_jobs) { std::cout << j.JobPriority << ":"; }
  std::cout << std::endl;

  JobControlRecord jcr4(40);
  running_jobs.insert(jcr4);
  running_jobs.insert(jcr4);
  running_jobs.insert(jcr4);
  running_jobs.insert(jcr2);
  running_jobs.insert(jcr2);
  running_jobs.insert(jcr2);

  for (auto j : running_jobs) { std::cout << j.JobPriority << ":"; }
  std::cout << std::endl;
}
}  // namespace directordaemon

