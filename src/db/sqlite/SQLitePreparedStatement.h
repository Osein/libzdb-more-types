/*
 * Copyright (C) 2008 Tildeslash Ltd. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef SQLITEPREPAREDSTATEMENT_H
#define SQLITEPREPAREDSTATEMENT_H
#define T IPreparedStatement_T
T SQLitePreparedStatement_new(void *stmt, int maxRows);
void SQLitePreparedStatement_free(T *P);
int SQLitePreparedStatement_setString(T P, int parameterIndex, const char *x);
int SQLitePreparedStatement_setInt(T P, int parameterIndex, int x);
int SQLitePreparedStatement_setLLong(T P, int parameterIndex, long long int x);
int SQLitePreparedStatement_setDouble(T P, int parameterIndex, double x);
int SQLitePreparedStatement_setBlob(T P, int parameterIndex, const void *x, int size);
int SQLitePreparedStatement_execute(T P);
ResultSet_T SQLitePreparedStatement_executeQuery(T P);
#undef T
#endif