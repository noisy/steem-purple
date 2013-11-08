/*
 *   This file is part of mrim-prpl.
 *
 *  mrim-prpl is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  mrim-prpl is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with mrim-prpl.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <check.h>
#include "../src/mrim-util.h"

START_TEST (test_is_valid_email)
{
	/* check all domains */
	ck_assert(is_valid_email("aaa@mail.ru"));
	ck_assert(is_valid_email("aaa@corp.mail.ru"));
	ck_assert(is_valid_email("aaa@list.ru"));
	ck_assert(is_valid_email("aaa@bk.ru"));
	ck_assert(is_valid_email("aaa@inbox.ru"));
}
END_TEST

START_TEST (test_is_valid_phone)
{
	ck_assert(is_valid_phone("81234567890"));
	ck_assert(is_valid_phone("+71234567890"));
	
	/* issue 43 */
	ck_assert(is_valid_phone("+987651234567"));
}
END_TEST

Suite *mrim_util_suite (void) {
	Suite *s = suite_create ("Utils");

	/* Core test case */
	TCase *tc_core = tcase_create ("Core");
	tcase_add_test (tc_core, test_is_valid_email);
	tcase_add_test (tc_core, test_is_valid_phone);
	
	suite_add_tcase (s, tc_core);
	return s;
}
